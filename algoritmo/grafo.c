#include "grafo.h"
#include<stdlib.h>
#include <stdio.h>

void inicializarGrafo(Grafo *g, int n) {
    g->tam = n;
    g->vertices = malloc(n * sizeof(Vertice));
    for (int i = 0; i < n; i++) {
        g->vertices[i].grau = 0;
        g->vertices[i].capacidade = 10; 
        g->vertices[i].vizinhos = malloc(10 * sizeof(int));
    }
}

void addAresta(Grafo *g, int u, int v) {
    Vertice *vu = &g->vertices[u];
    Vertice *vv = &g->vertices[v];

    if (vu->grau >= vu->capacidade) {
        vu->capacidade *= 2;
        vu->vizinhos = realloc(vu->vizinhos, vu->capacidade * sizeof(int));
    }
    if (vv->grau >= vv->capacidade) {
        vv->capacidade *= 2;
        vv->vizinhos = realloc(vv->vizinhos, vv->capacidade * sizeof(int));
    }

    vu->vizinhos[vu->grau++] = v;
    vv->vizinhos[vv->grau++] = u;
}