#ifndef __PONTE_H__
#define __PONTE_H__

#include <Python.h>
#include <vector>
#include <array>
#include <iostream>

using namespace std;

PyObject* iniciar_modulo_python();
void chamar_plotar_grafo(PyObject*, int, const vector<array<int,3>>&, bool);
void chamar_plotar_arestas(PyObject*, int, const vector<array<int,4>>&, bool);
void chamar_plotar_vertices(PyObject*, int, const vector<int>&, const vector<array<int,3>>&, bool);

#endif