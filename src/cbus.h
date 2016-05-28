#ifndef __CBUS_H__
#define __CBUS_H__

#include "stdint.h"

#define CBUS_BOUND  (uint8_t)0xff // CBus bound
#define CBUS_CH_MIN (uint8_t)0x00 // CBus channel min value (0)
#define CBUS_CH_MAX (uint8_t)0xfe // CBus channel max value (254)
#define CBUS_CH_OFS (uint8_t)0x7f // CBus channel min value (127)
#define CBUS_BUF_LEN    10    // CBus buffer length

typedef struct CBus {
	uint8_t x; // horizontal
	uint8_t y; // vertical
	uint8_t z; // depth
	uint8_t f; // functional bits
}CBus;

uint8_t CBus_PackUp(const CBus* cbus, uint8_t* cbuf);
uint8_t CBus_Unpack(CBus* cbus, const uint8_t* cbuf);

#endif
