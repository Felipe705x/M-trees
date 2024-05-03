#pragma once

#include "../headers/cluster_utils.h"

double coveringRadius(const Cluster &cluster);
pair<Cluster, Cluster> minMaxSplit(const Cluster &c);
