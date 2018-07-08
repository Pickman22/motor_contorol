#ifndef APP_H
#define APP_H

extern Serial pc;
extern DigitalOut led;
extern Ticker ctrltick;

#define APP_CTRLTASK_RATE_US (1.)
#define APP_CTRLTASK_RATE_MS (APP_CTRLTASK_RATE_US * 1000)
#define APP_CTRLTASK_RATE (APP_CTRLTASK_RATE_MS * 1000)

void App_CtrlTask(void);

#endif