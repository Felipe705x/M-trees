#include "../../headers/ss/nearest_pair.h"



/***************************
* Implementacion O(n^2)
***************************/

pair<int,int> nearestPairBrute(const vector <Cluster> &C) {
    double min = DBL_MAX;
    pair<int,int> nearest;
    int n = C.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            double distance = dist(C[i], C[j]);
            if (distance < min) {
                min = distance;
                nearest = {i,j};
            }
        }
    }
    
    return nearest;
}


/***************************
* Implementacion O(nlogn)
***************************/

// Codigo adaptado de https://www.geeksforgeeks.org/closest-pair-of-points-onlogn-implementation/

typedef struct {
	double x, y;
	int index;
} IndexedPoint;


// Needed to sort array of points according to X coordinate
static int compareX(const void* a, const void* b){
	IndexedPoint *p1 = (IndexedPoint *)a,  *p2 = (IndexedPoint *)b;
    double dif = (p1->x != p2->x) ? (p1->x - p2->x) : (p1->y - p2->y);
    if(dif == 0) return 0;    // El codigo de referencia utiliza puntos con coords enteras. Debemos modificar la comparacion para que retorne int
    return (dif > 0)? 1 : -1;
}
// Needed to sort array of points according to Y coordinate
static int compareY(const void* a, const void* b){
	IndexedPoint *p1 = (IndexedPoint *)a,  *p2 = (IndexedPoint *)b;
	double dif = (p1->y != p2->y) ? (p1->y - p2->y) : (p1->x - p2->x);
    if(dif == 0) return 0;  // El codigo de referencia utiliza puntos con coords enteras. Debemos modificar la comparacion para que retorne int
    return (dif > 0)? 1 : -1;
}


// A Brute Force method to return the smallest distance between two points
// in P[] of size n
static pair<pair<int, int>, double> bruteForce(IndexedPoint *P, int n) {
    double min = DBL_MAX;
    pair<int, int> best_indices = {0, 0};
    for (int i = 0; i < n; ++i){
        for (int j = i+1; j < n; ++j){
			Point p1 = {P[i].x, P[i].y};
			Point p2 = {P[j].x, P[j].y};
			double d = dist(p1, p2);
            if (d < min){
                min = d;
                best_indices = {P[i].index, P[j].index};
            }
		}
	}

    return {best_indices, min};
}


// A utility function to find the distance between the closest points of
// strip of a given size. All points in strip[] are sorted according to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
static pair<pair<int, int>, double> stripNearest(IndexedPoint *strip, int size, double d, pair<int, int> indices) {
	double min = d; // Initialize the minimum distance as d
    pair<int, int> best_indices = indices;
	// Pick all points one by one and try the next points till the difference
	// between y coordinates is smaller than d.
	// This is a proven fact that this loop runs at most 6 times
	for (int i = 0; i < size; ++i){
		for (int j = i+1; j < size && (strip[j].y - strip[i].y) < min; ++j){
			Point p1 = {strip[i].x, strip[i].y};
			Point p2 = {strip[j].x, strip[j].y};
			double distance = dist(p1, p2);
			if (distance < min){
				min = distance;
                best_indices = {strip[i].index, strip[j].index};
            }
		}
	}

	return {best_indices, min};
}

// A recursive function to find the smallest distance. The array Px contains
// all points sorted according to x coordinates and Py contains all points
// sorted according to y coordinates
static pair<pair<int, int>, double> nearestUtil(IndexedPoint *Px, IndexedPoint *Py, int n)
{
	// If there are 2 or 3 points, then use brute force
	if (n <= 3)
		return bruteForce(Px, n);

	// Find the middle point
	int mid = n/2;
	IndexedPoint midPoint = Px[mid];


	// Divide points in y sorted array around the vertical line.
	// Assumption: All x coordinates are distinct.
	IndexedPoint* Pyl = new IndexedPoint[mid];     // y sorted points on left of vertical line
    IndexedPoint* Pyr = new IndexedPoint[n - mid]; // y sorted points on right of vertical line

	int li = 0, ri = 0; // indexes of left and right subarrays
	for (int i = 0; i < n; i++) {
        //std::cout << li << " " << ri << std::endl;
        if ((Py[i].x < midPoint.x || (Py[i].x == midPoint.x && Py[i].y < midPoint.y)) && li < mid)
            Pyl[li++] = Py[i];
        else
            Pyr[ri++] = Py[i];
	}

	// Consider the vertical line passing through the middle point
	// calculate the smallest distance dl on left of middle point and
	// dr on right side
	pair<pair<int, int>, double> dl = nearestUtil(Px, Pyl, mid);
	pair<pair<int, int>, double> dr = nearestUtil(Px + mid, Pyr, n-mid);
    delete[] Pyl;
    delete[] Pyr;

	// Find the smaller of two distances

    pair <int, int> indices;
	double d;

    if( dl.second < dr.second){
        indices = dl.first;
        d = dl.second;
    }else{
        indices = dr.first;
        d = dr.second;
    }

	// Build an array strip[] that contains points close (closer than d)
	// to the line passing through the middle point
	IndexedPoint* strip = new IndexedPoint[n];
	int j = 0;
	for (int i = 0; i < n; i++)
		if (abs(Py[i].x - midPoint.x) < d)
			strip[j] = Py[i], j++;

	// Find the closest points in strip. Return the minimum of d and closest
	// distance is strip[]
    pair<pair<int, int>, double> ans = stripNearest(strip, j, d, indices);
    delete[] strip;

    return ans;
}

// The main function that finds the smallest distance
// This method mainly uses nearestUtil()

pair<int, int> nearestPair(const vector<Cluster> &C) {
    int n = C.size();
	IndexedPoint* Px = new IndexedPoint[n];
    IndexedPoint* Py = new IndexedPoint[n];

	for (int i = 0; i < n; i++) {
		Px[i] = {C[i].medoid.x, C[i].medoid.y, i};
		Py[i] = {C[i].medoid.x, C[i].medoid.y, i};
	}

	qsort(Px, n, sizeof(IndexedPoint), compareX);
	qsort(Py, n, sizeof(IndexedPoint), compareY);

	// Use recursive function nearestUtil() to find the smallest distance
    pair<pair<int, int>, double> ans = nearestUtil(Px, Py, n);

    delete[] Px;
    delete[] Py;
    
	return ans.first;
}