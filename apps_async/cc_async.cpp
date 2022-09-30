//
// Created by moyu on 2022/9/14.
//
#include "../Graph.h"
#include "../Graph.cpp"
#if defined(OPENMP)
#include "../kernels_async_parallel.h"
#else
//#include "../kernels_async.h"
//#include "../kernels_async_dev.h"
#include "../kernels_async_bdfs.h"
#endif
#include "../timer.h"

int main(int argc, char ** argv){
    string filename(argv[1]);
    Graph<OutEdge> graph(filename, false);

    for(unsigned int i = 0; i < graph.num_nodes; i ++){
        graph.value[i] = i;
        graph.label1[i] = true;
    }

    uint itr = 0;   //迭代次数
    Timer timer;
    timer.Start();

    cc_async(graph.num_nodes,
             &itr,
             graph.offset,
             graph.edgeList,
             graph.outDegree,
             graph.value,
             graph.label1);

    cout << "iteration times: " << itr << endl;
    float runtime = timer.Finish();
    cout << "Processing finished in " << runtime/1000 << " (s).\n";

    for(int i = 0; i < 100; i ++){
        cout << graph.value[i] << " ";
    }
    cout <<endl;
    return 0;
}