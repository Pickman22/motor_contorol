#include "mbed.h"
#include "app.h"
#include "utils.h"
#include "err.h"

void App_ThReadCurrent(App_s* obj) {
float imtr;
float dc;
size_t idx;
uint8_t* tmp;

	obj->hbridge->Enable(1);
	obj->hbridge->DutyCycle(1.);
	idx = 0;
	while(1) {
//		obj->hbridge->DutyCycle(1.);
	
		if(idx < obj->imtr_sz) {
//			imtr->imtr_a[idx++] = obj->hbridge->MilliAmps();
		}
 
		wait(TASK_RATE_MS_READ_CURRENT);
	}
}