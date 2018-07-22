#include "pid.h"
#include "utils.h"
#include <float.h>
#include <string.h>

Pid::Pid() {
	memset(&obj, 0u, sizeof(Pid_t));
	obj.kp = 1.;
	obj.ki = 1.;
	obj.kd = 0.;
	obj.ts = UTILS_MAX(PID_MIN_TS, 1.e-3);
	setItgrLimits(-FLT_MAX, FLT_MAX);
	setOutputLimits(-FLT_MAX, FLT_MAX);
	obj.y_tgt = 0.;
	obj.y_act = 0.;
	obj.u = 0.;
	obj.lpf = 1.;
}

Pid::Pid(float kp, float ki, float kd, float ts, float y_tgt, float y_act,
		 float u_min, float u_max, float i_min, float i_max, float lpf, float u)
{
	memset(&obj, 0u, sizeof(Pid_t));
	obj.kp = kp;
	obj.ki = ki;
	obj.kd = kd;
	obj.ts = UTILS_MAX(PID_MIN_TS, ts);
	setItgrLimits(i_min, i_max);
	setOutputLimits(u_min, u_max);
	obj.y_tgt = y_tgt;
	obj.y_act = y_act;
	obj.u = u;
	obj.lpf = lpf;
}


float Pid::update(float y_tgt, float y_act) {
	float res;
	(void)Pid_update(&obj, y_tgt, y_act, &res);
	return res;
}

float Pid::getOutput(void) {
	return UTILS_SAT(obj.u_min, obj.u, obj.u_max);
}

float Pid::getError(void) {
	return obj.y_tgt - obj.y_act;
}

float Pid::getTarget(void) {
	return obj.y_tgt;
}

float Pid::getActual(void) {
	return obj.y_act;
}

void Pid::setTarget(float y_tgt) {
	obj.y_tgt = y_tgt;
}

void Pid::setActual(float y_act) {
	obj.y_act = y_act;
}

void Pid::setItgrLimits(float i_min, float i_max) {
	if(i_min > i_max) {
		obj.i_min = -FLT_MAX;
		obj.i_max = FLT_MAX;
	} else {
		obj.i_min = i_min;
		obj.i_max = i_max;
		obj._itgr = UTILS_SAT(obj.i_min, obj._itgr, obj.i_max);
	}
}

void Pid::getItgrLimits(float* i_min, float* i_max) {
	if(i_min) {
		*i_min = obj.i_min;
	}
	if(i_max) {
		*i_max = obj.i_max;
	}
}

void Pid::resetItgr(void) {
	obj._itgr = 0.;
}

void Pid::setOutputLimits(float u_min, float u_max) {
	if(u_min > u_max) {
		obj.u_min = -FLT_MAX;
		obj.u_max = FLT_MAX;
	} else {
		obj.u_min = u_min;
		obj.u_max = u_max;
		obj.u = UTILS_SAT((obj.u_min), (obj.u), (obj.u_max));
	}
}

void Pid::getOutputLimits(float* u_min, float* u_max) {
	if(u_min) {
		*u_min = obj.u_min;
	}
	if(u_max) {
		*u_max = obj.u_max;
	}
}

void Pid::setTs(float ts) {
	obj.ts = UTILS_MAX(PID_MIN_TS, ts);
}

float Pid::getTs(void) {
	return obj.ts;
}

void Pid::getGains(float *kp, float *ki, float* kd) {
	if(kp) {
		*kp = obj.kp;
	}
	if(ki) {
		*ki = obj.ki;
	}
	if(kd) {
		*kd = obj.kd;
	}
}

void Pid::setGains(float kp, float ki, float kd) {
	obj.kp = kp;
	obj.ki = ki;
	if(obj.ki == 0.) {
		resetItgr();
	}
	obj.kd = kd;
}

void Pid::setGain(PIDGain_e gain, float val) {
	switch(gain) {
		case Kp_e:
		obj.kp = val;
		break;

		case Ki_e:
		if(val == 0.) {
			resetItgr();
		}
		obj.ki = val;
		break;

		case Kd_e:
		obj.kd = val;
		break;
	}
}