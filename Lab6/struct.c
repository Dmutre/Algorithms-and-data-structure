#ifndef GRAPH_H
#define GRAPH_H

#define MAX_VERTICES 100

typedef struct Graph {
    int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES];
    int numVertices;
} Graph;

void initializeGraph(Graph* graph, int numVertices);
void addEdge(Graph* graph, int source, int destination);
void removeEdge(Graph* graph, int source, int destination);
int isAdjacent(Graph* graph, int source, int destination);
void printGraph(Graph* graph);

#endif
