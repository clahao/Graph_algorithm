//
// Created by 58253 on 2022/9/18.
//

#ifndef GRAPH_ALGORITHM_KERNELS_ASYNC_BDFS_H
#define GRAPH_ALGORITHM_KERNELS_ASYNC_BDFS_H

#include "global.h"

int update = 0;

template<class E>
void rebfs(int id, int depth, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, bool *finished) {
    if (depth == MAX_BDFS_DEPTH)
        return;
    active[id] = false;
    uint nbegin = offset[id];
    uint nend = nbegin + outDegree[id];
    uint finalDist = value[id] + 1;

    for(uint j = nbegin; j < nend; j ++){
        uint dest = edgeList[j].end;
        if(finalDist < value[dest]){
            update++;
            value[dest] = finalDist;
            *finished = false;
            active[dest] = true;
        }
        if (active[dest])
            rebfs(dest, depth + 1, offset, edgeList, outDegree, value, active, finished);
    }
}

template<class E>
void resssp(int id, int depth, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, bool *finished) {
    if (depth == MAX_BDFS_DEPTH)
        return;
    active[id] = false;
    uint nbegin = offset[id];
    uint nend = nbegin + outDegree[id];

    for(uint j = nbegin; j < nend; j ++){
        uint dest = edgeList[j].end;
        uint finalDist = value[id] + edgeList[j].w8;
        if(finalDist < value[dest]){
            update++;
            value[dest] = finalDist;
            *finished = false;
            active[dest] = true;
        }
        if (active[dest])
            rebfs(dest, depth + 1, offset, edgeList, outDegree, value, active, finished);
    }
}

template<class E>
void recc(int id, int depth, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, bool *finished) {
    if (depth == MAX_BDFS_DEPTH)
        return;
    active[id] = false;
    uint nbegin = offset[id];
    uint nend = nbegin + outDegree[id];

    for(uint j = nbegin; j < nend; j ++){
        uint dest = edgeList[j].end;
        if(value[id] < value[dest]){
            update++;
            value[dest] = value[id];
            *finished = false;
            active[dest] = true;
        }
        if (active[dest])
            rebfs(dest, depth + 1, offset, edgeList, outDegree, value, active, finished);
    }
}

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

    stack<pair<uint, uint>> bdfs_stack;
    int size = 0;
    bool finished = false;
    while (!finished){
        finished = true;

        for(uint id = 0; id < num_nodes; id ++){
            if(active[id]){
                rebfs(id, 0, offset, edgeList, outDegree, value, active, &finished);
//                active[id] = false;
//
//                bdfs_stack.push(make_pair(id, 0));
//                size = 1;
//                while (size != 0) {
//                    uint top_id = bdfs_stack.top().first;
//                    uint top_offset = bdfs_stack.top().second;
//                    bool pushed = false;
//                    uint finalDist = value[top_id] + 1;
//                    for (uint j = top_offset; j < outDegree[top_id]; j++) {
//                        uint dest = edgeList[offset[top_id] + j].end;
//                        if (finalDist < value[dest]) {
//                            update++;
//                            value[dest] = finalDist;
//                            finished = false;
//                            active[dest] = true;
//                        }
//                        if (active[dest] && size < MAX_BDFS_DEPTH) {
//                            active[dest] = false;
//                            bdfs_stack.top().second = j + 1;
//                            bdfs_stack.push(make_pair(dest, 0));
//                            size++;
//                            pushed = true;
//                            break;
//                        }
//                    }
//                    if (!pushed) {
//                        bdfs_stack.pop();
//                        size--;
//                    }
//                }
            }
        }
        (*iter) ++;
    }
    cout << update << endl;
}

template<class E>
void __attribute__((linx_kernel, noinline))
sssp_async(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active){

    bool finished = false;

    while (!finished){
        finished = true;

        for(uint id = 0; id < num_nodes; id ++){
            if(active[id]){
                resssp(id, 0, offset, edgeList, outDegree, value, active, &finished);
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
                recc(id, 0, offset, edgeList, outDegree, value, active, &finished);
            }
        }
        (*iter) ++;
    }
    cout << update << endl;
}
#endif //GRAPH_ALGORITHM_KERNELS_ASYNC_BDFS_H
