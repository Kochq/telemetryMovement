#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Queue {
    struct Node* first;
    struct Node* last;
    short count;
} Queue;

typedef struct Position {
    float latE;
    float lngE;
    float latC;
    float lngC;
} Position;

typedef struct Node {
    float azimuth;
    struct Node* next;
} Node;

typedef enum Direccion {
    FW = 0,
    RV = 1,
    STOP = 2
} Direccion;

void enqueue(Queue* queue, float azimuth) {
    Node* newNode = (Node *) malloc(sizeof(Node));
    newNode->azimuth = azimuth;

    if(queue->first == NULL) {
        queue->first = newNode;
        queue->last = newNode;
        queue->first->next = queue->last;
        queue->count++;

        return;
    }
    
    if(queue->count < 5) {
        queue->count++;
    } else {
        Node* t = queue->first;
        queue->first = queue->first->next;
        free(t);
    }

    queue->last->next = newNode;
    queue->last = newNode;
    return;
}

void printQue(Node* first) {
    while(first != NULL) {
        printf("Item: %f \n", first->azimuth);
        first = first->next;
    }
}

void clearQueue(Queue* queue) {
    while(queue->first != NULL) {
        Node* t = queue->first;
        queue->first = queue->first->next;
        free(t);
    }
    queue->first = NULL;
    queue->last = NULL;
    queue->count = 0;
}

float distanciaGeodesica(float lat1, float long1, float lat2, float long2){
	float degtorad = 0.01745329;
	float radtodeg = 57.29578;

    float dlong = (long1 - long2);
	float dvalue = (sin(lat1 * degtorad)*sin(lat2 * degtorad))+(cos(lat1*degtorad)*cos(lat2*degtorad)*cos(dlong * degtorad));

	float dd = acos(dvalue) * radtodeg;

	float km = (dd * 109642);

	return km;
}

float Calculo_AZIMUTH(float latC, float lngC, float latE, float lngE) {
    float x42 = 0;

    float dist = distanciaGeodesica(lngE, lngC, latE, lngE);
    float y42 = lngE + (dist / 109642);
    float dist2 = distanciaGeodesica(y42, lngC, latE, lngE); // Distancia entre el Centro_NORTE y el extremo

    float x2 = dist2 * dist2;
    float y2 = 2 * dist * dist;
    y42 = 1 - (x2 / y2);

    if ((y42 < 0) && (y42 < -1)) y42 = -1.0;
    else if(y42 > 1) y42 = 1.0;

    if(lngC >= lngE) { // El Centro esta mas al Este
        if(lngE > latE) x42 = 360 - (acos(y42) * 57.29578); // El Centro esta mas al Norte
        else x42 = 360 - (acos(y42) * 57.29578);
    }
    else if(lngC < lngE) { // El Centro esta mas al Oeste
        if(lngE > latE) x42 = acos(y42) * 57.29578; // El Centro esta mas al Norte
        else x42 = acos(y42) * 57.29578;
    }

    return x42;
}

void checkNewPos(Queue* queue, Position pos, Direccion* dir) {
    float currentAzimuth = Calculo_AZIMUTH(pos.lngE, pos.lngC, pos.latE, pos.lngE);

    if(queue->count == 0) {
        enqueue(queue, currentAzimuth);
    } else if(queue->count == 1) {
        bool isAzimuthInc = (currentAzimuth > queue->last->azimuth);

        if (currentAzimuth != queue->last->azimuth) {
            enqueue(queue, currentAzimuth);
            *dir = isAzimuthInc ? FW : RV;
        } else {
            *dir = STOP;
        }
    } else {
        bool isAzimuthInc = (currentAzimuth > queue->last->azimuth);

        if ((isAzimuthInc && *dir == FW) || (!isAzimuthInc && *dir == RV)) {
            printf("thing");
            enqueue(queue, currentAzimuth);
        } else if(currentAzimuth != queue->last->azimuth) {
            clearQueue(queue);
        }
    }
}

int main() {
    Queue queue;
    queue.last = NULL;
    queue.first = NULL;
    queue.count = 0;
    Direccion dir = STOP;

    Position pos;
    pos.latE = 3404.4346;
    pos.lngE = 06019.2300;
    pos.latC = 3404.3714;
    pos.lngC = 06019.3936;
    checkNewPos(&queue, pos, &dir);

    pos.latE = 3404.4346;
    pos.lngE = 06019.2300;
    pos.latC = 3404.3714;
    pos.lngC = 06019.3936;
    checkNewPos(&queue, pos, &dir);

    printQue(queue.first);
    printf("Count: %d", queue.count);
}
