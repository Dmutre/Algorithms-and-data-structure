#include <stdio.h>
#include <stdlib.h>


struct Node {
    int data;
    struct Node* next;
};


struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}


struct Node* createNodeRand(){
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));

    newNode->data = -50 +rand()%101;//create number from -50 to 50
    newNode->next = NULL;
    return newNode;
};


void RandNode(struct Node** headRef, int N){//Fill our Node with random values, using push way of adding elements
    for(int i = 0; i < N; i++){
        struct Node* newNode = createNodeRand();
    }
}


void Shift(struct Node** headRef, int value) {
    struct Node* newNode = createNode(value);

    newNode->next = *headRef;
    *headRef = newNode;
}


void Push(struct Node** headRef, int value) {
    struct Node* newNode = createNode(value);

    if (*headRef == NULL) {
        *headRef = newNode;
        return;
    }
    struct Node* current = *headRef;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}


void deleteNode(struct Node** headRef, int value) {
    struct Node* current = *headRef;
    struct Node* previous = NULL;

    while (current != NULL && current->data != value) {
        previous = current;
        current = current->next;
    }
    if (current == NULL) {
        return;
    }
    if (previous == NULL) {
        *headRef = current->next;
    } else {
        previous->next = current->next;
    }
    free(current);
}


void BubbleSort1(struct Node** headRef) {//From lower to bigger
    if (*headRef == NULL) {
        return;
    }
    int swapped = 1;
    while (swapped) {
        swapped = 0;
        struct Node* current = *headRef;
        struct Node* prev = NULL;
        while (current->next != NULL) {
            if (current->data > current->next->data) {
                int temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                swapped = 1;
            }
            prev = current;
            current = current->next;
        }
    }
}


void BubbleSort2(struct Node** headRef) {//From bigger to lower
    if (*headRef == NULL) {
        return;
    }
    int swapped = 1;
    while (swapped) {
        swapped = 0;
        struct Node* current = *headRef;
        struct Node* prev = NULL;
        while (current->next != NULL) {
            if (current->data < current->next->data) {
                int temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                swapped = 1;
            }
            prev = current;
            current = current->next;
        }
    }
}


void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}


int main() {
    struct Node* head = NULL;
    bool work = true;
    int option;

    while(work){
        printf("1- Input N and fill Node with N amount of random numbers\n");
        printf("2- Input value to insert at the end of Node\n");
        printf("3- Input value to insert at the start of Node\n");
        printf("4- Print our Node\n");
        printf("5- Sort our Node\n");
        printf("6- Clear our Node\n");
        printf("7- Delete element with certain value (will delete first element with this value in Node)\n\n");

        printf("0- Stop program");
        scanf("%d", &option);
        clrscr();

        switch(option){
            case 1:
                int N;
                printf("Input N:");
                scanf("%d", &N);

        }
    }

    return 0;
}
