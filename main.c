#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int *vizinhos;
    int grau;
    int capacidade;
} Vertice;

typedef struct {
    int tam;
    Vertice *vertices;
} Grafo;

typedef struct {
    int sat;
    int degree;
    int indice;
} infoVertice;

void inicializarGrafo(Grafo *g, int n) {
    g->tam = n;
    g->vertices = malloc(n * sizeof(Vertice));
    for (int i = 0; i < n; i++) {
        g->vertices[i].grau = 0;
        g->vertices[i].capacidade = 10; // capacidade inicial para vizinhos
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

int compararInfoVertice(const void *a, const void *b) {
    infoVertice *na = (infoVertice *)a;
    infoVertice *nb = (infoVertice *)b;

    if (na->sat != nb->sat)
        return nb->sat - na->sat;

    if (na->degree != nb->degree)
        return nb->degree - na->degree;

    return na->indice - nb->indice;
}

void ordenarFila(infoVertice *queue, int size) {
    qsort(queue, size, sizeof(infoVertice), compararInfoVertice);
}

void removerPrimeiro(infoVertice *queue, int *size) {
    for (int i = 1; i < *size; i++)
        queue[i - 1] = queue[i];
    (*size)--;
}

int contarCores(int *vetorCores, int tam) {
    int max = 0;
    for (int i = 0; i < tam; i++) {
        if (vetorCores[i] > max)
            max = vetorCores[i];
    }
    return max + 1; 
}

void Dsatur(Grafo *g) {
    int cor[g->tam];
    infoVertice q_infos[g->tam];
    bool ehColorido[g->tam];
    int queue_size = g->tam;

    for (int j = 0; j < g->tam; j++) {
        cor[j] = -1;
        q_infos[j].indice = j;
        q_infos[j].degree = g->vertices[j].grau;
        q_infos[j].sat = 0;
    }

    ordenarFila(q_infos, queue_size);

    while (queue_size > 0) {
        infoVertice vertAtual = q_infos[0];
        removerPrimeiro(q_infos, &queue_size);
        int u = vertAtual.indice;

        for (int i = 0; i < g->tam; i++)
            ehColorido[i] = false;

        for (int i = 0; i < g->vertices[u].grau; i++) {
            int viz = g->vertices[u].vizinhos[i];
            if (cor[viz] != -1)
                ehColorido[cor[viz]] = true;
        }

        int menorCor = 0;
        while (ehColorido[menorCor])
            menorCor++;

        cor[u] = menorCor;

        for (int i = 0; i < g->vertices[u].grau; i++) {
            int viz = g->vertices[u].vizinhos[i];
            if (cor[viz] == -1) {
                int coresUsadas[g->tam];
                for (int j = 0; j < g->tam; j++)
                    coresUsadas[j] = 0;

                int cont = 0;
                for (int j = 0; j < g->vertices[viz].grau; j++) {
                    int v = g->vertices[viz].vizinhos[j];
                    if (cor[v] != -1 && !coresUsadas[cor[v]]) {
                        coresUsadas[cor[v]] = 1;
                        cont++;
                    }
                }

                for (int k = 0; k < queue_size; k++) {
                    if (q_infos[k].indice == viz) {
                        q_infos[k].sat = cont;
                        break;
                    }
                }
            }
        }

        ordenarFila(q_infos, queue_size);
    }

    printf("\nColoração final:\n");
    for (int i = 0; i < g->tam; i++) {
        printf("Vértice %d ---> Cor %d\n", i, cor[i]);
    }

    int totalCores = contarCores(cor, g->tam);
    printf("\nNúmero total de cores usadas: %d\n", totalCores);
}


//Leitura do grafo
int carregarGrafo(char *nomeArquivo, Grafo *g) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    char c;
    int n_vertices = 0, n_arestas = 0;
    while ((c = fgetc(arquivo)) != EOF) {
        if (c == 'p') {
            char tipo[10];
            fscanf(arquivo, "%s %d %d", tipo, &n_vertices, &n_arestas);
            inicializarGrafo(g, n_vertices);
        } else if (c == 'e') {
            int u, v;
            fscanf(arquivo, "%d %d", &u, &v);
            addAresta(g, u - 1, v - 1);
        } else {
            while ((c = fgetc(arquivo)) != '\n' && c != EOF);
        }
    }

    fclose(arquivo);
    return 1;
}

void liberarGrafo(Grafo *g) {
    for (int i = 0; i < g->tam; i++)
        free(g->vertices[i].vizinhos);
    free(g->vertices);
}

void descricaoArquivo(char *nomeArquivo){
    char linha[100];
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
 
    }
    printf("\n");
    char c;  
    while (fgets(linha,sizeof(linha),arquivo) != NULL) {
        if(linha[0] == 'c' || linha[0] == 'p'){
            printf("%s", linha+2);
        }
    }
    fclose(arquivo);
}

int main() {
    clock_t start,end;
    double time_used;

    start = clock();

    Grafo g;
    char nomeArquivo[] = "inithx.i.1.col";
    
    descricaoArquivo(nomeArquivo);

    if (carregarGrafo(nomeArquivo, &g)) {
        Dsatur(&g);
        liberarGrafo(&g);
    }
    end = clock();

    time_used = ((double) (end-start) / CLOCKS_PER_SEC);

    printf("Tempo de execução: %.2lfs\n", time_used);
    return 0;
}
