#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

// Структура вузла списку
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// Структура графа
typedef struct Graph {
    int numVertices;
    Node** adjacencyList;
} Graph;

// Створення графа з певою кількістю вершин
Graph* createGraph(int numVertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = numVertices;

    graph->adjacencyList = (Node**)malloc(numVertices * sizeof(Node*));
    for (int i = 0; i < numVertices; i++) {
        graph->adjacencyList[i] = NULL;
    }

    return graph;
}

// Додавання ребра між вершинами source та destination
void addEdge(Graph* graph, int source, int destination) {
    // Створюємо новий вузол та заповнюємо його значеннями
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = destination;
    newNode->next = NULL;

    // Додаємо новий вузол до списку суміжності вершини source
    if (graph->adjacencyList[source] == NULL) {
        graph->adjacencyList[source] = newNode;
    } else {
        Node* temp = graph->adjacencyList[source];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    // Додаємо також ребро в зворотньому напрямку для ненапрямленого графа
    newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = source;
    newNode->next = NULL;

    // Додаємо новий вузол до списку суміжності вершини destination
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

// Друк графа
void printGraph(Graph* graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Vertex %d: ", i);
        Node* currentNode = graph->adjacencyList[i];
        while (currentNode != NULL) {
            printf("%d ", currentNode->vertex);
            currentNode = currentNode->next;
        }
        printf("\n");
    }
}

// Звільнення пам'яті, виділеної для графа
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

#endif // GRAPH_H_INCLUDED
