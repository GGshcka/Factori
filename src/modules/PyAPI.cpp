//
// Created by silly on 12.01.2025.
//

#include <python3.13/Python.h>
#include "PyAPI.h"
#include <Qt>
#include <QtWidgets>

void PyAPI::Initialize() {
    if (PyImport_AppendInittab("game", &pyInit_game) == -1) {
        fprintf(stderr, "Error: could not extend in-built modules table\n");
        return;
    }

    Py_Initialize();
    if (!Py_IsInitialized()) {
        fprintf(stderr, "Python initialization failed\n");
        return;
    }
}

PyObject* PyAPI::example(PyObject *self, PyObject *args) {
    int exampleArgVar;

    if (!PyArg_ParseTuple(args, "i", &exampleArgVar)) {
        return nullptr;
    }

    /*game->Move(direction);

    QEventLoop loop;
    GraphicalGameObject::connect(game, &Game::moveCompleted, &loop, &QEventLoop::quit);
    loop.exec();*/

    qDebug() << "This is an example of using CPy. This value:" << exampleArgVar;

    return PyLong_FromLong(1);
}

PyMethodDef PyAPI::IntegrationMethods[] = {
        {"example", example, METH_VARARGS, "Just example"},
        {nullptr, nullptr, 0, nullptr}
};

struct PyModuleDef PyAPI::IntegrationModule = {
        PyModuleDef_HEAD_INIT,
        "game",
        "Game integration module",
        -1,
        IntegrationMethods
};

PyObject* PyAPI::pyInit_game(void) {
    return PyModule_Create(&IntegrationModule);
}

void PyAPI::Finalize() {
    if (Py_IsInitialized()) {
        Py_Finalize();
    }
}

