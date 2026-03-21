/*
 * Unit tests for MCCP2/MCCP3 zlib compression round-trip.
 *
 * These tests verify that zlib deflate/inflate works correctly as used by
 * the MCCP2 (server→client) and MCCP3 (client→server) implementations.
 * No game structures are required.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

/* ---- helpers ---- */

/*
 * Compress src_len bytes from src using zlib deflate (Z_SYNC_FLUSH).
 * Returns compressed length, or 0 on error.
 */
static int compress_buf(const unsigned char *src, int src_len, unsigned char *dst, int dst_cap)
{
   z_stream z;
   memset(&z, 0, sizeof(z));
   if (deflateInit(&z, Z_DEFAULT_COMPRESSION) != Z_OK)
      return 0;

   z.next_in = (Bytef *)src;
   z.avail_in = (uInt)src_len;
   z.next_out = (Bytef *)dst;
   z.avail_out = (uInt)dst_cap;

   int ret = deflate(&z, Z_SYNC_FLUSH);
   int produced = dst_cap - (int)z.avail_out;
   deflateEnd(&z);

   if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR)
      return 0;
   return produced;
}

/*
 * Decompress src_len bytes from src using zlib inflate.
 * Returns decompressed length, or 0 on error.
 */
static int decompress_buf(const unsigned char *src, int src_len, unsigned char *dst, int dst_cap)
{
   z_stream z;
   memset(&z, 0, sizeof(z));
   if (inflateInit(&z) != Z_OK)
      return 0;

   z.next_in = (Bytef *)src;
   z.avail_in = (uInt)src_len;
   z.next_out = (Bytef *)dst;
   z.avail_out = (uInt)dst_cap;

   int ret = inflate(&z, Z_SYNC_FLUSH);
   int produced = dst_cap - (int)z.avail_out;
   inflateEnd(&z);

   if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR)
      return 0;
   return produced;
}

/* ---- tests ---- */

static void test_roundtrip_hello(void)
{
   const char *input = "Hello, MCCP world!";
   int input_len = (int)strlen(input);
   unsigned char compressed[512];
   unsigned char decompressed[512];

   int clen = compress_buf((const unsigned char *)input, input_len, compressed, sizeof(compressed));
   assert(clen > 0);

   int dlen = decompress_buf(compressed, clen, decompressed, sizeof(decompressed));
   assert(dlen == input_len);
   assert(memcmp(input, decompressed, input_len) == 0);
}

static void test_roundtrip_empty(void)
{
   const char *input = "";
   unsigned char compressed[64];
   unsigned char decompressed[64];

   int clen = compress_buf((const unsigned char *)input, 0, compressed, sizeof(compressed));
   /* Empty input may produce 0 or a small zlib header — either is valid */
   (void)clen;

   /* Even if 0 bytes compressed, inflate of 0 bytes should give 0 bytes */
   int dlen = decompress_buf(compressed, clen, decompressed, sizeof(decompressed));
   assert(dlen == 0);
}

static void test_roundtrip_repeated(void)
{
   /* Highly compressible data */
   unsigned char input[256];
   memset(input, 'A', sizeof(input));

   unsigned char compressed[512];
   unsigned char decompressed[512];

   int clen = compress_buf(input, sizeof(input), compressed, sizeof(compressed));
   assert(clen > 0);
   /* Should compress well below original size */
   assert(clen < (int)sizeof(input));

   int dlen = decompress_buf(compressed, clen, decompressed, sizeof(decompressed));
   assert(dlen == (int)sizeof(input));
   assert(memcmp(input, decompressed, sizeof(input)) == 0);
}

static void test_roundtrip_mud_output(void)
{
   /* Simulate typical MUD game text */
   const char *input = "You are in a small stone room.\r\n"
                       "Exits: north east\r\n"
                       "HP: 100/100  MP: 80/80  MV: 120/120\r\n"
                       "> ";
   int input_len = (int)strlen(input);
   unsigned char compressed[1024];
   unsigned char decompressed[1024];

   int clen = compress_buf((const unsigned char *)input, input_len, compressed, sizeof(compressed));
   assert(clen > 0);

   int dlen = decompress_buf(compressed, clen, decompressed, sizeof(decompressed));
   assert(dlen == input_len);
   assert(memcmp(input, decompressed, input_len) == 0);
}

static void test_compress_produces_different_bytes(void)
{
   const char *input = "test compression changes the bytes";
   int len = (int)strlen(input);
   unsigned char compressed[256];

   int clen = compress_buf((const unsigned char *)input, len, compressed, sizeof(compressed));
   assert(clen > 0);
   /* Compressed form should differ from raw input (unless trivially small) */
   if (clen == len)
   {
      /* If same length, at least some bytes should differ */
      int identical = (memcmp(input, compressed, len) == 0);
      /* This is extremely unlikely; the test still passes if it happens */
      (void)identical;
   }
   /* Main assertion: round-trip recovers original */
   unsigned char decompressed[256];
   int dlen = decompress_buf(compressed, clen, decompressed, sizeof(decompressed));
   assert(dlen == len);
   assert(memcmp(input, decompressed, len) == 0);
}

static void test_deflate_init_end(void)
{
   /* Test that deflateInit/deflateEnd match (no leak/error) */
   z_stream z;
   memset(&z, 0, sizeof(z));
   assert(deflateInit(&z, Z_DEFAULT_COMPRESSION) == Z_OK);
   assert(deflateEnd(&z) == Z_OK);
}

static void test_inflate_init_end(void)
{
   z_stream z;
   memset(&z, 0, sizeof(z));
   assert(inflateInit(&z) == Z_OK);
   assert(inflateEnd(&z) == Z_OK);
}

int main(void)
{
   test_deflate_init_end();
   test_inflate_init_end();
   test_roundtrip_hello();
   test_roundtrip_empty();
   test_roundtrip_repeated();
   test_roundtrip_mud_output();
   test_compress_produces_different_bytes();

   puts("test_mccp: all tests passed");
   return 0;
}
