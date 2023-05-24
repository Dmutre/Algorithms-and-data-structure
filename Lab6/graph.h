#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <float.h>

#define INF FLT_MAX

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    int numVertices;
    Node** adjacencyList;
} Graph;

Graph* createGraph(int numVertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = numVertices;

    graph->adjacencyList = (Node**)malloc(numVertices * sizeof(Node*));
    for (int i = 0; i < numVertices; i++) {
        graph->adjacencyList[i] = NULL;
    }

    return graph;
}

bool isAdjacent(Graph* graph, int source, int destination) {
    Node* currentNode = graph->adjacencyList[source];
    while (currentNode) {
        if (currentNode->vertex == destination) {
            return true;
        }
        currentNode = currentNode->next;
    }
    return false;
}

void addEdge(Graph* graph, int source, int destination) {
    if (isAdjacent(graph, source, destination))//to avoid copy dependency that already exist
        return;

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = destination;
    newNode->next = NULL;

    if (graph->adjacencyList[source] == NULL) {
        graph->adjacencyList[source] = newNode;
    } else {
        Node* temp = graph->adjacencyList[source];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = source;
    newNode->next = NULL;

    if (graph->adjacencyList[destination] == NULL) {
        graph->adjacencyList[destination] = newNode;
    } else {
        Node* temp = graph->adjacencyList[destination];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void setConnectionsFromMatrix(Graph* graph, float** adjacencyMatrix) {
    int numVertices = graph->numVertices;

    for (int i = 0; i < numVertices; i++) {
        for (int j = i + 1; j < numVertices; j++) {
            if (adjacencyMatrix[i][j] == 1) {
                addEdge(graph, i, j);
            }
        }
    }
}

void printGraph(Graph* graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Vertex %d: ", (i+1));
        Node* currentNode = graph->adjacencyList[i];
        while (currentNode != NULL) {
            printf("%d ", (currentNode->vertex+1));
            currentNode = currentNode->next;
        }
        printf("\n");
    }
}

int getMinVertex(bool* inMST, float* key, int numVertices) {
    int minVertex = -1;
    float minValue = INF;

    for (int v = 0; v < numVertices; v++) {
        if (!inMST[v] && key[v] < minValue) {
            minVertex = v;
            minValue = key[v];
        }
    }

    return minVertex;
}

void destroyGraph(Graph* graph) {
    if (graph) {
        if (graph->adjacencyList) {
            for (int i = 0; i < graph->numVertices; i++) {
                Node* currentNode = graph->adjacencyList[i];
                while (currentNode != NULL) {
                    Node* nextNode = currentNode->next;
                    free(currentNode);
                    currentNode = nextNode;
                }
            }
            free(graph->adjacencyList);
        }
        free(graph);
    }
}

#endif
