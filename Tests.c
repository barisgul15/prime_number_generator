#include "config.h"

// n declares how many prime numbers should be written 
void compareTime(size_t n, uint64_t prims[n]){

    printf("\nRunning time complexity tests...\n");
    double time = 0;
    sleep(2);

    time += get_time_prim(prim,n,prims,3);
    printf("\n-> Version 0 calculates first %zu prime numbers in %f seconds.\n",n,(time/3));
    time = 0;
    sleep(2);
    
    time += get_time_prim(prim_V1,n,prims,3);
    printf("-> Version 1 calculates first %zu prime numbers in %f seconds.\n",n,(time/3));
    time = 0;
    sleep(2);
    
    time += get_time_prim(prim_V2,n,prims,3);
    printf("-> Version 2 calculates first %zu prime numbers in %f seconds.\n",n,(time/3));
    time = 0;
    sleep(2);

    time += get_time_prim(prim_V3,n,prims,3);
    printf("-> Version 3 calculates first %zu prime numbers in %f seconds.\n",n,(time/3));
    time = 0;
    sleep(2);


    time += get_time_prim(prim_V4,n,prims,3);
    printf("-> Version 4 calculates first %zu prime numbers in %f seconds.\n",n,(time/3));
    time = 0;
    sleep(2);

    time += get_time_prim(prim_V5,n,prims,3);
    printf("-> Version 5 calculates first %zu prime numbers in %f seconds.\n",n,(time/3));
    time = 0;
    sleep(2);

    time += get_time_prim(prim_V6,n,prims,3);
    printf("-> Version 6 calculates first %zu prime numbers in %f seconds.\n",n,(time/3));
    time = 0;
    sleep(2);

    printf("\nLook Up Table is being created for Version 6 and 7...\n\n");
    table = createTable((size_t)n);
    sleep(2);

    time += get_time_prim(prim_V7,n,prims,3);
    printf("-> Version 7 calculates first %zu prime numbers in %f seconds.\n",n,(time/3));

    time = 0;
    sleep(2);

    time += get_time_prim(prim_V8,n,prims,3);
    printf("-> Version 8 calculates first %zu prime numbers in %f seconds.\n\n",n,(time/3));
   
    free(table);

}

// This function creates a look up table correctly and respectively and compares the results of all algorithms with the look up table
// LUT algorithms are not included in this test, because the comparing element is the LUT itself.
void compareCorrectness(size_t n, uint64_t prims[n]){

    table = createTable(n);
    printf("\nRunning correctness tests...\n\n");
    sleep(2);

    // VERSION 0
    size_t result = prim(n,prims);
    bool res = true;

    for(uint64_t i = 0 ; i < result ; i++){

        if(*(table+i) != prims[i]){
            res = false;
            break;
        }
    }

    if(!res){
        fprintf(stderr,"-> Version 0 failed while calculating the first %zu prime numbers!\n",result);
    }else{
        printf("-> Version 0 successfully calculated the first %zu prime numbers!\n",result);
    }

    sleep(1);

    // VERSION 1
    result = prim_V1(n,prims);
    res = true;

    for(uint64_t i = 0 ; i < result ; i++){

        if(*(table+i) != prims[i]){
            res = false;
            break;
        }
    }

    if(!res){
        fprintf(stderr,"-> Version 1 failed while calculating the first %zu prime numbers!\n",result);
    }else{
        printf("-> Version 1 successfully calculated the first %zu prime numbers!\n",result);
    }

    sleep(1);

    // VERSION 2 
    result = prim_V2(n,prims);
    res = true;

    for(uint64_t i = 0 ; i < result ; i++){

        if(*(table+i) != prims[i]){
            res = false;
            break;
        }
    }

    if(!res){
        fprintf(stderr,"-> Version 2 failed while calculating the first %zu prime numbers!\n",result);
    }else{
        printf("-> Version 2 successfully calculated the first %zu prime numbers!\n",result);
    }

    sleep(1);

    // VERSION 3
    result = prim_V3(n,prims);
    res = true;

    for(uint64_t i = 0 ; i < result ; i++){

        if(*(table+i) != prims[i]){
            res = false;
            break;
        }
    }

    if(!res){
        fprintf(stderr,"-> Version 3 failed while calculating the first %zu prime numbers!\n",result);
    }else{
        printf("-> Version 3 successfully calculated the first %zu prime numbers!\n",result);
    }

    sleep(1);

    // VERSION 4
    result = prim_V4(n,prims);
    res = true;

    for(uint64_t i = 0 ; i < result ; i++){

        if(*(table+i) != prims[i]){
            res = false;
            break;
        }
    }

    if(!res){
        fprintf(stderr,"-> Version 4 failed while calculating the first %zu prime numbers!\n",result);
    }else{
        printf("-> Version 4 successfully calculated the first %zu prime numbers!\n",result);
    }

    sleep(1);

    // VERSION 5
    result = prim_V5(n,prims);
    res = true;

    for(uint64_t i = 0 ; i < result ; i++){

        if(*(table+i) != prims[i]){
            res = false;
            break;
        }
    }

    if(!res){
        fprintf(stderr,"-> Version 5 failed while calculating the first %zu prime numbers!\n",result);
    }else{
        printf("-> Version 5 successfully calculated the first %zu prime numbers!\n",result);
    }

    sleep(1);

    // VERSION 6
    result = prim_V6(n,prims);
    res = true;

    for(uint64_t i = 0 ; i < result ; i++){

        if(*(table+i) != prims[i]){
            res = false;
            break;
        }
    }

    if(!res){
        fprintf(stderr,"-> Version 6 failed while calculating the first %zu prime numbers!\n\n",result);
    }else{
        printf("-> Version 6 successfully calculated the first %zu prime numbers!\n\n",result);
    }

}