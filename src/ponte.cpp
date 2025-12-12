#include "ponte.h"

PyObject* iniciar_modulo_python() {
    Py_Initialize();
    PyRun_SimpleString("import sys; sys.path.append('.')");
    
    PyObject* pName = PyUnicode_FromString("visualizacao");
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    
    if (pModule == nullptr) {
        PyErr_Print();
        std::cerr << "Falha ao carregar o modulo Python 'visualizacao.py'" << std::endl;
        return nullptr;
    }
    return pModule;
}

void chamar_plotar_grafo(PyObject* pModule, int n, const std::vector<std::array<int, 3>>& arestas, bool direcionado) {
    PyObject* pFunc = PyObject_GetAttrString(pModule, "plotar_grafo");

    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject* pList = PyList_New(arestas.size());
        for (size_t i = 0; i < arestas.size(); ++i) {
            PyObject* pEdge = Py_BuildValue("(iii)", arestas[i][0], arestas[i][1], arestas[i][2]);
            PyList_SetItem(pList, i, pEdge);
        }

        PyObject* pArgs = PyTuple_Pack(3, PyLong_FromLong(n), pList, direcionado ? Py_True : Py_False);
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);

        if (pValue == nullptr) PyErr_Print();
        
        Py_XDECREF(pValue);
        Py_DECREF(pArgs);
    } else {
        if (PyErr_Occurred()) PyErr_Print();
    }
    Py_XDECREF(pFunc);
}

void chamar_plotar_arestas(PyObject* pModule, int n, const std::vector<std::array<int, 4>>& arestas_coloridas, bool direcionado) {
    PyObject* pFunc = PyObject_GetAttrString(pModule, "plotar_grafo_arestas_coloridas");

    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject* pList = PyList_New(arestas_coloridas.size());
        for (size_t i = 0; i < arestas_coloridas.size(); ++i) {
            // Cria tupla (u, v, w, cor)
            PyObject* pEdge = Py_BuildValue("(iiii)", arestas_coloridas[i][0], arestas_coloridas[i][1], arestas_coloridas[i][2], arestas_coloridas[i][3]);
            PyList_SetItem(pList, i, pEdge);
        }

        PyObject* pArgs = PyTuple_Pack(3, PyLong_FromLong(n), pList, direcionado ? Py_True : Py_False);
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);

        if (pValue == nullptr) PyErr_Print();

        Py_XDECREF(pValue);
        Py_DECREF(pArgs);
    } else {
         if (PyErr_Occurred()) PyErr_Print();
    }
    Py_XDECREF(pFunc);
}

void chamar_plotar_vertices(PyObject* pModule, int n, const std::vector<int>& cores_vertices, const std::vector<std::array<int, 3>>& arestas, bool direcionado) {
    PyObject* pFunc = PyObject_GetAttrString(pModule, "plotar_grafo_vertices_coloridos");

    if (pFunc && PyCallable_Check(pFunc)) {
        PyObject* pColors = PyList_New(cores_vertices.size());
        for (size_t i = 0; i < cores_vertices.size(); ++i) {
            PyList_SetItem(pColors, i, PyLong_FromLong(cores_vertices[i]));
        }

        PyObject* pEdges = PyList_New(arestas.size());
        for (size_t i = 0; i < arestas.size(); ++i) {
            PyObject* pEdge = Py_BuildValue("(iii)", arestas[i][0], arestas[i][1], arestas[i][2]);
            PyList_SetItem(pEdges, i, pEdge);
        }

        PyObject* pArgs = PyTuple_Pack(4, PyLong_FromLong(n), pColors, pEdges, direcionado ? Py_True : Py_False);
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);

        if (pValue == nullptr) PyErr_Print();

        Py_XDECREF(pValue);
        Py_DECREF(pArgs);
    } else {
         if (PyErr_Occurred()) PyErr_Print();
    }
    Py_XDECREF(pFunc);
}
