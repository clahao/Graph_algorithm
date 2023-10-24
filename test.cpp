//
// Created by moyu on 2022/9/27.
//
#include "Graph.h"
#include "Graph.cpp"
int main(int argc, char ** argv) {
    string filename(argv[1]);
    Graph <OutEdgeWeighted> graph(filename, true);
    for(int i = 0; i < graph.num_nodes; i ++)
        cout << graph._offset[i] << " ";
    cout << endl;
    for(int i = 0; i < graph.num_nodes; i ++){
        for(int j = graph._offset[i]; j < graph._offset[i] + graph.inDegree[i]; j ++){
            cout << graph._edgeList[j].end << " " << graph._edgeList[j].w8 << endl;
        }
    }
    return 0;

}