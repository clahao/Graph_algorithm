//
// Created by moyu on 2022/9/14.
//


#include "Graph.h"
#include <string>

template <>
inline void Graph<OutEdgeWeighted>::AssignW8(uint w8, uint index) {
    edgeList[index].w8 = w8;
}

template <>
inline void Graph<OutEdge>::AssignW8(uint w8, uint index) {
    edgeList[index].end = edgeList[index].end;
}


template <class E>
Graph<E>::Graph(string filename, bool _isWeighted) {
    isWeighted = _isWeighted;

    srand((int)time(0));
    ifstream ifs;
    ifs.open(filename);
    string line;
    getline(ifs, line);
    getline(ifs, line);
    getline(ifs, line);
    line = line.substr(9);
    int pos = line.find(' ');
    num_nodes = stoi(line.substr(0, pos));
    pos = line.find_last_of(' ');
    num_edges = stoi(line.substr(pos + 1));
    getline(ifs, line);

    char c = ' ';

    for (int i = 0; i < num_edges; i++) {
        getline(ifs, line);
        pos = line.find(c);
        line = line.substr(pos + 1);
        pos = line.find(' ');
        uint dest = stoi(line.substr(0, pos));
        num_nodes = max(num_nodes, dest + 1);
    }
    printf("%d %d\n", num_nodes, num_edges);
    ifs.seekg(0);
    for(int i = 0; i < 4; i ++)
        getline(ifs, line);

    offset = (uint *)malloc(num_nodes * sizeof(uint));
    edgeList = (E *)malloc(num_edges * sizeof(E));
    outDegree = (uint *)malloc(num_nodes * sizeof(uint));
    label1 = (bool *)malloc(num_nodes * sizeof(bool));
    label2 = (bool *)malloc(num_nodes * sizeof(bool));
    delta1 = (Adouble *)malloc(num_nodes * sizeof(Adouble));
    delta2 = (Adouble *)malloc(num_nodes * sizeof(Adouble));
    pr_value = (Adouble *)malloc(num_nodes * sizeof(Adouble));
    value = (uint *)malloc(num_nodes * sizeof(uint));

    for(int i = 0; i < package_num; i ++)
        queue[i] = (uint *)malloc(num_nodes * sizeof(uint));

    for (int i = 0; i < num_nodes; i++)
        outDegree[i] = 0;

    for (int i = 0; i < num_edges; i++) {
        getline(ifs, line);
        pos = line.find(c);
        uint source = stoi(line.substr(0, pos));
        uint dest = stoi(line.substr(pos + 1));
        outDegree[source] ++;
        edgeList[i].end = dest;
    }
    if(isWeighted)
        for (int i = 0; i < num_edges; i++) {
            getline(ifs, line);
            AssignW8(stoi(line), i);
        }
    uint cnt = 0;
    for (int i = 0; i < num_nodes; i++) {
        offset[i] = cnt;
        cnt += outDegree[i];
    }
    ifs.close();
    printf("read finished\n");
}