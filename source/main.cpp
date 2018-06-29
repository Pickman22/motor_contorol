
#include "mbed.h"
#include "hbridge.h"
#include "app.h"

#define IMTR_A_SIZE (50)

Serial pc(USBTX, USBRX);
int main(void) {

Thread thRdCurrent(osPriorityNormal, OS_STACK_SIZE);
DigitalOut led(LED1);
App_s app;
HBridge hbridge(HBridgeA_e);
float imtr_a[IMTR_A_SIZE];
size_t idx;

    led = 1;
    app.buffer = NULL;
    app.hbridge = &hbridge;
    app.status = E_OK;
    app.imtr_a = imtr_a;
    app.imtr_sz = IMTR_A_SIZE;

	pc.baud(115200);

//	thRdCurrent.start(callback(App_ThReadCurrent, &app));

    hbridge.Enable(1);
    hbridge.DutyCycle(1.);
    for(idx = 0; idx < app.imtr_sz; idx++) {
        if(idx > 5) {
            hbridge.DutyCycle(0.);
        } else {
            hbridge.DutyCycle(1.);
        }
        imtr_a[idx] = hbridge.MilliAmps();
        Thread::wait(1);
    }
    hbridge.DutyCycle(0.);
    hbridge.Enable(0);

    for(idx = 0; idx < app.imtr_sz; idx++) {
        pc.printf("%f\n", imtr_a[idx]);
    }

    pc.printf("STOP\n");

    while(1);
    pc.puts("This shouldn't run.");
    led = 0;
	return 0;
}



 
