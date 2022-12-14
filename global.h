//
// Created by moyu on 2022/9/14.
//

#ifndef GRAPH_ALGORITHM_GLOBAL_H
#define GRAPH_ALGORITHM_GLOBAL_H
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <random>
#include <cstdio>
#include <iomanip>
#include <locale>
#include <cstring>
#include <vector>
#include <cmath>
#include <chrono>
#include <stdexcept>
#include <sstream>
#include <atomic>


using namespace std;
using Adouble = atomic<double>;
const unsigned int DIST_INFINITY = std::numeric_limits<unsigned int>::max() - 1;

typedef unsigned int uint;
typedef unsigned long long ll;
#define damping_factor 0.85
#define threshold 0.001

struct OutEdge {
    uint end;
};

struct OutEdgeWeighted {
    uint end;
    uint w8;
};

struct Edge {
    uint source;
    uint end;
};

struct EdgeWeighted {
    uint source;
    uint end;
    uint w8;
};
#endif //GRAPH_ALGORITHM_GLOBAL_H
