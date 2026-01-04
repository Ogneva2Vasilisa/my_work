/* numbers_binary_hex.c
   Hex и бинарные числа (0b — GCC/Clang extension), суффиксы и использование в битовых полях. */


struct Bits {
    unsigned int a : 4;
    unsigned int b : 8;
    unsigned int c : 20;
};

int main(void) {
    unsigned int dec = 42;
    unsigned int hex1 = 0xFF;        /* 255 */
    unsigned int hex2 = 0X1Aul;      /* 26 with UL suffix */
    unsigned long long big = 0x1ULL << 40; /* large shift of hex constant */

    printf("dec=%u hex1=%u hex2=%llu big(>>40)=%llu\n",
           dec, hex1, (unsigned long long)hex2, big >> 40);

    struct Bits s = { .a = 0xF, .b = 0b10101010, .c = 0xABCDE & 0xFFFFF };
    printf("Bits a=%u b=%u c=%u\n", s.a, s.b, s.c);

    // использование hex/binary в индексах и выражениях:
    int arr[ (0x10 >> 2) + 2 ]; // 16>>2 +2 = 6
    (void)arr;
    printf("Array size expression (no-op): %d\n", (0x10 >> 2) + 2);

    // смешанные операции:
    unsigned int mix = (0xF0 & 0b11110000) | (0x0F & 0b00001111);
    printf("mix = 0x%X (%u)\n", mix, mix);

    return 0;
}
