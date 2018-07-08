#ifndef PIDCTRL_H
#define PIDCTRL_H

#include "err.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Pid_s {
	float y_tgt;
	float y_act;
	float kp, ki, kd;
	float lpf;
	float ts;
	float u;
	float u_min, u_max;
	float i_min, i_max;
	float _err;
	float _itgr;
	float _drv;
} Pid_t;

ErrCode_e Pid_update(Pid_t* pid, float y_tgt, float y_act, float *res);

#ifdef __cplusplus
}
#endif

#endif