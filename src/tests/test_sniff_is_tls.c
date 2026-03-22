/*
 * Unit tests for sniff_is_tls() — the protocol-sniffing helper that peeks
 * at the first byte of a new connection to detect a TLS ClientHello (0x16).
 *
 * The production function lives in socket.c (static, #ifdef HAVE_OPENSSL).
 * We copy its logic here so we can test it with socketpair() without pulling
 * in all of socket.c's dependencies.
 */

#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

typedef int bool;
#define TRUE 1
#define FALSE 0

/*
 * Copy of sniff_is_tls() from socket.c — must be kept in sync.
 */
static bool sniff_is_tls(int fd)
{
   fd_set fds;
   struct timeval tv;
   unsigned char byte;
   int n;

   FD_ZERO(&fds);
   FD_SET(fd, &fds);
   tv.tv_sec = 0;
   tv.tv_usec = 500000; /* 500ms */
   if (select(fd + 1, &fds, NULL, NULL, &tv) <= 0)
      return FALSE;
   n = recv(fd, &byte, 1, MSG_PEEK);
   if (n <= 0)
      return FALSE;
   return byte == 0x16;
}

/* Create a socketpair; write one byte to [1], return [0] for sniff. */
static int make_pair_with_byte(unsigned char b, int sv[2])
{
   if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0)
      return -1;
   if (write(sv[1], &b, 1) != 1)
      return -1;
   return 0;
}

static void test_tls_clienthello(void)
{
   int sv[2];
   assert(make_pair_with_byte(0x16, sv) == 0);
   assert(sniff_is_tls(sv[0]) == TRUE);
   /* Byte must still be in buffer (MSG_PEEK). */
   unsigned char buf;
   assert(recv(sv[0], &buf, 1, 0) == 1);
   assert(buf == 0x16);
   close(sv[0]);
   close(sv[1]);
   printf("PASS: test_tls_clienthello\n");
}

static void test_plain_iac(void)
{
   int sv[2];
   assert(make_pair_with_byte(0xFF, sv) == 0); /* telnet IAC */
   assert(sniff_is_tls(sv[0]) == FALSE);
   close(sv[0]);
   close(sv[1]);
   printf("PASS: test_plain_iac\n");
}

static void test_plain_ascii(void)
{
   int sv[2];
   assert(make_pair_with_byte('G', sv) == 0);
   assert(sniff_is_tls(sv[0]) == FALSE);
   close(sv[0]);
   close(sv[1]);
   printf("PASS: test_plain_ascii\n");
}

static void test_no_data_timeout(void)
{
   int sv[2];
   /* Create pair but write nothing — sniff should time out and return FALSE. */
   assert(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0);
   assert(sniff_is_tls(sv[0]) == FALSE);
   close(sv[0]);
   close(sv[1]);
   printf("PASS: test_no_data_timeout\n");
}

int main(void)
{
   test_tls_clienthello();
   test_plain_iac();
   test_plain_ascii();
   test_no_data_timeout();
   printf("All sniff_is_tls tests passed.\n");
   return 0;
}
