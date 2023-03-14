#include "config.h"

double get_time_prim(size_t (*f) (size_t,uint64_t*),size_t size, uint64_t* prims, int repeat){

    size_t res;
    double time = 0; 
    struct timespec start;      
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i = 0 ; i < repeat ; i++){
        res = f(size,prims);
    }
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    time += end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
    return time;
    
}
