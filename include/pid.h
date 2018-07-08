#include "pidctrl.h"
#include <float.h>

#define PID_MIN_TS 1e-6

class Pid {

private:

	Pid_t obj;

public:

	Pid();

	Pid(float kp, float ki, float kd, float ts = 1e-3,
		float y_tgt = 0., float y_act = 0., float u_min = -FLT_MAX,
		float u_max = FLT_MAX, float i_min = -FLT_MIN, float i_max = FLT_MAX,
		float lpf = 1., float u = 0.);

	float update(float y_tgt, float y_act);

	float getOutput(void);

	float getError(void);

	float getTarget(void);

	float getActual(void);

	void setTarget(float y_tgt);

	void setActual(float y_act);

	void setOutputLimits(float u_min, float u_max);

	void setItgrLimits(float i_min, float i_max);

	void setTs(float ts);

};