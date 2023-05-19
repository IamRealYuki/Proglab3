#ifndef command_h
#define command_h
#include "coder.h"

int encode_file(const char *in_file_name, const char *out_file_name);
int decode_file(const char *in_file_name, const char *out_file_name);
#endif