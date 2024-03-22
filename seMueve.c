#include <math.h>
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

    float dist = distanciaGeodesica(latC, lngC, latE, lngE);
    float y42 = latC + (dist / 109642);
    float dist2 = distanciaGeodesica(y42, lngC, latE, lngE); // Distancia entre el Centro_NORTE y el extremo

    float x2 = dist2 * dist2;
    float y2 = 2 * dist * dist;
    y42 = 1 - (x2 / y2);

    if ((y42 < 0) && (y42 < -1)) y42 = -1.0;
    else if(y42 > 1) y42 = 1.0;

    if(lngC >= lngE) { // El Centro esta mas al Este
        if(latC > latE) x42 = 360 - (acos(y42) * 57.29578); // El Centro esta mas al Norte
        else x42 = 360 - (acos(y42) * 57.29578);
    }
    else if(lngC < lngE) { // El Centro esta mas al Oeste
        if(latC > latE) x42 = acos(y42) * 57.29578; // El Centro esta mas al Norte
        else x42 = acos(y42) * 57.29578;
    }

    return x42;
}

int main() {
    Node* first = NULL;
    Node* last = NULL;
    short queueCount = 0;
}
