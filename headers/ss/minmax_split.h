#pragma once

#include "../general/cluster_utils.h"

double coveringRadius(const Cluster &cluster);
pair<Cluster, Cluster> minMaxSplit(const Cluster &c);
