#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int val;
    struct Node *next;
} Node;

void enqueue(Node** first, Node** last, short *queueCount, int val) {
    Node* newNode = (Node *) malloc(sizeof(Node));
    newNode->val = val;

    if(*first == NULL) {
        *first = newNode;
        *last = newNode;
        (*first)->next = *last;
        (*queueCount)++;

        return;
    }
    
    if(*queueCount < 5) {
        (*queueCount)++;
    } else {
        Node* t = *first;
        *first = (*first)->next;
        free(t);
    }

    (*last)->next = newNode;
    *last = newNode;
    return;
}

void printQue(Node** first, Node** last, short queueCount) {
    Node* t = *first;
    for(short i = 0; i <= queueCount; i++) {
        printf("Item: %d \n", t->val);
        t = t->next;
    }
}

int main() {
    Node* first = NULL;
    Node* last = NULL;
    short queueCount = 0;

    enqueue(&first, &last, &queueCount, 1);
    enqueue(&first, &last, &queueCount, 2);
    enqueue(&first, &last, &queueCount, 3);
    enqueue(&first, &last, &queueCount, 4);
    enqueue(&first, &last, &queueCount, 5);
    enqueue(&first, &last, &queueCount, 6);

    printQue(&first, &last, queueCount);
}
