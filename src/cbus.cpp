#include "cbus.h"

uint8_t CBus_PackUp(const CBus* cbus, uint8_t* cbuf) {
	cbuf[0] = CBUS_BOUND;
	cbuf[1] = CBUS_BOUND;
	cbuf[2] = cbus->x;
	cbuf[3] = cbus->x;
	cbuf[4] = cbus->y;
	cbuf[5] = cbus->y;
	cbuf[6] = cbus->z;
	cbuf[7] = cbus->z;
	cbuf[8] = cbus->f;
	cbuf[9] = cbus->f;
	return 1;
}

uint8_t CBus_Unpack(CBus* cbus, const uint8_t* cbuf) {
	if (cbuf[0] != CBUS_BOUND || cbuf[1] != CBUS_BOUND) { return 0; }
	if (cbuf[2] == cbuf[3]) { cbus->x = cbuf[2]; } else { return 0; }
	if (cbuf[4] == cbuf[5]) { cbus->y = cbuf[4]; } else { return 0; }
	if (cbuf[6] == cbuf[7]) { cbus->z = cbuf[6]; } else { return 0; }
	if (cbuf[8] == cbuf[9]) { cbus->f = cbuf[8]; } else { return 0; }
	return 1;
}
