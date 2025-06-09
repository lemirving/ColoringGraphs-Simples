
typedef struct {
    int *vizinhos;
    int grau;
    int capacidade;
} Vertice;

typedef struct {
    int tam;
    Vertice *vertices;
} Grafo;
void inicializarGrafo(Grafo *g, int n);
void addAresta(Grafo *g, int u, int v);

