#include "../headers/minmax_split.h"

double coveringRadius(const Cluster &cluster) {
    double maxRadius = 0.0;
    vector<Point> points = cluster.points;
    Point medoid = cluster.medoid;
    for (const Point &point : points) {
        double distance = dist(point, medoid);
        if (distance > maxRadius)
            maxRadius = distance;
    }
    return maxRadius;
}

pair<Cluster, Cluster> minMaxSplit(const Cluster &c) {
    double minMaxRadius = DBL_MAX;
    pair<Cluster, Cluster> bestPair;
    vector<Point> points = c.points;

    for (int i=0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            vector<Point> points1, points2;
            Point center1 = points[i], center2 = points[j];

            for (const Point &point : points) {
                if (dist(point, center1) < dist(point, center2))
                    points1.push_back(point);
                else
                    points2.push_back(point);
            }

            Cluster cluster1 = clusterize(points1);
            Cluster cluster2 = clusterize(points2);
            double maxRadius = max(coveringRadius(cluster1), coveringRadius(cluster2));

            if (maxRadius < minMaxRadius) {
                minMaxRadius = maxRadius;
                bestPair = {cluster1, cluster2};
            }
        }
    }
    return bestPair;
}
