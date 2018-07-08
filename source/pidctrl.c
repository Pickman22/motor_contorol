#include "pidctrl.h"
#include "err.h"
#include "utils.h"

#define PID_ERR(obj) (obj->y_tgt - obj->y_act)

static float _update_itgr(Pid_t* pid) {
	float err = 0;
	float itgr = 0;
	if(pid && pid->ts) {
		err = PID_ERR(pid);
		itgr += err * pid->ts;
		itgr = UTILS_SAT(pid->i_min, itgr, pid->i_max);
		pid->_itgr = itgr;
	}
	return itgr;
}

static float _update_drv(Pid_t* pid) {
	float err = 0.;
	float drv = 0.;
	if(pid && pid->ts) {
		err = PID_ERR(pid);
		pid->_drv = (err - pid->_err) / pid->ts;
		pid->_err = err;
		drv = pid->_drv;
	}
	return drv;
}

ErrCode_e Pid_update(Pid_t* pid, float y_tgt, float y_act, float *res) {
	ErrCode_e ret = E_NOK;
	float drv;
	float itgr;
	float temp;
	if(pid && pid->ts && res) {
		pid->y_tgt = y_tgt;
		pid->y_act = y_act;
		drv = _update_drv(pid);
		itgr = _update_itgr(pid);
		temp = pid->kp * PID_ERR(pid) + pid->ki * itgr + pid->kd * drv;
		pid->u = UTILS_SAT((pid->u_min), (temp), (pid->u_max));
		*res = pid->u;
	}
	return ret;
}

