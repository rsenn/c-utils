#include "utf8.h"

const char _u8cmaxlen = 6;
const char _u8bit = '\x80';
const char _u8bits[] = {'\x0', '\xc0', '\xe0', '\xf0', '\xf8', '\xfc'};
const wchar_t _wcsrange[] = {L'\x0', L'\x80', L'\x800', L'\x10000', L'\x200000', L'\x4000000'};
const char _u8fvallen[] = {7, 5, 4, 3, 2, 1};
const char _u8vallen = 6;

size_t u8stowcs(wchar_t *wcs, const char *u8s) {
    size_t len = 0;
    wchar_t *p_wcs = wcs;
    size_t rp_u8s = 0;
    char u8;
    while ((u8 = *(u8s + rp_u8s++)) != '\0') {
        wchar_t wc = L'\xfffd';
        char b;
        for (b = _u8cmaxlen - 1; b >= 0; b--)
            if ((_u8bits[b] & u8) == _u8bits[b])
                break;
        if (b >= 0) {
            len++;
            if (wcs) {
                wc = (u8 ^ _u8bits[b]) << b * _u8vallen;
                while (b-- > 0) {
                    u8 = *(u8s + rp_u8s++);
                    if ((_u8bit & u8) == _u8bit)
                        wc += (u8 ^ _u8bit) << b * _u8vallen;
                    else {
                        wc = L'\xfffd';
                        break;
                    }
                }
                *p_wcs++ = wc;
            }
        }
    }
    if (wcs)
        *p_wcs = L'\0';
    return len;
}

size_t wcstou8s(char *u8s, const wchar_t *wcs) {
    size_t len = 0;
    char *p_u8s = u8s;
    size_t rp_wcs = 0;
    wchar_t wc;
    while ((wc = *(wcs + rp_wcs++)) != L'\0') {
        char b;
        for (b = _u8cmaxlen - 1; b; b--)
            if (_wcsrange[b] < wc)
                break;
        len += b + 1;
        if (u8s) {
            *p_u8s = _u8bits[b];
            p_u8s += b;
            char c = b;
            while (c-- > 0) {
                *p_u8s =  _u8bit + (wc & L'\x3f');
                wc >>= 6;
                p_u8s--;
            }
            *p_u8s++ |= wc;
            p_u8s += b;
        }
    }
    if (u8s)
        *p_u8s = '\0';
    return len;
}
