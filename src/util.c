#include "util.h"

uint8_t hex2int(char c) {
    if ('0' <= c && c <= '9') return (c - '0');
    else if ('A' <= c && c <= 'F') return (c - 'A' + 10);
    else if ('a' <= c && c <= 'f') return (c - 'a' + 10);
    return 0;
}

void hexstr2a(uint8_t *dest, const char *src, int len) {
    for (int i=0; i<len; i+=2) {
        dest[i/2] = hex2int(src[i])<<4 | hex2int(src[i+1]);
    }
}

int a2hexstr(char *dest, const uint8_t *src, int len) {
    static const char hex_chars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    int i;
    for (i=0; i<len; i++) {
        dest[2*i + 0] = hex_chars[src[i] >> 4];
        dest[2*i + 1] = hex_chars[src[i] & 0x0F];
    }
    dest[2*len] = '\0';
    return 2*i;
}
