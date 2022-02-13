#pragma once

#include <sys/types.h>

#define IPI_DST_SELF 0x40000 // Self
#define IPI_DST_ALL  0x80000 // All including self
#define IPI_DST_OTHS 0xc0000 // All excluding self

void send_ipi(uint8_t id, uint8_t vec, uint32_t sh);
void send_strt_ipi(uint8_t id);
void send_init_ipi(uint8_t id);
void send_eoi();
