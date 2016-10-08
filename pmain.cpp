#include <Python.h>
#include "System.h"

double test(int tech_num, int reg, int pac, int flx, int che, int pay, int simulate_num, double total_service_minutes) {
	double schedule[tech_num][2] = {
		{-1, -1},
		{-1, -1},
		{-1, -1},
		{-1, -1},
		{-1, -1},
		{-1, -1},
		{-1, -1},
		{-1, -1},
		{-1, -1},
		{-1, -1},
		{-1, -1},
	};
	System system(total_service_minutes, tech_num, schedule);
	system.setTechAllocation(reg, pac, flx, che, pay);
	system.simulate(simulate_num);
	return system.getAvgStayMinutes();
}

static PyObject * _test(PyObject *self, PyObject *args) {
	int _tech_num;
	int _reg;
	int _pac;
	int _flx;
	int _che;
	int _pay;
	int _simulate_num;
	double _total_service_minutes;
	double res;

	if (!PyArg_ParseTuple(args, "iiiiiiid", &_tech_num, &_reg, &_pac, &_flx, &_che, &_pay, &_simulate_num, &_total_service_minutes))
		return NULL;
	res = test(_tech_num, _reg, _pac, _flx, _che, _pay, _simulate_num, _total_service_minutes);
	return PyLong_FromLong(res);
}

static PyMethodDef test_methods[] = {
	{
		"test",
		_test,
		METH_VARARGS,
		""
	},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initpmain(void) {
	(void) Py_InitModule("pmain", test_methods);
}