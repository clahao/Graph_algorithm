//
// Created by moyu on 2022/9/16.
//

#ifndef GRAPH_ALGORITHM_KERNELS_ASYNC_PRIORITY_SCHEDULE_H
#define GRAPH_ALGORITHM_KERNELS_ASYNC_PRIORITY_SCHEDULE_H

#include <queue>
#include "global.h"
#include "timer.h"
//#include "Index_Min_PriorityQueue.h"
//#include "Index_Min_PriorityQueue.cpp"

//template<class E>
//void __attribute__((linx_kernel, noinline))
//bfs_async(uint *offset, E *edgeList, uint *outDegree, uint *value, IndexMinPriorityQueue<uint> queue){
//    Timer time;
//    float run_time = 0;
//    time.Start();
//    while (!queue.isEmpty()){
//        int id = queue.minIndex();
//        uint Dist = queue.items[id];
//        uint finalDist = queue.items[id] + 1;
//        //time.Start();
//        queue.delMin();
//        //run_time += time.Finish();
//        if(Dist < value[id]){
//            value[id] = Dist;
//            uint nbegin = offset[id];
//            uint nend = nbegin + outDegree[id];
//
//            for(uint j = nbegin; j < nend; j ++){
//                uint dest = edgeList[j].end;
//                //cout<< finalDist <<endl;
//                //time.Start();
//                if(finalDist < value[dest]){
//                    if(!queue.contains(dest))
//                        queue.insert(dest,finalDist);
//                    else if(finalDist < queue.items[dest])
//                        queue.changeItem(dest, finalDist);
//                }
//                //run_time += time.Finish();
//            }
//        }
//
//    }
//    run_time = time.Finish();
//    cout << "Processing stack finished in " << run_time/1000 << " (s).\n";
//}

//template<class E>
//void __attribute__((linx_kernel, noinline))
//bfs_async(uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, queue<int> queue){
//
//    while (!queue.empty()){
//        uint id = queue.front();
//        queue.pop();
//        active[id] = false;
//        uint nbegin = offset[id];
//        uint nend = nbegin + outDegree[id];
//        uint finalDist = value[id] + 1;
//
//        for(uint j = nbegin; j < nend; j ++){
//            uint dest = edgeList[j].end;
//            if(finalDist < value[dest]){
//                value[dest] = finalDist;
//                if(active[dest] == false){
//                    active[dest] = true;
//                    queue.push(dest);
//                }
//            }
//        }
//    }
//}
int update = 0;
uint priority = 0;
uint min_prior;

template<class E>
void rebfs(int id, int depth, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, bool *finished, queue<uint> *queue) {
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
            active[dest] = true;

            if(depth == MAX_BDFS_DEPTH - 1){
                *finished = false;
                uint prior = finalDist >> package_interval;
                prior = prior < package_num ? prior : package_num - 1;
                min_prior = min_prior < prior ? min_prior : prior;//min(min_prior,prior);
                queue[prior].push(dest);
            }

        }
        if (active[dest])
            rebfs(dest, depth + 1, offset, edgeList, outDegree, value, active, finished, queue);
    }
}

template<class E>
void __attribute__((linx_kernel, noinline))
bfs_async(uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, queue<uint> *queue){
    bool finished = false;


    while (!finished || priority < package_num){
        finished = true;
        uint id = queue[priority].front();
        queue[priority].pop();

        if(active[id]){
            min_prior = priority;
            //value[id] = Dist;
            rebfs(id, 0, offset, edgeList, outDegree, value, active, &finished, queue);

            priority = min_prior;
        }

        while(priority < package_num && queue[priority].empty())
            priority++;
    }
    cout<<"update times: "<<update<<endl;
}

//template<class E>
//void __attribute__((linx_kernel, noinline))
//bfs_async(uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, queue<uint> *queue){
//    bool finished = false;
//    uint priority = 0;
//    uint min_prior;
//    uint update_times = 0;
//
//    while (!finished || priority < package_num){
//        finished = true;
//        uint id = queue[priority].front();
//        queue[priority].pop();
//
//        if(active[id]){
//            active[id] = false;
//            uint Dist = value[id];
//            min_prior = priority;
//            //value[id] = Dist;
//            uint nbegin = offset[id];
//            uint nend = nbegin + outDegree[id];
//            uint finalDist = Dist + 1;
//
//            for(uint j = nbegin; j < nend; j ++){
//                uint dest = edgeList[j].end;
//                if(finalDist < value[dest]){
//                    update_times ++;
//                    active[dest] = true;
//                    finished = false;
//                    value[dest] = finalDist;
//                    uint prior = finalDist >> package_interval;
//                    prior = prior < package_num ? prior : package_num - 1;
//                    min_prior = min_prior < prior ? min_prior : prior;//min(min_prior,prior);
//                    queue[prior].push(dest);
//                }
//            }
//            priority = min_prior;
//        }
//
//        while(priority < package_num && queue[priority].empty())
//            priority++;
//    }
//    cout<<"update times: "<<update_times<<endl;
//}


template<class E>
void __attribute__((linx_kernel, noinline))
sssp_async(uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, queue<uint> *queue){
    bool finished = false;
    uint priority = 0;
    uint min_prior;
    uint update_times = 0;

    while (!finished || priority < package_num){
        finished = true;
        uint id = queue[priority].front();
        queue[priority].pop();

        if(active[id]){
            active[id] = false;
            uint Dist = value[id];
            min_prior = priority;
            //value[id] = Dist;
            uint nbegin = offset[id];
            uint nend = nbegin + outDegree[id];

            for(uint j = nbegin; j < nend; j ++){
                uint dest = edgeList[j].end;
                uint finalDist = Dist + edgeList[j].w8;
                if(finalDist < value[dest]){
                    update_times ++;
                    active[dest] = true;
                    finished = false;
                    value[dest] = finalDist;
                    uint prior = finalDist >> package_interval;
                    prior = prior < package_num ? prior : package_num - 1;
                    min_prior = min_prior < prior ? min_prior : prior;//min(min_prior,prior);
                    queue[prior].push(dest);
                }
            }
            priority = min_prior;
        }

        while(priority < package_num && queue[priority].empty())
            priority++;
    }
    cout<<"update times: "<<update_times<<endl;
}


template<class E>
void __attribute__((linx_kernel, noinline))
cc_async(uint num_nodes, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, queue<uint> queue){

    uint update_times = 0;
    for(uint i = 0; i < num_nodes; i ++){

        if(active[i]){
            active[i] = false;
            queue.push(i);

            while(!queue.empty()){
                uint id = queue.front();
                queue.pop();
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];

                for(uint j = nbegin; j < nend; j ++){
                    uint dest = edgeList[j].end;
                    if(value[id] < value[dest]){
                        update_times ++;
                        active[dest] = false;
                        value[dest] = value[id];
                        queue.push(dest);
                    }
                }
            }
        }
    }
    cout<<"update times: "<<update_times<<endl;
}


#endif //GRAPH_ALGORITHM_KERNELS_ASYNC_PRIORITY_SCHEDULE_H
