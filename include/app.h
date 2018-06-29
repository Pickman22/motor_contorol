#ifndef APP_H
#define APP_H

#include "hbridge.h"
#include "err.h"
#include "platform/CircularBuffer.h"

#define APP_BUF_SIZE 8
#define APP_BAUDRATE 115200
#define APP_I32_SCALING (100)
#define APP_PWM_FREQ (10e3)
#define TASK_RATE_MS_READ_CURRENT (1)

typedef struct {
	HBridge* hbridge;
	CircularBuffer<float, APP_BUF_SIZE>* buffer;
	ErrCode_e status;
	float* imtr_a;
	size_t imtr_sz;
} App_s;

extern Serial pc;

void App_ThReadCurrent(App_s* obj);

#endif