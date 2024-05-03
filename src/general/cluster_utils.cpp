#include "../../headers/general/cluster_utils.h"


/***************************
 * POINT
 ***************************/

bool Point::operator<(const Point& other) const {
        if (x == other.x) return y < other.y; 
        return x < other.x;
    }

bool Point::operator==(const Point& other) const {
    bool val = (x == other.x);
    return val && (y == other.y);
}

ostream& operator<<(ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}


double dist(Point p, Point q) {
    double dist = pow((p.x-q.x),2) + pow((p.y-q.y),2);
    return dist;
}

/***************************
 * CLUSTER
 ***************************/

ostream& operator<<(ostream& os, const Cluster &c) {
        os << "{";
        for (const Point &point : c.points) 
            os << point;
        os << "}";
        return os;
    }

double dist(Cluster c1, Cluster c2)  {
    return dist(c1.medoid, c2.medoid);
}

void setMedoid(Cluster &c) {
    vector<Point> points = c.points;
    vector<Point> medoids;   // Borrar para producto final o bien refactorizar codigo
    Point bestMedoid;        // Trackea el candidato a medoide
    double minDistanceSum = DBL_MAX;
    for (const Point &p : points)  {
        double distance_sum = 0;
        for (const Point &q: points)
            distance_sum+=dist(p,q);
        if (distance_sum < minDistanceSum) {
            minDistanceSum = distance_sum;
            bestMedoid = p;
        }
    }
    c.medoid = bestMedoid;
}

Point calculateMedoid(const vector <Point> &points) {
    vector<Point> medoids;   // Borrar para producto final o bien refactorizar codigo
    Point bestMedoid;
    double minDistanceSum = DBL_MAX;
    for (const Point &p : points)  {
        double distance_sum = 0;
        for (const Point &q: points)
            distance_sum+=dist(p,q);
        if (distance_sum < minDistanceSum) {
            minDistanceSum = distance_sum;
            bestMedoid = p;
        }
    }
    return bestMedoid;
}

Cluster clusterize(const vector<Point> &points) {
    Cluster c;
    c.points = points;
    setMedoid(c);
    return c;
}

Cluster clusterize(const Point &point) {
    Cluster c;
    c.points = {point};
    c.medoid = point;
    return c;
}

Cluster clustUnion(const Cluster &c1, const Cluster &c2) {
    vector<Point> points1 = c1.points;
    vector<Point> points2 = c2.points;
    for (int i = 0; i < points2.size(); i++)
        points1.push_back(points2[i]);
    return clusterize(points1);
}

vector<Point> randomPoints(const ull n) {
    vector<Point> vector_point(n);
    random_device rd;
    mt19937 gen(rd());  // Para testear, podria usarse una semilla dada
    uniform_real_distribution<> dis(0.0, 100); // Range [0, 1]
    for (ull i=0; i<n; i++)
        vector_point[i] = {dis(gen), dis(gen)};
    return vector_point;
}

Point* randomPoints_static(const ull n) {
    Point* P = new Point[n];
    random_device rd;
    mt19937 gen(rd());  // Para testear, podria usarse una semilla dada
    uniform_real_distribution<> dis(0.0, 100); // Range [0, 1]
    for (ull i=0; i<n; i++)
        P[i] = {dis(gen), dis(gen)};
    return P;
}

vector<Cluster> randomSingletons(const ull n) {
    vector<Cluster> vector_singleton(n);
    random_device rd;
    mt19937 gen(rd());  // Para testear, podria usarse una semilla dada
    uniform_real_distribution<> dis(0.0, 100); // Range [0, 1]
    for (ull i=0; i<n; i++)
        vector_singleton[i] = clusterize({dis(gen), dis(gen)});
    return vector_singleton;
}