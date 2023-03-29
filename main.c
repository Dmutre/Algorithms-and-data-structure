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


void RandNode(struct Node** headRef, int N) {
    struct Node* current = *headRef;
    while (N > 0) {
        struct Node* newNode = createNodeRand();
        newNode->next = current;
        *headRef = newNode;
        current = newNode;
        N--;
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
        printf("Lest is empty\n");
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


void deleteAllNode(struct Node** headRef){
    struct Node* current = *headRef;
    while (current != NULL) {
        struct Node* next = current->next;
        free(current);
        current = next;
    }
    *headRef = NULL;
}


void InsertionSort(struct Node** headRef) {
    if (*headRef == NULL) {
        return;
    }
    struct Node* sortedList = *headRef;
    *headRef = (*headRef)->next;
    sortedList->next = NULL;
    while (*headRef != NULL) {
        struct Node* current = *headRef;
        *headRef = (*headRef)->next;
        if (current->data > sortedList->data) {
            current->next = sortedList;
            sortedList = current;
        } else {
            struct Node* temp = sortedList;
            while (temp->next != NULL && current->data <= temp->next->data) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
    }
    *headRef = sortedList;
}


void printList(struct Node* headRef) {
    struct Node* current = headRef;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    };
    printf("\n");
}


int main() {
    struct Node* head = NULL;
    int work = 1;
    int option, N, value;


    while(work){
        printf("1- Input N and fill Node with N amount of random numbers\n");
        printf("2- Input value to insert at the end of Node\n");
        printf("3- Input value to insert at the start of Node\n");
        printf("4- Print our Node\n");
        printf("5- Sort our Node\n");
        printf("6- Clear our Node\n");
        printf("7- Delete element with certain value (will delete first element with this value in Node)\n");
        printf("8- Use already made Node for showing good situation for testing sorting\n\n");

        printf("0- Stop program\n");
        scanf("%d", &option);
        system("@cls||clear");

        switch(option){
            case 1:
                printf("Input N:");
                scanf("%d", &N);
                RandNode(&head, N);
                break;
            case 2:
                printf("Input value:\n");
                scanf("%d", &value);
                Push(&head, value);
                break;
            case 3:
                printf("Input value:\n");
                scanf("%d", &value);
                Shift(&head, value);
                break;
            case 4:
                printList(head);
                break;
            case 5:
                InsertionSort(&head);
                printList(head);
                break;
            case 6:
                deleteAllNode(&head);
                printf("Deleting is done");
                break;
            case 7:
                printf("Input value:\n");
                scanf("%d", &value);
                deleteNode(&head, value);
                break;
            case 8:
                Shift(&head, 1);
                Shift(&head, -3);
                Shift(&head, 7);
                Shift(&head, -199);
                Push(&head, 1);
                Push(&head, 10);
                Push(&head, 0);
                Push(&head, 515);
                Push(&head, 1015);
                Push(&head, 0);
                Push(&head, -15);
                Push(&head, -0);
                printList(head);
                break;
            case 0:
                printf("Programme ended");
                work = 0;
                deleteAllNode(&head);
                break;
        }
    }

    return 0;
}
