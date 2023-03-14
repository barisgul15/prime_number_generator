#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include <stdbool.h>
#include <unistd.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <stddef.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <inttypes.h>

// LOOK UP TABLE
static uint64_t *table;

// HELPER FUNCTIONS
uint64_t approximate(size_t n);  
uint64_t* createTable(size_t n);
size_t sOE(size_t n, uint64_t prims[]);

// CHECKING WHETHER A NUMBER IS PRIME OR NOT
bool checkPrime(uint64_t n);
bool checkPrime_V2(uint64_t n);
bool checkPrime_V3(uint64_t n, uint64_t a);

// PRIME FUNCTIONS THAT CALCULATE FIRST N PRIMES AND WRITES INTO PRIMS ARRAY
size_t prim(size_t n, uint64_t prims[]);
size_t prim_V1(size_t n, uint64_t prims[]);
size_t prim_V2(size_t n, uint64_t prims[]);
size_t prim_V3(size_t n, uint64_t prims[]);
size_t prim_V4(size_t n, uint64_t prims[]);
size_t prim_V5(size_t n, uint64_t prims[]);
size_t prim_V6(size_t n, uint64_t prims[]);
size_t prim_V7(size_t n, uint64_t prims[]);
size_t prim_V8(size_t n, uint64_t prims[]);


void SieveOfAtkin(size_t z, uint64_t prims[],uint64_t limit);

// BENCHMARK & TEST FUNCTIONS
void compareTime(size_t n, uint64_t prims[]);
void compareCorrectness(size_t n, uint64_t prims[]);
double get_time_prim(size_t (*f) (size_t,uint64_t*),size_t size, uint64_t* prims,int repeat);




