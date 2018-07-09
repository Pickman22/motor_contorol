#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"
#include "../../../include/pid.h"

using namespace utest::v1;

void test_default_ctor(void) {
	Pid pid;
	
	float kp, ki, kd;
	float i_min, i_max;
	float u_min, u_max;

	pid.getGains(&kp, &ki, &kd);
	pid.getItgrLimits(&i_min, &i_max);
	pid.getOutputLimits(&u_min, &u_max);

	TEST_ASSERT_EQUAL_FLOAT(kp, 1.0);
	TEST_ASSERT_EQUAL_FLOAT(ki, 1.0);
	TEST_ASSERT_EQUAL_FLOAT(kd, 0.0);
	TEST_ASSERT_EQUAL_FLOAT(pid.getTs(), 0.001);
	TEST_ASSERT_EQUAL_FLOAT(pid.getTarget(), 0.);
	TEST_ASSERT_EQUAL_FLOAT(pid.getOutput(), 0.);
	TEST_ASSERT_EQUAL_FLOAT(pid.getError(), 0.);
	TEST_ASSERT_EQUAL_FLOAT(u_min, -FLT_MAX);
	TEST_ASSERT_EQUAL_FLOAT(u_max, FLT_MAX);
	TEST_ASSERT_EQUAL_FLOAT(i_min, -FLT_MAX);
	TEST_ASSERT_EQUAL_FLOAT(i_max, FLT_MAX);
}

void test_ctor(void) {
	float kp = 10.;
	float ki = 32.;
	float kd = -1.;
	float ts = 0.0001;
	float y_tgt = 100.;
	float y_act = 23.;
	float u_min = -1.;
	float u_max = 1.;
	float i_min = -1.;
	float i_max = 1.;
	float lpf = 0.3;
	float u = 0.01;

	float kptest, kitest, kdtest;

	Pid pid(kp, ki, kd, ts, y_tgt, y_act, u_min, u_max, i_min, i_max, lpf, u);
	pid.getGains(&kptest, &kitest, &kdtest);

	TEST_ASSERT_EQUAL_FLOAT(kp, kptest);
	TEST_ASSERT_EQUAL_FLOAT(ki, kitest);
	TEST_ASSERT_EQUAL_FLOAT(kd, kdtest);
	TEST_ASSERT_EQUAL_FLOAT(pid.getTs(), ts);
	TEST_ASSERT_EQUAL_FLOAT(pid.getTarget(), y_tgt);
	TEST_ASSERT_EQUAL_FLOAT(pid.getOutput(), u);
	TEST_ASSERT_EQUAL_FLOAT(pid.getError(), (y_tgt - y_act));

}

void test_limits(void) {
	Pid pid;

	float u_min = 0;
	float u_max = 0;
	
	// Test valid limits.
	pid.setOutputLimits(-100, 100);
	pid.getOutputLimits(&u_min, &u_max);
	TEST_ASSERT_EQUAL_FLOAT(u_min, -100.);
	TEST_ASSERT_EQUAL_FLOAT(u_max, 100.);

	// Test invalid limits. Limits get
	// restored to default values.
	pid.setOutputLimits(100, -100);
	pid.getOutputLimits(&u_min, &u_max);
	TEST_ASSERT_EQUAL_FLOAT(u_min, -FLT_MAX);
	TEST_ASSERT_EQUAL_FLOAT(u_max, FLT_MAX);

	// Test valid limits.
	pid.setItgrLimits(-100., 100.);
	pid.getItgrLimits(&u_min, &u_max);
	TEST_ASSERT_EQUAL_FLOAT(u_min, -100.);
	TEST_ASSERT_EQUAL_FLOAT(u_max, 100.);

	// Test invalid limits. Limites get
	// restored to default values.
	pid.setItgrLimits(100., -100.);
	pid.getItgrLimits(&u_min, &u_max);
	TEST_ASSERT_EQUAL_FLOAT(u_min, -FLT_MAX);
	TEST_ASSERT_EQUAL_FLOAT(u_max, FLT_MAX);
}

void test_dynamics(void) {
	int i = 1000;
	float u = 0.0;
	float err = 0.;
	float ts = 1.0;
	float y_act = 0.;
	float y_tgt = 1.;

	// Test integrator.
	Pid pid(0.0, 1.0, 0.0, ts);

	pid.setTarget(y_tgt);
	pid.setActual(y_act);

	while(i--) {
		u = pid.update(y_tgt, y_act);
		err += (y_tgt - y_act) * ts;
		TEST_ASSERT_EQUAL_FLOAT(err, u);
	}

	pid.resetItgr();
	u = pid.update(y_tgt, y_act);
	err = 0.;
	err += (y_tgt - y_act) * ts;
	TEST_ASSERT_EQUAL_FLOAT(err, u);

	// Test derivative.

	// Test PID action.
}

utest::v1::status_t test_setup(const size_t number_of_cases) {
    // Setup Greentea using a reasonable timeout in seconds
    GREENTEA_SETUP(40, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

// Test cases
Case cases[] = {
    Case("Testing PID Class Default Constructor", test_default_ctor),
    Case("Testing PID Class Constructor", test_ctor),
    Case("Testing PID Class output/integrator limits", test_limits),
    Case("Testing PID Class dynamics", test_dynamics),
};

Specification specification(test_setup, cases);

// Entry point into the tests
int main() {
    return !Harness::run(specification);
}
