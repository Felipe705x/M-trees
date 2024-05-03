#pragma once

#include "commons.h"


struct Point {
    double x;
    double y;

    // Define un orden para los puntos
    bool operator<(const Point& other) const;

    // Define igualdad entre puntos
    bool operator==(const Point& other) const;

    // Define representación visual de los puntos
    friend ostream& operator<<(ostream& os, const Point& p);
};


struct Cluster {
    vector<Point> points;
    Point medoid;

    // Define representación visual de clusters
    friend ostream& operator<<(ostream& os, const Cluster &c);
};

// Distancia Eucleidiana (cuadrada) entre dos puntos
double dist(Point p, Point q);

// Distancia Eucleidiana (cuadrada) entre dos clusters, según distancia entre medioides
double dist(Cluster c1, Cluster c2);

// Calcula y asigna el primer medioide encontrado en el cluster
void setMedoid(Cluster &c);

// Calcula y retorna el primer medioide encontrado en un conjunto de puntos
Point calculateMedoid(const vector <Point> &points);

// Crea un cluster con el conjunto de puntos especificados y se asigna el medioide
Cluster clusterize(const vector<Point> &points);

// Crea un cluster con el único punto especificado y se asigna el medioide (en este caso sería el mismo punto)
Cluster clusterize(const Point &point);

// Une dos clusters mediante la union entre los puntos de ambos clusters y la posterior asignación de medioide
Cluster clustUnion(const Cluster &c1, const Cluster &c2);

// Genera un vector de n puntos aleatorios (x e y uniformemente distribuidos en [0,1])
vector<Point> randomPoints(const ull n);

// Genera un arreglo de n puntos aleatorios (x e y uniformemente distribuidos en [0,1])
Point* randomPoints_static(const ull n) ;

// Genera un vector de n clusters singletons {p} (donde p.x e p.y uniformemente distribuidos en [0,1])
vector<Cluster> randomSingletons(const ull n);