#include "unit_test.h"
#include "../lib/playlist.h"
#include "../lib/buffer.h"
#include "../lib/byte.h"
#include "../lib/str.h"
#include <string.h>
#include <unistd.h>

/* what the callback saw, one line per entry: "title|path|length;" */
static char seen[512];

static void
collect(playlist* pl, stralloc* title, stralloc* path, uint32 len) {
  size_t n = str_len(seen);

  (void)pl;
  byte_copy(seen + n, title->len, title->s);
  n += title->len;
  seen[n++] = '|';
  byte_copy(seen + n, path->len, path->s);
  n += path->len;
  seen[n++] = '|';
  
  {
    size_t k;
    unsigned long v = len;
    char tmp[16];
    size_t d = 0;

    do {
      tmp[d++] = (char)('0' + v % 10);
      v /= 10;
    } while(v);
    for(k = 0; k < d; ++k)
      seen[n++] = tmp[d - 1 - k];
  }
  seen[n++] = ';';
  seen[n] = 0;
}

/*
 * void playlist_init(playlist*); void playlist_m3u(playlist*, buffer*); int playlist_read(playlist*);
 */
TEST(test_playlist_m3u) {
  static const char m3u[] = "#EXTM3U\n#EXTINF:123,Artist - Title\nsong.mp3\n#EXTINF:-1,Second\nhttp://x/y.ogg\nplain.mp3\n";
  playlist pl;
  buffer b;

  seen[0] = 0;
  playlist_init(&pl);
  pl.callback = collect;
  buffer_frombuf(&b, m3u, sizeof(m3u) - 1);
  playlist_m3u(&pl, &b);
  ASSERT_EQ(M3U, pl.type);
  ASSERT_EQ(1, playlist_read(&pl) > 0);
  ASSERT_EQ(3, pl.count);
  /* a title and a length belong to the next path only (the plain entry has none) */
  ASSERT_STR_EQUAL("Artist - Title|song.mp3|123;Second|http://x/y.ogg|0;|plain.mp3|0;", seen);
  playlist_free(&pl);
}

TEST(test_playlist_pls) {
  static const char pls[] = "[playlist]\nNumberOfEntries=2\nFile2=b.mp3\nTitle2=B\nLength2=20\nFile1=a.mp3\nTitle1=A\nLength1=10\nVersion=2\n";
  playlist pl;
  buffer b;

  seen[0] = 0;
  playlist_init(&pl);
  pl.callback = collect;
  buffer_frombuf(&b, pls, sizeof(pls) - 1);
  playlist_pls(&pl, &b);
  ASSERT_EQ(PLS, pl.type);
  playlist_read(&pl);
  ASSERT_EQ(2, pl.count);
  /* lines of one track may come in any order; entries are delivered in track order */
  ASSERT_STR_EQUAL("A|a.mp3|10;B|b.mp3|20;", seen);
  playlist_free(&pl);
}

TEST(test_playlist_xspf) {
  static const char xspf[] =
      "<?xml version=\"1.0\"?><playlist version=\"1\" xmlns=\"http://xspf.org/ns/0/\"><trackList>"
      "<track><location>file:///a.mp3</location><title>A</title><duration>1000</duration></track>"
      "<track><location>file:///b.mp3</location></track></trackList></playlist>";
  playlist pl;
  buffer b;

  seen[0] = 0;
  playlist_init(&pl);
  pl.callback = collect;
  buffer_frombuf(&b, xspf, sizeof(xspf) - 1);
  playlist_xspf(&pl, &b);
  ASSERT_EQ(XSPF, pl.type);
  playlist_read(&pl);
  ASSERT_EQ(2, pl.count);
  ASSERT_STR_EQUAL("A|file:///a.mp3|1000;|file:///b.mp3|0;", seen);
  playlist_free(&pl);
}

/* write each format into memory and read it back with the reader of the same type */
TEST(test_playlist_write_read_roundtrip) {
  static const playlist_type types[] = {M3U, PLS, XSPF};
  size_t i;

  for(i = 0; i < 3; ++i) {
    playlist w, r;
    playlist_entry e;
    buffer out, in;
    char mem[2048], text[2048];
    int fd[2];
    ssize_t n;

    byte_zero(&e, sizeof e);
    stralloc_copys(&e.title, "T");
    stralloc_copys(&e.path, "/p/a.mp3");
    e.length = 42;

    ASSERT_EQ(0, pipe(fd));
    buffer_init(&out, (buffer_op_ptr)write, fd[1], mem, sizeof mem);
    playlist_init(&w);
    playlist_settype(&w, types[i]);
    ASSERT_EQ(1, playlist_write_start(&out, &w) >= 0);
    ASSERT_EQ(1, playlist_write_entry(&out, &w, &e));
    ASSERT_EQ(1, playlist_write_finish(&out, &w) >= 0);
    buffer_flush(&out);
    close(fd[1]);
    n = read(fd[0], text, sizeof text);
    close(fd[0]);
    ASSERT_EQ(1, n > 10);
    ASSERT_EQ(1, w.count == 1);

    seen[0] = 0;
    playlist_init(&r);
    r.callback = collect;
    buffer_frombuf(&in, text, (size_t)n);
    if(types[i] == M3U)
      playlist_m3u(&r, &in);
    else if(types[i] == PLS)
      playlist_pls(&r, &in);
    else
      playlist_xspf(&r, &in);
    playlist_read(&r);
    ASSERT_EQ(1, r.count);
    /* xspf writes a file:// URL for a plain path */
    ASSERT_EQ(1, str_equal(seen, "T|/p/a.mp3|42;") || str_equal(seen, "T|file:///p/a.mp3|42;"));
    playlist_free(&r);
    stralloc_free(&e.title);
    stralloc_free(&e.path);
  }
}

TEST(test_playlist_free_twice) {
  playlist pl;
  buffer b;

  playlist_init(&pl);
  buffer_frombuf(&b, "a\n", 2);
  playlist_m3u(&pl, &b);
  playlist_free(&pl);
  playlist_free(&pl); /* harmless */
  playlist_init(&pl);
  playlist_free(&pl);
}

#define RUN_PLAYLIST_TESTS() \
  RUN(test_playlist_m3u); \
  RUN(test_playlist_pls); \
  RUN(test_playlist_xspf); \
  RUN(test_playlist_write_read_roundtrip); \
  RUN(test_playlist_free_twice)

TESTS(playlist) { RUN_PLAYLIST_TESTS(); }
