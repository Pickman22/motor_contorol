#include "pid.h"
#include "utils.h"
#include <float.h>


Pid::Pid() {
	obj.kp = 1.;
	obj.ki = 1.;
	obj.kd = 0.;
	obj.ts = UTILS_MAX(PID_MIN_TS, 1.e-3);
	setItgrLimits(-FLT_MIN, FLT_MAX);
	setOutputLimits(FLT_MIN, FLT_MAX);
	obj.y_tgt = 0.;
	obj.y_act = 0.;
	obj.u = 0.;
	obj.lpf = 1.;
}

Pid::Pid(float kp, float ki, float kd, float ts, float y_tgt, float y_act,
		 float u_min, float u_max, float i_min, float i_max, float lpf, float u)
{
	obj.kp = kp;
	obj.ki = ki;
	obj.kd = kd;
	obj.ts = UTILS_MAX(PID_MIN_TS, 1.e-3);
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
	return obj.u;
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
	if(i_min >= i_max) {
		obj.i_min = -FLT_MAX;
		obj.i_max =  FLT_MAX;
	} else {
		obj.i_min = UTILS_MIN(i_min, 0.);
		obj.i_max = UTILS_MAX(i_max, 0.);
	}
}

void Pid::setOutputLimits(float u_min, float u_max) {
	if(u_min >= u_max) {
		obj.u_min = -FLT_MAX;
		obj.u_max =  FLT_MAX;
	} else {
		obj.u_min = UTILS_MIN(u_min, 0.);
		obj.u_max = UTILS_MAX(u_max, 0.);
	}
}

void Pid::setTs(float ts) {
	obj.ts = UTILS_MAX(PID_MIN_TS, ts);
}