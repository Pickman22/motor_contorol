#ifndef HBRIDGE_H
#define HBRIDGE_H

#include "mbed.h"
#include <stdint.h>

extern Serial pc;

#define HBRIDGE_ENABLE (1)
#define HBRIDGE_DISABLE (0)

typedef enum HBridgeId {
	HBridgeA_e = 0,
	HBrridgeB_e,
} HBridgeId_e;


class HBridge {

public:

	HBridge(HBridgeId_e id, uint8_t en = HBRIDGE_DISABLE);

	int SetFrq(float freqhz);

	int Enable(uint8_t en);

	float MilliAmps(void);

	float Amps(void);

	int DutyCycle(float dc);

private:

	DigitalOut lside;
	
	PwmOut hside;
	
	AnalogIn ifb;

	DigitalOut enable;
	
	DigitalIn fault;
};

#endif