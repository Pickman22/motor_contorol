#include "hbridge.h"
#include "utils.h"

#define HBRIDGE_A_LS PTC3
#define HBRIDGE_A_HS PTC4
#define HBRIDGE_A_IFB PTE23
#define HBRIDGE_A_ENABLE PTE21
#define HBRIDGE_A_FAULT PTE20

#define HBRIDGE_B_LS PTC1
#define HBRIDGE_B_HS PTC2
#define HBRIDGE_B_IFB PTE23
#define HBRIDGE_B_ENABLE PTE21
#define HBRIDGE_B_FAULT PTE20

#define HBRIDGE_PWM_FREQ 1000

/*
	Constant to convert from ADC counts to Milliamps:

	Im = 375 * Vadc / Feedback_R,
	
	where Vadc is the millivolts convertion factor,
	375 is the H-Bridge feedbkac scaling factor,
	Feedback_R is the 220 ohms feedback resistor. 
*/
const float HBridgeFbackScaling_c = (375 * 3300) / 220;

typedef struct HBridge_Config {
	PinName lside;
	PinName hside;
	PinName ifb;
	PinName enable;
	PinName fault;
} HBridge_Config_s;

static HBridge_Config_s _hb_config_s[] = {
	{
		.lside = HBRIDGE_A_LS,
		.hside = HBRIDGE_A_HS,
		.ifb = HBRIDGE_A_IFB,
		.enable = HBRIDGE_A_ENABLE,
		.fault = HBRIDGE_A_FAULT,
	},

	{
		.lside = HBRIDGE_B_LS,
		.hside = HBRIDGE_B_HS,
		.ifb = HBRIDGE_B_IFB,
		.enable = HBRIDGE_B_ENABLE,
		.fault = HBRIDGE_B_FAULT,
	}
};

HBridge::HBridge(HBridgeId_e id, uint8_t en/*HBRIDGE_ENABLE */) :
	lside(_hb_config_s[id].lside),
	hside(_hb_config_s[id].hside),
	ifb(_hb_config_s[id].ifb),
	enable(_hb_config_s[id].enable),
	fault(_hb_config_s[id].fault)
{
	enable = en;
	hside.period(1. / HBRIDGE_PWM_FREQ);
	hside = 0.;
	lside = 0;
}

int HBridge::Enable(uint8_t en) {
	enable = UTILS_MAX(en, 1);
	return 0;
}

int HBridge::DutyCycle(float dc) {
	dc = UTILS_MAX(dc, -1.);
	dc = UTILS_MIN(dc, 1.);

	if(dc < 0.) {
		hside = dc + 1.;
		lside = 1;
	} else {
		hside = dc;
		lside = 0;
	}

	return 0;
}

float HBridge::MilliAmps(void) {
	return ifb.read() * HBridgeFbackScaling_c;
}

float HBridge::Amps(void) {
	return MilliAmps() / 1000.;
}
