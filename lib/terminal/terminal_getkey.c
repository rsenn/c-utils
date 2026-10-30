#include "../terminal_internal.h"
#include "../ucs.h"

#if WINDOWS_NATIVE

/* the key of a virtual key code, 0 if it is no named key */
static unsigned
named_key(WORD vk) {
  switch(vk) {
    case VK_UP: return TERMINAL_KEY_UP;
    case VK_DOWN: return TERMINAL_KEY_DOWN;
    case VK_RIGHT: return TERMINAL_KEY_RIGHT;
    case VK_LEFT: return TERMINAL_KEY_LEFT;
    case VK_HOME: return TERMINAL_KEY_HOME;
    case VK_END: return TERMINAL_KEY_END;
    case VK_PRIOR: return TERMINAL_KEY_PAGEUP;
    case VK_NEXT: return TERMINAL_KEY_PAGEDOWN;
    case VK_INSERT: return TERMINAL_KEY_INSERT;
    case VK_DELETE: return TERMINAL_KEY_DELETE;
  }
  if(vk >= VK_F1 && vk <= VK_F12)
    return TERMINAL_KEY_F(vk - VK_F1 + 1);
  return 0;
}

int
terminal_getkey(struct terminal* t, struct terminal_key* key) {
  INPUT_RECORD rec;
  DWORD n;

  for(;;) {
    KEY_EVENT_RECORD* ke = &rec.Event.KeyEvent;
    DWORD state;
    WCHAR ch;

    if(!GetNumberOfConsoleInputEvents((HANDLE)t->in, &n) || n == 0)
      return 0;
    if(!ReadConsoleInputW((HANDLE)t->in, &rec, 1, &n) || n == 0)
      return -1;

    key->mods = 0;

    if(rec.EventType == WINDOW_BUFFER_SIZE_EVENT) {
      key->code = TERMINAL_KEY_RESIZE;
      return 1;
    }
    if(rec.EventType != KEY_EVENT || !ke->bKeyDown)
      continue;

    state = ke->dwControlKeyState;
    ch = ke->uChar.UnicodeChar;

    if(!ch) {
      if(!(key->code = named_key(ke->wVirtualKeyCode)))
        continue;
    } else if(ch >= 0xd800 && ch < 0xdc00) {
      t->hi = ch;
      continue;
    } else if(ch >= 0xdc00 && ch < 0xe000) {
      if(!t->hi)
        continue;
      key->code = 0x10000 + (((unsigned)t->hi - 0xd800) << 10) + (ch - 0xdc00);
      t->hi = 0;
    } else if(ke->wVirtualKeyCode == VK_RETURN)
      key->code = '\n';
    else if(ke->wVirtualKeyCode == VK_BACK)
      key->code = 127;
    else
      key->code = ch;

    if(state & SHIFT_PRESSED)
      key->mods |= TERMINAL_SHIFT;
    if(state & LEFT_ALT_PRESSED)
      key->mods |= TERMINAL_ALT;
    if(state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
      key->mods |= TERMINAL_CTRL;
    /* shift is already in a character, and AltGr is not alt */
    if(key->code < 0x110000)
      key->mods &= ~(unsigned)TERMINAL_SHIFT;
    if((state & RIGHT_ALT_PRESSED) && (state & LEFT_CTRL_PRESSED))
      key->mods = 0;
    return 1;
  }
}

#else

#include <unistd.h>

/* fill makes sure that at least need bytes are buffered, by reading.
 * Returns 1, 0 at the end of input, -1 on error. */
static int
fill(struct terminal* t, unsigned need) {
  unsigned i, n = t->inlen - t->inpos;
  ssize_t r;

  for(i = 0; i < n; ++i)
    t->inbuf[i] = t->inbuf[t->inpos + i];
  t->inpos = 0;
  t->inlen = n;

  while(t->inlen < need) {
    if((r = read(t->infd, t->inbuf + t->inlen, sizeof(t->inbuf) - t->inlen)) <= 0)
      return r < 0 ? -1 : 0;
    t->inlen += (unsigned)r;
  }
  return 1;
}

/* the key of an xterm style sequence: the final byte and the numbers
 * before it, where the second number is 1 plus the modifier bits */
static unsigned
named_key(char final, const int* p, int np) {
  static const unsigned tilde[] = {0,
                                   TERMINAL_KEY_HOME,
                                   TERMINAL_KEY_INSERT,
                                   TERMINAL_KEY_DELETE,
                                   TERMINAL_KEY_END,
                                   TERMINAL_KEY_PAGEUP,
                                   TERMINAL_KEY_PAGEDOWN,
                                   TERMINAL_KEY_HOME,
                                   TERMINAL_KEY_END};
  static const unsigned char fkeys[] = {15, 5, 17, 6, 18, 7, 19, 8, 20, 9, 21, 10, 23, 11, 24, 12};
  unsigned i;

  switch(final) {
    case 'A': return TERMINAL_KEY_UP;
    case 'B': return TERMINAL_KEY_DOWN;
    case 'C': return TERMINAL_KEY_RIGHT;
    case 'D': return TERMINAL_KEY_LEFT;
    case 'H': return TERMINAL_KEY_HOME;
    case 'F': return TERMINAL_KEY_END;
    case 'P':
    case 'Q':
    case 'R':
    case 'S': return TERMINAL_KEY_F(final - 'P' + 1);
    case '~':
      if(np < 1)
        return 0;
      if(p[0] > 0 && p[0] < (int)(sizeof(tilde) / sizeof(tilde[0])))
        return tilde[p[0]];
      for(i = 0; i < sizeof(fkeys); i += 2)
        if(fkeys[i] == p[0])
          return TERMINAL_KEY_F(fkeys[i + 1]);
  }
  return 0;
}

/* decode the UTF-8 character that starts with the byte just taken */
static unsigned
utf8_key(struct terminal* t, unsigned char c) {
  unsigned need = c >= 0xf0 ? 4 : c >= 0xe0 ? 3 : c >= 0xc0 ? 2 : 1, code;
  size_t r;

  if(need == 1)
    return 0xfffd;
  --t->inpos;
  if(fill(t, need) != 1)
    return 0xfffd;
  if(!(r = scan_utf8_ucs4((const char*)t->inbuf + t->inpos, t->inlen - t->inpos, &code))) {
    ++t->inpos;
    return 0xfffd;
  }
  t->inpos += (unsigned)r;
  return code;
}

int
terminal_getkey(struct terminal* t, struct terminal_key* key) {
  int r;

  for(;;) {
    unsigned char c;

    if(t->inpos == t->inlen && (r = fill(t, 1)) != 1)
      return r;

    c = t->inbuf[t->inpos++];
    key->mods = 0;
    key->code = c;

    if(c >= 0x80)
      key->code = utf8_key(t, c);
    else if(c == 27 && t->inpos < t->inlen) {
      unsigned char b = t->inbuf[t->inpos];

      if(b == '[' || b == 'O') {
        int p[4] = {0, 0, 0, 0}, np = 0;
        unsigned code;

        ++t->inpos;

        /* digits and semicolons up to the final byte */
        for(;;) {
          if(t->inpos == t->inlen) {
            /* cut off sequence */
            key->code = 27;
            return 1;
          }
          b = t->inbuf[t->inpos++];
          if(b >= '0' && b <= '9') {
            if(!np)
              np = 1;
            p[np - 1] = p[np - 1] * 10 + (b - '0');
          } else if(b == ';') {
            if(np < 4)
              ++np;
          } else if(b >= 0x40 && b <= 0x7e)
            break;
        }

        if(b == 'Z') {
          key->code = '\t';
          key->mods = TERMINAL_SHIFT;
          return 1;
        }
        if(!(code = named_key((char)b, p, np)))
          continue;
        key->code = code;
        if(np > 1 && p[1] > 1) {
          key->mods = (unsigned)(p[1] - 1) & 7;
        }
      } else {
        /* alt with a key */
        ++t->inpos;
        key->mods = TERMINAL_ALT;
        key->code = b >= 0x80 ? utf8_key(t, b) : b;
      }
    }
    return 1;
  }
}
#endif
