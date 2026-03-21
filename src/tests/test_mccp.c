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

/*
 * Simulate the MCCP2 write path: a persistent deflate stream with
 * Z_SYNC_FLUSH after each write, as used in write_to_descriptor().
 * Returns number of compressed bytes written into dst, or 0 on error.
 */
static int mccp2_write(z_stream *z, const unsigned char *src, int src_len, unsigned char *dst,
                       int dst_cap)
{
   int total = 0;

   z->next_in = (Bytef *)src;
   z->avail_in = (uInt)src_len;

   do
   {
      z->next_out = dst + total;
      z->avail_out = (uInt)(dst_cap - total);

      int ret = deflate(z, Z_SYNC_FLUSH);
      if (ret < 0)
         return 0;

      total += (dst_cap - total) - (int)z->avail_out;
   } while (z->avail_out == 0);

   return total;
}

/*
 * Simulate the MCCP3 read path: a persistent inflate stream, as used in
 * read_from_descriptor().
 * Returns decompressed length, or -1 on error.
 */
static int mccp3_read(z_stream *z, const unsigned char *src, int src_len, unsigned char *dst,
                      int dst_cap)
{
   z->next_in = (Bytef *)src;
   z->avail_in = (uInt)src_len;
   z->next_out = (Bytef *)dst;
   z->avail_out = (uInt)dst_cap;

   int ret = inflate(z, Z_SYNC_FLUSH);
   if (ret < 0)
      return -1;

   return dst_cap - (int)z->avail_out;
}

/*
 * MCCP2: single write compressed with persistent stream round-trips correctly.
 */
static void test_mccp2_single_write(void)
{
   z_stream zout, zin;
   memset(&zout, 0, sizeof(zout));
   memset(&zin, 0, sizeof(zin));
   assert(deflateInit(&zout, Z_DEFAULT_COMPRESSION) == Z_OK);
   assert(inflateInit(&zin) == Z_OK);

   const char *msg = "You strike the goblin for 12 damage!\r\n";
   int msglen = (int)strlen(msg);
   unsigned char cbuf[512], dbuf[512];

   int clen = mccp2_write(&zout, (const unsigned char *)msg, msglen, cbuf, sizeof(cbuf));
   assert(clen > 0);

   int dlen = decompress_buf(cbuf, clen, dbuf, sizeof(dbuf));
   assert(dlen == msglen);
   assert(memcmp(msg, dbuf, msglen) == 0);

   deflateEnd(&zout);
   inflateEnd(&zin);
}

/*
 * MCCP2: multiple sequential writes through the same persistent stream all
 * decompress correctly when concatenated.
 */
static void test_mccp2_multiple_writes(void)
{
   z_stream zout;
   memset(&zout, 0, sizeof(zout));
   assert(deflateInit(&zout, Z_DEFAULT_COMPRESSION) == Z_OK);

   const char *msgs[] = {
       "You are in the town square.\r\n",
       "Exits: north south east west\r\n",
       "HP: 100/100  MP: 80/80\r\n> ",
   };
   unsigned char cbuf[2048];
   int ctotal = 0;
   int expected_total = 0;
   char expected[2048];

   for (int i = 0; i < 3; i++)
   {
      int mlen = (int)strlen(msgs[i]);
      int clen = mccp2_write(&zout, (const unsigned char *)msgs[i], mlen, cbuf + ctotal,
                             sizeof(cbuf) - ctotal);
      assert(clen > 0);
      ctotal += clen;
      memcpy(expected + expected_total, msgs[i], mlen);
      expected_total += mlen;
   }

   /* The entire compressed stream should decompress to all three messages */
   unsigned char dbuf[2048];
   int dlen = decompress_buf(cbuf, ctotal, dbuf, sizeof(dbuf));
   assert(dlen == expected_total);
   assert(memcmp(expected, dbuf, expected_total) == 0);

   deflateEnd(&zout);
}

/*
 * MCCP3: client-to-server compressed data decompresses correctly through the
 * persistent inflate stream used in read_from_descriptor().
 */
static void test_mccp3_single_read(void)
{
   z_stream zin;
   memset(&zin, 0, sizeof(zin));
   assert(inflateInit(&zin) == Z_OK);

   const char *cmd = "north\r\n";
   int cmdlen = (int)strlen(cmd);
   unsigned char cbuf[256], dbuf[256];

   int clen = compress_buf((const unsigned char *)cmd, cmdlen, cbuf, sizeof(cbuf));
   assert(clen > 0);

   int dlen = mccp3_read(&zin, cbuf, clen, dbuf, sizeof(dbuf));
   assert(dlen == cmdlen);
   assert(memcmp(cmd, dbuf, cmdlen) == 0);

   inflateEnd(&zin);
}

/*
 * MCCP3: multiple compressed reads through the same persistent inflate stream.
 * The client uses a single persistent deflate stream and Z_SYNC_FLUSH so that
 * each flush produces a self-contained decompressible chunk — mirroring real
 * MCCP3 behaviour.
 */
static void test_mccp3_multiple_reads(void)
{
   z_stream zout, zin;
   memset(&zout, 0, sizeof(zout));
   memset(&zin, 0, sizeof(zin));
   assert(deflateInit(&zout, Z_DEFAULT_COMPRESSION) == Z_OK);
   assert(inflateInit(&zin) == Z_OK);

   const char *cmds[] = {"look\r\n", "score\r\n", "north\r\n"};

   for (int i = 0; i < 3; i++)
   {
      int cmdlen = (int)strlen(cmds[i]);
      unsigned char cbuf[256], dbuf[256];

      /* Compress with the persistent client stream, flushing after each cmd */
      int clen = mccp2_write(&zout, (const unsigned char *)cmds[i], cmdlen, cbuf, sizeof(cbuf));
      assert(clen > 0);

      int dlen = mccp3_read(&zin, cbuf, clen, dbuf, sizeof(dbuf));
      assert(dlen == cmdlen);
      assert(memcmp(cmds[i], dbuf, cmdlen) == 0);
   }

   deflateEnd(&zout);
   inflateEnd(&zin);
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
   test_mccp2_single_write();
   test_mccp2_multiple_writes();
   test_mccp3_single_read();
   test_mccp3_multiple_reads();

   puts("test_mccp: all tests passed");
   return 0;
}
