//
// Created by moyu on 2022/9/14.
//

#ifndef GRAPH_ALGORITHM_KERNELS_ASYNC_H
#define GRAPH_ALGORITHM_KERNELS_ASYNC_H

#include "global.h"

template<class E>
void __attribute__((linx_kernel, noinline))
pagerank_async(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, Adouble *value, Adouble *delta){

    bool finished = false;

    while (!finished){
        finished = true;

        for(uint id = 0; id < num_nodes; id ++){
            if(delta[id] > threshold){
                value[id] = value[id] + delta[id];

                if(outDegree[id] > 0){
                    double finalDelta = delta[id] * damping_factor / outDegree[id];
                    uint nbegin = offset[id];
                    uint nend = nbegin + outDegree[id];

                    for(uint j = nbegin; j < nend; j ++){
                        uint dest = edgeList[j].end;
                        delta[dest] = delta[dest] + finalDelta;
                    }
                    finished = false;
                }
                delta[id] = 0;
            }
        }
        (*iter) ++;
    }
}

template<class E>
void __attribute__((linx_kernel, noinline))
bfs_async(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active){

    bool finished = false;

    while (!finished){
        finished = true;

        for(uint id = 0; id < num_nodes; id ++){
            if(active[id]){
                active[id] = false;
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];
                uint finalDist = value[id] + 1;

                for(uint j = nbegin; j < nend; j ++){
                    uint dest = edgeList[j].end;
                    if(finalDist < value[dest]){
                        value[dest] = finalDist;
                        finished = false;
                        active[dest] = true;
                    }
                }
            }
        }
        (*iter) ++;
    }
}

template<class E>
void __attribute__((linx_kernel, noinline))
sssp_async(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active){

    bool finished = false;

    while (!finished){
        finished = true;

        for(uint id = 0; id < num_nodes; id ++){
            if(active[id]){
                active[id] = false;
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];

                for(uint j = nbegin; j < nend; j ++){
                    uint finalDist = value[id] + edgeList[j].w8;
                    if(finalDist < value[edgeList[j].end]){
                        value[edgeList[j].end] = finalDist;
                        finished = false;
                        active[edgeList[j].end] = true;
                    }
                }
            }
        }
        (*iter) ++;
    }
}

template<class E>
void __attribute__((linx_kernel, noinline))
cc_async(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active){

    bool finished = false;

    while (!finished){
        finished = true;

        for(uint id = 0; id < num_nodes; id ++){
            if(active[id]){
                active[id] = false;
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];

                for(uint j = nbegin; j < nend; j ++){
                    uint dest = edgeList[j].end;
                    if(value[id] < value[dest]){
                        value[dest] = value[id];
                        finished = false;
                        active[dest] = true;
                    }
                }
            }
        }
        (*iter) ++;
    }
}

#endif //GRAPH_ALGORITHM_KERNELS_ASYNC_H
