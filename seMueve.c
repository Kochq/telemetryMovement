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
    double latE;
    double lngE;
    double latC;
    double lngC;
} Position;

typedef struct Node {
    double azimuth;
    struct Node* next;
} Node;

typedef enum Direccion {
    FW = 0,
    RV = 1,
    STOP = 2
} Direccion;

void enqueue(Queue* queue, double azimuth) {
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

double distanciaGeodesica(double lat1, double long1, double lat2, double long2){
	double degtorad = 0.01745329;
	double radtodeg = 57.29578;

	double dlong = (long1 - long2);
	double dvalue = (sin(lat1 * degtorad)*sin(lat2 * degtorad))+(cos(lat1*degtorad)*cos(lat2*degtorad)*cos(dlong * degtorad));

    double v6 = acos(dvalue);
	double dd = v6 * radtodeg;

	double km = (dd * 109642);

	return km;
}

double Calculo_AZIMUTH(double latC, double lngC, double latE, double lngE) {
    double x42 = 0;

    double dist = distanciaGeodesica(latC, lngC, latE, lngE);
    double y42 = latC + (dist / 109642);
    double dist2 = distanciaGeodesica(y42, lngC, latE, lngE); // Distancia entre el Centro_NORTE y el extremo

    double x2 = dist2 * dist2;
    double y2 = 2 * dist * dist;
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

void checkNewPos(Queue* queue, Position pos, Direccion* dir) {
    double currentAzimuth = Calculo_AZIMUTH(pos.lngE, pos.lngC, pos.latE, pos.lngE);

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
            enqueue(queue, currentAzimuth);
        } else if(currentAzimuth != queue->last->azimuth) {
            clearQueue(queue);
        }
    }
}

double procesar_pos(const char *lat) {
    char grados_str[3], minutos_str[9];
    double grados, minutos, decimas_de_grado, pos_procesada;

    strncpy(grados_str, lat + (strstr(lat, ".") - lat) - 4, 2);
    grados_str[2] = '\0';
    strncpy(minutos_str, lat + (strstr(lat, ".") - lat) - 2, 8);
    minutos_str[8] = '\0';

    grados = atof(grados_str);
    minutos = atof(minutos_str);
    decimas_de_grado = minutos / 60;

    pos_procesada = -1 * (grados + decimas_de_grado);

    return pos_procesada;
}

int main() {
    Queue queue;
    queue.last = NULL;
    queue.first = NULL;
    queue.count = 0;
    Direccion dir = STOP;

    Position pos;
    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);

    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3312");
    pos.lngE = procesar_pos("06011.2296");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3258");
    pos.lngE = procesar_pos("06011.2272");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3258");
    pos.lngE = procesar_pos("06011.2272");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    pos.latE = procesar_pos("3442.3258");
    pos.lngE = procesar_pos("06011.2272");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);

    pos.latE = procesar_pos("3442.3288");
    pos.lngE = procesar_pos("06011.2242");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);

    pos.latE = procesar_pos("3442.3318");
    pos.lngE = procesar_pos("06011.2212");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);

    pos.latE = procesar_pos("3442.3348");
    pos.lngE = procesar_pos("06011.2182");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);

    pos.latE = procesar_pos("3442.3378");
    pos.lngE = procesar_pos("06011.2152");
    pos.latC = procesar_pos("3442.1444");
    pos.lngC = procesar_pos("06011.5048");
    checkNewPos(&queue, pos, &dir);


    printQue(queue.first);
    printf("Count: %d\n", queue.count);

    printf("AZ: %f", Calculo_AZIMUTH(procesar_pos("3442.3258"), procesar_pos("06011.2272"), procesar_pos("3442.1444"), procesar_pos("06011.5048")));


}
