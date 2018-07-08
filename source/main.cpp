#include "mbed.h"
#include "app.h"

extern void App_CtrlTask(void);

Serial pc(USBTX, USBRX);
Ticker ctrltick;
DigitalOut led(LED1);

int main(void) {
    ctrltick.attach_us(&App_CtrlTask, APP_CTRLTASK_RATE);
	return 0;
}



 
