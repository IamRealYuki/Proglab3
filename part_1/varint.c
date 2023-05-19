#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdint.h>

uint32_t generate_number()
{
    const int r = rand();
    const int p = r % 100;
    if (p < 90)
    {
        return r % 128;
    }
    if (p < 95)
    {
        return r % 16384;
    }
    if (p < 99)
    {
        return r % 2097152;
    }
    return r % 268435455;
}

void reset(uint8_t *buf)
{
    for (int i = 0; i < 4; i++)
    {
        buf[i] = 0;
    }
}

size_t encode_varint(uint32_t value, uint8_t *buf)
{
    assert(buf != NULL);
    uint8_t *cur = buf;
    while (value >= 0x80)
    {
        const uint8_t byte = (value & 0x7f) | 0x80;
        *cur = byte;
        value >>= 7;
        ++cur;
    }
    *cur = value;
    ++cur;
    return cur - buf;
}

uint32_t decode_varint(const uint8_t **bufp)
{
    const uint8_t *cur = *bufp;
    uint8_t byte = *cur++;
    uint32_t value = byte & 0x7f;
    size_t shift = 7;
    while (byte >= 0x80)
    {
        byte = *cur++;
        value += (byte & 0x7f) << shift;
        shift += 7;
    }
    *bufp = cur;
    return value;
}

int main()
{
    FILE *uncompressed = fopen("uncompressed numbers", "w");
    FILE *compressed = fopen("compressed numbers", "w");
    uint32_t number, compressed_number;
    uint8_t buf[4];

    for (uint32_t i = 0; i < 1000000; i++)
    {
        number = generate_number();
        fwrite(&number, sizeof(uint32_t), 1, uncompressed);
        compressed_number = encode_varint(number, buf);
        fwrite(buf, sizeof(uint8_t), compressed_number, compressed);
    }

    fclose(uncompressed);
    fclose(compressed);

    uncompressed = fopen("uncompressed numbers", "r");
    compressed = fopen("compressed numbers", "r");
    double un_size = 0, com_size = 0;
    for (un_size = 0; !feof(uncompressed); un_size++)
    {
        fread(&number, sizeof(uint32_t), 1, uncompressed);
        if (feof(uncompressed))
            break;
        reset(buf);
        for (int i = 0; i < 4 && !feof(compressed); i++)
        {
            buf[i] = getc(compressed);
            com_size++;
            if (buf[i] < 0x80)
                break;
        }
        const uint8_t *bufp = buf;
        compressed_number = decode_varint(&bufp);
        if (number != compressed_number)
        {
            printf("Error!\n");
            return -1;
        }
    }
    printf("Размер файла: %f\nРазмер сжатого файла: %f\nОтношения сжатого файла и не сжатого: %f\n", un_size * sizeof(uint32_t), com_size * sizeof(uint8_t), un_size * sizeof(uint32_t) / com_size);
    return 0;
}