#ifndef _CRC32_H
#define _CRC32_H
#include <stdint.h>
uint32_t crc32(uint32_t crc, const void *buf, int size);
uint32_t thps_gen_checksum(const char *path);
#endif