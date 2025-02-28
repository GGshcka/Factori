//
// Created by silly on 12.01.2025.
//

#ifndef NEGOTIUM_PYAPI_H
#define NEGOTIUM_PYAPI_H


#include <python3.13/Python.h>

class PyAPI {
public:
    static void Initialize();
    static void Finalize();
private:
    static PyObject *example(PyObject* self, PyObject* args);
    static PyObject *pyInit_game(void);
    static PyMethodDef IntegrationMethods[];
    static struct PyModuleDef IntegrationModule;
};

#endif //NEGOTIUM_PYAPI_H
