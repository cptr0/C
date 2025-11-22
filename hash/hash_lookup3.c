/**
 * @addtogroup hash Hash algorithms
 * @{
 * @file hash_lookup3.c
 * @author [cptr0](https://github.com/cptr0)
 * @brief 32-bit [Jenkins hash](https://en.wikipedia.org/wiki/Jenkins_hash_function) algorithm
 */

#include <assert.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>

// Rotate x left by k bits
#define rot(x,k) (((x) << (k)) | ((x) >> (32-(k))))

/**
 * @brief 32-bit Jenkins Lookup3 algorithm implementation
 *
 * @param s NULL terminated ASCII string to hash
 * @return 32-bit hash result
 */

uint32_t lookup3_32(const char *key, uint32_t seed) {
    uint32_t a, b, c;
    a = b = 0x9e3779b9;
    c = seed;

    size_t i = 0;

    size_t length = strlen(key);

    while (length - i >= 12) {
        a += (uint32_t)key[i] | ((uint32_t)key[i+1] << 8) |
             ((uint32_t)key[i+2] << 16) | ((uint32_t)key[i+3] << 24);
        b += (uint32_t)key[i+4] | ((uint32_t)key[i+5] << 8) |
             ((uint32_t)key[i+6] << 16) | ((uint32_t)key[i+7] << 24);
        c += (uint32_t)key[i+8] | ((uint32_t)key[i+9] << 8) |
             ((uint32_t)key[i+10] << 16) | ((uint32_t)key[i+11] << 24);

        a -= c;  a ^= rot(c,4);  c += b;
        b -= a;  b ^= rot(a,6);  a += c;
        c -= b;  c ^= rot(b,8);  b += a;
        a -= c;  a ^= rot(c,16); c += b;
        b -= a;  b ^= rot(a,19); a += c;
        c -= b;  c ^= rot(b,4);  b += a;

        i += 12;
    }

    uint32_t remainingA = 0, remainingB = 0, remainingC = 0;
    switch (length - i) {
        case 11: remainingC += ((uint32_t)key[i+10]) << 16;
        case 10: remainingC += ((uint32_t)key[i+9]) << 8;
        case 9:  remainingC += (uint32_t)key[i+8];
        case 8:  remainingB += ((uint32_t)key[i+7]) << 24;
        case 7:  remainingB += ((uint32_t)key[i+6]) << 16;
        case 6:  remainingB += ((uint32_t)key[i+5]) << 8;
        case 5:  remainingB += (uint32_t)key[i+4];
        case 4:  remainingA += ((uint32_t)key[i+3]) << 24;
        case 3:  remainingA += ((uint32_t)key[i+2]) << 16;
        case 2:  remainingA += ((uint32_t)key[i+1]) << 8;
        case 1:  remainingA += (uint32_t)key[i];
                 a += remainingA;
                 b += remainingB;
                 c += remainingC;
    }

    c ^= b; c -= rot(b,14);
    a ^= c; a -= rot(c,11);
    b ^= a; b -= rot(a,25);
    c ^= b; c -= rot(b,16);
    a ^= c; a -= rot(c,4);
    b ^= a; b -= rot(a,14);
    c ^= b; c -= rot(b,24);

    return c;
}
/**
 * @brief Test function for lookup3_32 function
 *
 * @returns None
 */
void test_lookup3_32()
{
    assert(lookup3_32("Hello, Jenkins!", 0) == 2484708164);
    assert(lookup3_32("Hello Jenkinss", 0) == 850494015);
    assert(lookup3_32("Hello, Jenkins", 0) == 3393305742);
    assert(lookup3_32("hello, jenkins!", 0) == 3521406038);

    printf("%s", "Test passed.\n");
}

/**
 * @brief Main function
 *
 * @returns 0 on successful program exit
 */
int main()
{
    test_lookup3_32();
    return 0;
}
