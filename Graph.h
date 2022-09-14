//
// Created by moyu on 2022/9/14.
//

#ifndef GRAPH_ALGORITHM_GRAPH_H
#define GRAPH_ALGORITHM_GRAPH_H
#include "global.h"

template <class E>
class Graph {
private:

public:
    bool isWeighted;
    bool islarge;

    uint num_nodes;     // 图顶点数
    uint num_edges;     // 边数
    uint *offset;       // CSR顶点偏移量数组
    E *edgeList;        // CSR边数组

    uint *outDegree;
    bool *label1;
    bool *label2;
    Adouble *delta1;
    Adouble *delta2;
    Adouble *pr_value;
    uint *value;

    Graph(string filename, bool _isWeighted);

    void AssignW8(uint w8, uint index);

};
#endif //GRAPH_ALGORITHM_GRAPH_H
