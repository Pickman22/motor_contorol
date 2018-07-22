#include "mbed.h"
#include "app.h"
#include "utils.h"
#include "err.h"
#include "pid.h"
#include "hbridge.h"

Pid pidMtrCtrl(1.0, 1.0, 0.0, (APP_CTRLTASK_RATE));
HBridge hbridge(HBridgeA_e, HBRIDGE_DISABLE);

void App_CtrlTask(void) {
	led = !led;
	return;
}

void App_LogTask(void) {
	return;
}