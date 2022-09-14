//
// Created by moyu on 2022/9/14.
//

#ifndef GRAPH_ALGORITHM_KERNELS_SYNC_PARALLEL_H
#define GRAPH_ALGORITHM_KERNELS_SYNC_PARALLEL_H

#include "global.h"
#include "parallel.h"

template<class E>
void __attribute__((linx_kernel, noinline))
pagerank_sync(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, Adouble *value, Adouble *delta1, Adouble *delta2){

    printf("parallel\n");
    bool finished = false;
    Adouble *delta;
    Adouble *nextDelta;

    while (!finished){
        finished = true;
        delta = (*iter % 2 == 0) ? delta1 : delta2;
        nextDelta = (*iter % 2 == 1) ? delta1 : delta2;

        parallel_for(uint id = 0; id < num_nodes; id ++){
            if(delta[id] > threshold){
                value[id] = value[id] + delta[id];

                if(outDegree[id] > 0){
                    double finalDelta = delta[id] * damping_factor / outDegree[id];
                    uint nbegin = offset[id];
                    uint nend = nbegin + outDegree[id];

                    for(uint j = nbegin; j < nend; j ++){
                        uint dest = edgeList[j].end;
                        nextDelta[dest] = nextDelta[dest] + finalDelta;
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
bfs_sync(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *label1, bool *label2){

    printf("parallel\n");
    bool finished = false;
    bool *active;
    bool *nextActive;

    while (!finished){
        finished = true;
        active = (*iter % 2 == 0) ? label1 : label2;
        nextActive = (*iter % 2 == 1) ? label1 : label2;

        parallel_for(uint id = 0; id < num_nodes; id ++){
            if(active[id]){
                active[id] = false;
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];
                uint finalDist = value[id] + 1;

                for(uint j = nbegin; j < nend; j ++){
                    uint dest = edgeList[j].end;
                    if(finalDist < value[dest]){
                        uint oldV;
                        do{
                            oldV = value[dest];
                        }while(!CAS(&value[dest], oldV, finalDist));
                        finished = false;
                        nextActive[dest] = true;
                    }
                }
            }
        }
        (*iter) ++;
    }
}

template<class E>
void __attribute__((linx_kernel, noinline))
sssp_sync(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *label1, bool *label2){

    printf("parallel\n");
    bool finished = false;
    bool *active;
    bool *nextActive;

    while (!finished){
        finished = true;
        active = (*iter % 2 == 0) ? label1 : label2;
        nextActive = (*iter % 2 == 1) ? label1 : label2;

        parallel_for(uint id = 0; id < num_nodes; id ++){
            if(active[id]){
                active[id] = false;
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];

                for(uint j = nbegin; j < nend; j ++){
                    uint finalDist = value[id] + edgeList[j].w8;
                    if(finalDist < value[edgeList[j].end]){
                        uint oldV;
                        do{
                            oldV = value[edgeList[j].end];
                        }while(!CAS(&value[edgeList[j].end], oldV, finalDist));
                        finished = false;
                        nextActive[edgeList[j].end] = true;
                    }
                }
            }
        }
        (*iter) ++;
    }
}

template<class E>
void __attribute__((linx_kernel, noinline))
cc_sync(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *label1, bool *label2){

    printf("parallel\n");
    bool finished = false;
    bool *active;
    bool *nextActive;

    while (!finished){
        finished = true;
        active = (*iter % 2 == 0) ? label1 : label2;
        nextActive = (*iter % 2 == 1) ? label1 : label2;

        parallel_for(uint id = 0; id < num_nodes; id ++){
            if(active[id]){
                active[id] = false;
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];

                for(uint j = nbegin; j < nend; j ++){
                    uint dest = edgeList[j].end;
                    if(value[id] < value[dest]){
                        uint oldV;
                        do{
                            oldV = value[dest];
                        }while(!CAS(&value[dest], oldV, value[id]));
                        finished = false;
                        nextActive[dest] = true;
                    }
                }
            }
        }
        (*iter) ++;
    }
}
#endif //GRAPH_ALGORITHM_KERNELS_SYNC_PARALLEL_H
