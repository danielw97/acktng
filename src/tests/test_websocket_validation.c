#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "config.h"
#define DEC_GLOBALS_H 1
#include "ack.h"

static const char *find_http_header_value(const char *headers, const char *header_name)
{
    const char *line_start;
    size_t name_len;

    if (headers == NULL || header_name == NULL || header_name[0] == '\0')
        return NULL;

    name_len = strlen(header_name);
    line_start = headers;

    while (*line_start != '\0')
    {
        const char *line_end = strstr(line_start, "\r\n");
        size_t line_len;

        if (line_end == NULL)
            break;

        line_len = (size_t)(line_end - line_start);
        if (line_len == 0)
            break;

        if (line_len > name_len + 1 && strncasecmp(line_start, header_name, name_len) == 0 &&
            line_start[name_len] == ':')
        {
            const char *value_start = line_start + name_len + 1;
            while (*value_start == ' ' || *value_start == '\t')
                value_start++;
            return value_start;
        }

        line_start = line_end + 2;
    }

    return NULL;
}

static bool header_value_contains_token(const char *value, const char *token)
{
    const char *cursor;
    size_t token_len;

    if (value == NULL || token == NULL || token[0] == '\0')
        return false;

    token_len = strlen(token);
    cursor = value;

    while (*cursor != '\0' && *cursor != '\r' && *cursor != '\n')
    {
        const char *entry_start = cursor;
        const char *entry_end;
        size_t entry_len;

        while (*entry_start == ' ' || *entry_start == '\t' || *entry_start == ',')
            entry_start++;

        if (*entry_start == '\0' || *entry_start == '\r' || *entry_start == '\n')
            break;

        entry_end = entry_start;
        while (*entry_end != '\0' && *entry_end != '\r' && *entry_end != '\n' && *entry_end != ',')
            entry_end++;

        while (entry_end > entry_start && (entry_end[-1] == ' ' || entry_end[-1] == '\t'))
            entry_end--;

        entry_len = (size_t)(entry_end - entry_start);
        if (entry_len == token_len && strncasecmp(entry_start, token, token_len) == 0)
            return true;

        cursor = (*entry_end == ',') ? entry_end + 1 : entry_end;
    }

    return false;
}

static bool validate_websocket_handshake_request(const char *request,
                                                 char *key_out,
                                                 size_t key_out_size,
                                                 size_t *header_len_out)
{
    const char *end_headers;
    const char *upgrade_value;
    const char *key_start;
    size_t key_len;

    if (request == NULL)
        return false;

    if (strncmp(request, "GET ", 4) != 0)
        return false;

    end_headers = strstr(request, "\r\n\r\n");
    if (end_headers == NULL)
        return false;

    if (header_len_out != NULL)
        *header_len_out = (size_t)((end_headers + 4) - request);

    upgrade_value = find_http_header_value(request, "Upgrade");
    if (upgrade_value == NULL || !header_value_contains_token(upgrade_value, "websocket"))
        return false;

    key_start = find_http_header_value(request, "Sec-WebSocket-Key");
    if (key_start == NULL)
        return false;

    key_len = strcspn(key_start, "\r\n");
    if (key_len == 0)
        return false;

    if (key_out != NULL && key_out_size > 0)
    {
        if (key_len >= key_out_size)
            return false;
        memcpy(key_out, key_start, key_len);
        key_out[key_len] = '\0';
    }

    return true;
}

static void test_accepts_valid_request(void)
{
    char key[64];
    size_t header_len = 0;
    const char *request =
        "GET / HTTP/1.1\r\n"
        "Host: localhost:4000\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "\r\n"
        "look\n";

    assert(validate_websocket_handshake_request(request, key, sizeof(key), &header_len));
    assert(strcmp(key, "dGhlIHNhbXBsZSBub25jZQ==") == 0);
    assert(header_len == (size_t)(strstr(request, "\r\n\r\n") - request + 4));
}

static void test_accepts_case_insensitive_headers_and_tokens(void)
{
    char key[64];
    const char *request =
        "GET / HTTP/1.1\r\n"
        "host: localhost:4000\r\n"
        "Connection: keep-alive, Upgrade\r\n"
        "uPgRaDe: WebSocket\r\n"
        "sec-websocket-key: lowercase-header-key==\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "\r\n";

    assert(validate_websocket_handshake_request(request, key, sizeof(key), NULL));
    assert(strcmp(key, "lowercase-header-key==") == 0);
}

static void test_rejects_non_get_or_incomplete(void)
{
    char key[64];
    size_t header_len = 0;

    assert(!validate_websocket_handshake_request("POST / HTTP/1.1\r\n\r\n", key, sizeof(key), &header_len));
    assert(!validate_websocket_handshake_request("GET / HTTP/1.1\r\nHost: local\r\n", key, sizeof(key), &header_len));
}

static void test_rejects_missing_required_headers(void)
{
    char key[64];
    const char *missing_upgrade =
        "GET / HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Sec-WebSocket-Key: abc123==\r\n"
        "\r\n";
    const char *missing_key =
        "GET / HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Upgrade: websocket\r\n"
        "\r\n";

    assert(!validate_websocket_handshake_request(missing_upgrade, key, sizeof(key), NULL));
    assert(!validate_websocket_handshake_request(missing_key, key, sizeof(key), NULL));
}

static void test_rejects_oversized_key_buffer(void)
{
    char tiny_key[8];
    const char *request =
        "GET / HTTP/1.1\r\n"
        "Upgrade: websocket\r\n"
        "Sec-WebSocket-Key: this-key-is-way-too-long\r\n"
        "\r\n";

    assert(!validate_websocket_handshake_request(request, tiny_key, sizeof(tiny_key), NULL));
}

int main(void)
{
    test_accepts_valid_request();
    test_accepts_case_insensitive_headers_and_tokens();
    test_rejects_non_get_or_incomplete();
    test_rejects_missing_required_headers();
    test_rejects_oversized_key_buffer();

    puts("test_websocket_validation: all tests passed");
    return 0;
}
