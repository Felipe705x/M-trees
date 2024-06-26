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

// La distancia es eucleidiana
double dist(Point p, Point q) {
    double dist = sqrt((p.x-q.x)*(p.x-q.x) + (p.y-q.y)*(p.y-q.y));
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

// C no vacio
void setMedoid(Cluster &c) {
    vector<Point> points = c.points;
    if (c.points.size() <= 1) {
        c.medoid = points[0];
        return;
    }
    Point medoid;        // Trackea el candidato a medoide
    double minRadius = DBL_MAX;
    for (const Point &c_medoid : points) {   // Para cada punto elegido como posible medoide
        double c_radius = 0.0;                  // Radio cobertor actual dado c_medoid como denominado medoide
        for (const Point &point : points) {     // Calculamos su maximo radio (i.e. el radio cobertor)
            if (point == c_medoid)
                continue;
            double distance = dist(c_medoid, point);
            if (distance > c_radius)
                c_radius = distance;
        }
        if (c_radius < minRadius) {
            minRadius = c_radius;
            medoid = c_medoid;
        }
    }
    c.medoid = medoid;
}

// Vector no vacio
Point calculateMedoid(const vector <Point> &points) {
    Point medoid;
    if (points.size() <= 1) {
        medoid = points[0];
        return medoid;
    }
    double minRadius = DBL_MAX;
    for (const Point &c_medoid : points) {   // Para cada punto elegido como posible medoide
        double c_radius = 0.0;                  // Radio cobertor actual dado c_medoid como denominado medoide
        for (const Point &point : points) {     // Calculamos su maximo radio (i.e. el radio cobertor)
            if (point == c_medoid)
                continue;
            double distance = dist(c_medoid, point);
            if (distance > c_radius)
                c_radius = distance;
        }
        if (c_radius < minRadius) {
            minRadius = c_radius;
            medoid = c_medoid;
        }
    }
    return medoid;
}

Cluster clusterize(const vector<Point> &points) {
    Cluster c;
    c.points = points;
    c.medoid =  calculateMedoid(points);
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
    points1.reserve(points1.size() + points2.size());
    for (int i = 0; i < points2.size(); i++)
        points1.push_back(points2[i]);
    return clusterize(points1);
}


/***************************
 * RNG
 ***************************/

vector<Point> randomPoints(const ull n, pair<double, double> range, optional<int> seed) { 
    mt19937 gen;
    if (seed.has_value())
        gen.seed(seed.value());
    else {
        random_device rd;
        gen.seed(rd());
    }

    vector<Point> vector_point(n);
    uniform_real_distribution<> dis(range.first, range.second);
    for (ull i=0; i<n; i++)
        vector_point[i] = {dis(gen), dis(gen)};
    return vector_point;
}

Point* randomPoints_static(const ull n, pair<double, double> range, optional<int> seed) {
    mt19937 gen;
    if (seed.has_value())
        gen.seed(seed.value());
    else {
        random_device rd;
        gen.seed(rd());
    }

    Point* P = new Point[n];
    uniform_real_distribution<> dis(range.first, range.second);
    for (ull i=0; i<n; i++)
        P[i] = {dis(gen), dis(gen)};
    return P;
}

vector<Cluster> randomSingletons(const ull n, pair<double, double> range, optional<int> seed) {
    mt19937 gen;
    if (seed.has_value())
        gen.seed(seed.value());
    else {
        random_device rd;
        gen.seed(rd());
    }

    vector<Cluster> vector_singleton(n);
    random_device rd;
    uniform_real_distribution<> dis(range.first, range.second);
    for (ull i=0; i<n; i++)
        vector_singleton[i] = clusterize({dis(gen), dis(gen)});
    return vector_singleton;
}