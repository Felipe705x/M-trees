#pragma once

#include "../general/mtree_struct.h"

/**
 * Obtiene los puntos de las hojas asociadas a una entrada, explorando solo las regiones fuera de un determinado radio coberto centrado en q
*/
vector<Point> getOuterPoints_recursive(Entry &root_entry, Point &q, double &r);


/**
 * Obtiene los puntos de las hojas asociadas a una entrada, explorando solo las regiones fuera de un determinado radio coberto centrado en q
 * 
 * Es una traducción de la versión recursiva a una version iterativa. Esta es la version oficial de la función
*/
vector<Point> getOuterPoints(Entry &root_entry, Point &q, double &r);


pair<vector<int>, int> getHeights(vector<Node*> T_j);