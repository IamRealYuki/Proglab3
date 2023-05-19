#include "coder.h"

void reset(CodeUnits *code_units)
{
    code_units->code[0] = 0;
    code_units->code[1] = 0;
    code_units->code[2] = 0;
    code_units->code[3] = 0;
    code_units->length = 0;
}

int encode(uint32_t code_point, CodeUnits *code_units)
{
    reset(code_units);
    if (code_point < 0x80)
    {
        code_units->length = 1;
        code_units->code[0] = code_point;
    }
    else if (code_point < 0x800)
    {
        code_units->length = 2;
        code_units->code[1] = 0x80 | (code_point >> 0 & 0x3f);
        code_units->code[0] = 0xC0 | (code_point >> 6 & 0x3f);
    }
    else if (code_point < 0x10000)
    {
        code_units->length = 3;
        code_units->code[2] = 0x80 | (code_point >> 0 & 0x3f);
        code_units->code[1] = 0x80 | (code_point >> 6 & 0x3f);
        code_units->code[0] = 0xE0 | (code_point >> 12 & 0x3f);
    }
    else if (code_point < 0x200000)
    {
        code_units->length = 4;
        code_units->code[3] = 0x80 | (code_point >> 0 & 0x3f);
        code_units->code[2] = 0x80 | (code_point >> 6 & 0x3f);
        code_units->code[1] = 0x80 | (code_point >> 12 & 0x3f);
        code_units->code[0] = 0xF0 | (code_point >> 18 & 0x3f);
    }
    else
        return -1;
    return 0;
}

uint32_t decode(const CodeUnits *code_unit)
{
    switch (code_unit->length)
    {
    case 1:

        return code_unit->code[0];
    case 2:
    {
        uint32_t number = code_unit->code[1] & 0x3f;
        number += (code_unit->code[0] & 0x1f) << 6;
        return number;
    }
    case 3:
    {
        uint32_t number = code_unit->code[2] & 0x3f;
        number += (code_unit->code[1] & 0x3f) << 6;
        number += (code_unit->code[0] & 0xf) << 12;
        return number;
    }
    case 4:
    {
        uint32_t number = code_unit->code[3] & 0x3f;
        number += (code_unit->code[2] & 0x3f) << 6;
        number += (code_unit->code[1] & 0x3f) << 12;
        number += (code_unit->code[0] & 0x7) << 18;
        return number;
    }
    default:
        return -1;
    }
}

int read_next_code_unit(FILE *in, CodeUnits *code_units)
{
    reset(code_units);
    if (feof(in))
        return -1;
    fread(code_units->code, sizeof(code_units->code[0]), 1, in);
    if ((code_units->code[0] & 0X80) == 0)
    {
        code_units->length = 1;
        return 0;
    }
    for (int i = 0; i < 8; i++)
    {
        if (code_units->code[0] & 0x80 >> i)
            code_units->length++;
        else
            break;
    }
    if (code_units->length == 1 || code_units->length > 4)
    {
        return read_next_code_unit(in, code_units);
    }
    for (int i = 1; i < code_units->length; i++)
    {
        if (feof(in))
            return -1;
        fread(code_units->code + i, sizeof(code_units->code[0]), 1, in);
        if ((code_units->code[i] & 0xC0) != 0x80)
        {
            fseek(in, -1, SEEK_CUR);
            return read_next_code_unit(in, code_units);
        }
    }
    return 0;
}
int write_code_unit(FILE *out, const CodeUnits *code_unit)
{
    if (fwrite(code_unit->code, sizeof(code_unit->code[0]), code_unit->length, out))
        return 0;
    else
        return -1;
}