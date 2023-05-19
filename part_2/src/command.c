#include "command.h"
#include <inttypes.h>

int encode_file(const char *in_file_name, const char *out_file_name)
{
    FILE *in = fopen(in_file_name, "r");
    FILE *out = fopen(out_file_name, "wb");
    if (in == NULL || out == NULL)
    {
        printf("file not found!\n");
        if (in)
            fclose(in);
        if (out)
            fclose(out);
        return -1;
    }
    uint32_t code_point;
    CodeUnits code_unit;
    while (fscanf(in, "%" SCNx32, &code_point) != EOF)
    {
        if (encode(code_point, &code_unit) == -1)
        {
            printf("Error encode!\n");
            fclose(in);
            fclose(out);
            return -1;
        }
        if (write_code_unit(out, &code_unit) != 0)
        {
            printf("Can't write in file!\n");
            fclose(in);
            fclose(out);
            return -1;
        }
    }
    fclose(in);
    fclose(out);
    return 0;
}
int decode_file(const char *in_file_name, const char *out_file_name)
{
    FILE *in = fopen(in_file_name, "r");
    FILE *out = fopen(out_file_name, "w");
    if (in == NULL || out == NULL)
    {
        printf("file not found!\n");
        if (in)
            fclose(in);
        if (out)
            fclose(out);
        return -1;
    }

    CodeUnits code_unit;
    while (!feof(in))
    {

        read_next_code_unit(in, &code_unit);

        if (feof(in))
            break;
        uint32_t x = decode(&code_unit);
        if (fprintf(out, "%" PRIx32 "\n", x) < 0)
        {
            printf("Can't write in file!\n");
            fclose(in);
            fclose(out);
            return -1;
        }
    }
    fclose(in);
    fclose(out);
    return 0;
}