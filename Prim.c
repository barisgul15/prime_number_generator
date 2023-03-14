#include "config.h"

// approximation function for nth prime
uint64_t approximate(size_t n) {
 
    // if n is too small, no need to approximate an upper bound
    if(n < 7){
        return 20;
    }
    //     9223372036854775808 := size_t_max 
    //     18446744073709551557:= largest prime number less than 2^64
    //     37022029335820970   := estimation of how many prime numbers there can be before the largest
    //                            prime number less than 2^64 := n_max
    //  estimation comes from  := x ≈ x/ln(x)

    // In order to make the approximation work in every case, we calculate at least until UINT64_MAX if then given n > n_max
    if(n > 37022029335820970){
        return UINT64_MAX - 1;
    }

    // approximation function
    uint64_t res = ceil(n * (log(n) + log(log(n)) - 1 + ((log(log(n))-2)/log(n)) - 
    ((pow(log(log(n)),2) - (6*log(log(n))) + 11)/(2*pow(log(n),2))) + (1/(pow(log(n),2)))));

    // if approximation so close to uint64_max, then we should leave a bit gap between them
    // so there can not be an overflow for the approximation
    if(res > UINT64_MAX - 1000){
        uint64_t temp = res - 1000;
        temp++;
        res -= temp;
    }

    return res + 1000;
}

//  table is created here, but since malloc etc. can not be used to create a global variable, table MUST be
//  initialised before used. Than can be done via createTable() method. Table is initialised via malloc()
//  and can be used afterwards. The size of the table should be determined by the user. There is no need to
//  always create a table consisting all the primes untill 2^64, the table should be created for user's needs.  

// First n prime numbers are found and written on a table(pointer) via Segmented Sieve of Eratosthenes and the table is returned.
uint64_t* createTable(size_t n) {

    table = (uint64_t*)malloc(n * sizeof(uint64_t));
    if (table == NULL) {
        fprintf(stderr,"Memory can not be allocated!\n");
        exit(0);
    }
    // segmented Sieve is used to create the table since segmented Sieve is selected as our main implementation because of memory allocation reasons.
    size_t total = prim(n, table);
    // printing total number of primes makes sense because there is a constant number of primes that can be represented in 64 bit unsigned. If the limit is exceeded
    // user can be aware of that.
    printf("\nThe program was asked to create a table consisting of %zu prime numbers.\n", n);
    printf("Total of %zu prime numbers are in the table.\n", total);
    if (n > total) {
        printf("Total number of primes that can be represented in uint64_t has a limit, therefore table has less elements than desired. The difference is %zu.\n\n", n - total);
    }
    printf("The infos printed untill here are regarding the table creation method. Copying from table to prims[] will start as soon as this message is printed.\n\n");
    return table;

}

// naive prime number checker
bool checkPrime(uint64_t n){

    bool isPrime = true;
    uint64_t limit = sqrt(n);

    // enough to check until the square root of the given number 
    for(uint64_t i = 2 ; i <= limit ; i++){
        // n should only be divided by 1 and itself with rest 0
        if(n % i == 0){
            isPrime = false;
            break;
        }
    }
return isPrime;
}

// 6k±1 theorem, no need to iterate each number
bool checkPrime_V2(uint64_t n){
    
    if (n <= 1){
        return false;
    }

    // multiples of 2 and 3 is not prime
    if (n % 2 == 0 || n % 3 == 0){
        
        // except first two prime numbers, 2 and 3
        if(n == 2 || n == 3){
            return true;
        }
        return false;
    }

    uint64_t k = sqrt(n);
    // 6k±1 theorem, if the number is multiple of 6k±1(a prime number) for a certain k, then it can not be prime
    for (uint64_t i = 5 ; i <= k ; i += 6){
        if (n % i == 0 || n % (i + 2) == 0){
            return false;
        }
    } 
    return true;
}

// Miller-Rabin primality test 
// !! ONLY checkPrime_V3 (miller-rabin test) IS NOT IMPLEMENTED BY US !!
// source-code : https://de.wikipedia.org/wiki/Miller-Rabin-Test
// Used to make comparisons(time&correctness) between other algorithms/versions that are implemented by us
bool checkPrime_V3(uint64_t n, uint64_t a){

    uint64_t m = n - 1;
    uint64_t d = m >> 1, e = 1;
    while(!(d & 1)) {
        d >>= 1;
        ++e;
    }
    uint64_t p = a , q = a;
    while(d >>= 1){
        q *= q;
        q %= n;
        if(d & 1) p *= q, p %= n;
    }

    if(p == 1 || p == m){
        return true;
    }

    while(--e){
        p *= p , p %= n;
        if(p == m) return true;
        if (p <= 1) break;
    }
    return false;
}

void SieveOfAtkin(size_t z, uint64_t prims[z],uint64_t limit){

    // we mark each element false in sieve ptr/array
    bool* sievePTR = (bool*) calloc(limit, sizeof(char));
    if (sievePTR == NULL) {
        fprintf(stderr,"Memory can not be allocated!\n");
        exit(0);
    }
    
    if(z == 1){
        prims[0] = 2;
        return;
    }

    if(z == 2){
        prims[0] = 2;
        prims[1] = 3;
        return;
    }

    // getting square root of the limit 
    double sq_root = sqrt(limit);

    // We mark sieve[n] true if one of the conditions is satisfied 
    for(uint64_t x = 1 ; x <= sq_root ; x++){
        for(uint64_t y = 1 ; y <= sq_root ; y++){

            // Condition 1 
            uint64_t q = (4 * x * x) + (y * y);
            // if q = 4x^2 + y^2 is odd AND modulo-12 remainder is 1 or 5 AND q is within the limit
            if(q <= limit && (q % 12 == 1 || q % 12 == 5)){
                // changing the state(logical not)
                sievePTR[q] = !sievePTR[q];
            }

            // Condition 2 
            q = (3 * x * x) + (y * y);
            // if q = 3x^2 + y^2 is odd AND modulo-12 remainder is 7 AND q is within the limit
            if(q <= limit && q % 12 == 7){
                sievePTR[q] = !sievePTR[q];
            }

            // Condition 3 
            q = (3 * x * x) - (y * y);
            // if q = 3x^2 - y^2 is odd AND x > y AND modulo-12 remainder is 11 AND q is within the limit
            if(x > y && q <= limit && q % 12 == 11){
                sievePTR[q] = !sievePTR[q]; 
            }

        }
    }

    // 3 conditions do not detect multiples of squares, we mark them here as non-prime
    // 2 and 3 already marked, that's why we start with 5
    for(uint64_t k = 5 ; k  <= sq_root ; k++){
        if(sievePTR[k]){
            for(uint64_t i = k * k ; i <= limit ; i += k * k){
                sievePTR[i] = false;
            }
        }
    }

    // Writing all marked primes in the sieve array into prims array 
    size_t count = 2;
    size_t index = 2;
    prims[0] = 2;
    prims[1] = 3;

    for(uint64_t i = 5 ; i <= limit ; i++){
        if(count >= z){
            break;
        }
        if(i == UINT64_MAX){
            break;
        }
        if(sievePTR[i]){
            prims[index++] = i;
            count++;
        }
    }
}

// This is the Sieve of Erast. Algorithm without any approximation. Only calculates the prime numbers until n, does not calculate n prime numbers.
// This is needed for segmented Sieve of Erast. and there implemented. A boolean array is used and SIMD is used to fasten this process. 
size_t sOE(size_t n, uint64_t prims[n]) {

    bool* boolArr = (bool*)malloc((n+1) * sizeof(bool));
    if(boolArr == NULL){
        fprintf(stderr,"Memory can not be allocated!\n");
        exit(0);
    }

    // Helper will be copied to boolArr to initiliase its boolean values. All of the even indexes are marked as false
    // since they are even and not prime, all the odds are marked as true, which will be fixed via the method.

    bool* helper = (bool*) malloc(16 * sizeof(bool));
    if(helper == NULL){
        fprintf(stderr,"Memory can not be allocated!\n");
        exit(0);
    }

    *(helper+0)  = false;
    *(helper+1)  = true;
    *(helper+2)  = false;
    *(helper+3)  = true;
    *(helper+4)  = false;
    *(helper+5)  = true;
    *(helper+6)  = false;
    *(helper+7)  = true;
    *(helper+8)  = false;
    *(helper+9)  = true;
    *(helper+10) = false;
    *(helper+11) = true;
    *(helper+12) = false;
    *(helper+13) = true;
    *(helper+14) = false;
    *(helper+15) = true;

    const __m128i* helper_vec  = (__m128i*)helper; // pointer to the memory location of the source array
    __m128i* boolArr_vec = (__m128i*)boolArr; // pointer to the memory location of the destination array

    // try to select all even numbers as not-prime except 2 by SIMD.
    for (size_t i = 0; i < n / 16; i+=1) {
        boolArr_vec[i] = _mm_loadu_si128(helper_vec);
    }
    // % is done outside the loop because in worst case there needs to be more than 1 modula op. to be made, but with that
    // it is sure that modulo will be used only for 1 time in the worst case. The compiler will probably optimise this by itself
    // but better safe than sorry. 
    size_t a = n - (n % 16);
    while (a <= n) {
        if (a % 2 == 0) {
            boolArr[a] = false;
        } 
        else {
            boolArr[a] = true;
        }
        a++;
    }

    // 2 is prime.
    boolArr[2] = true;

    // main loop for only odd numbers, because even numbers are eliminated.
    for (size_t i = 3; i * i <= n; i+=2) { 
        if (boolArr[i] == true) {
            for (size_t j = i * i; j <= n; j += i) {
                boolArr[j] = false;
            } 
        }
    }

    size_t index = 0;
    for (size_t i = 2; i <= n; i++) {
        if (boolArr[i] == true) {
            prims[index] = i;
            index++;
        }
    }
    return index;
}



// Segmented Sieve of Eratosthenes optimized accordingly to cache size
size_t prim(size_t n, uint64_t prims[n]) {
    
    if(n == 0){
        return 0;
    }
    
    // Tests are done on our own computers and below the size of the L1-, L2- and L3-Cache sizes are given.
    // L1 size =   32768 
    // L2 size =  262144
    // L3 size = 3145728
    // however since Rechnerhalle is the main platform, segment size is changed accordingly to L1-Cache size of Rechnerhalle.
    // l1 cache of rechnerhalle := 655360 
    // 5898239; // l1 + l2 - 1 of rechnerhalle 
    
    // l1 - 1 is selected because between all positive number a,b and a < b, there are (b-a) + 1 numbers to be checked in the Interval [a, b], therefore
    // we add the +1 while allocating the memory.
    uint64_t segmentSize = 655359; 

    // If numberOfNumbersToBeChecked are <= segment size, there is no need of segmentation.
    size_t numberOfNumbersToBeChecked = approximate(n);
    if (numberOfNumbersToBeChecked <= segmentSize) {
        segmentSize = n;
        return prim_V5(segmentSize, prims);
    }

    // firstArr is the segment which will help to find other primes and first segment sieved by the sOE, Sieve of Erat. but without any
    // approximation function, it calcualtes all the primes until the parameter-given size_t n.
    size_t firstArrSize = sOE(segmentSize, prims);
    size_t index = firstArrSize;

    // Segmentation starts here.
    // low is the smallest number to be checked in the segment.
    // high is the highest number to be checked in the segment;
    uint64_t low = segmentSize;
    uint64_t high = 2 * segmentSize;
    // segment is allocated here and always used the same memory for each segment, since at the end of sieving, primes found are saved in prims[].
    bool *arr = (bool*)malloc(segmentSize+1);
    if (arr == NULL) {
        fprintf(stderr,"Memory can not be allocated!\n");
        exit(0);
    }
   
    while (true) {
        memset(arr, true, segmentSize+1); 
        for (size_t i = 0; i < firstArrSize; i++) {
            // startValues are calculated through the first segment that is sieved. Via that segment, multiplicatives of these primes are found and eliminated.
            uint64_t startValue = floor(low/prims[i]) * prims[i];
            if (startValue < low)
                startValue += prims[i];
            for (size_t j=startValue; j<=high; j+=prims[i]) {
                arr[j-low] = false;
            }
        }
        // found primes in segment are saved in prims[].
        for (size_t i = low; i<=high; i++) {
            if (arr[i - low] == true) {
                prims[index] = i;
                index++;
                if (index == n) {
                    break;
                }
            }
        }
        if (index == n) {
            return index;  
        }
        // increment the low and high to search for the next segment.
        // segmentSize +1 because for example if the segment size was 10, and we start with the interval [10-20] then by incrementing 1
        // the segmentation goes on for [21-31]. Total of numbers checked in interval stays the same but there is no need to check 20 twice in our example.
        // if 20 was a prime number it would have been checked twice and can be written two times in the table. Therefore also incrementing by 1 makes sense.
        low += segmentSize +1;
        high+= segmentSize +1;
        // If the high overflows and 64 bit limit is exceeded, then the last segment to be sieved has come. The sieving of the last segment are handled below.
        // It needs to be written as a different case because altough it seems very similar, control for overflows has to be done. 
        // !!! 
        // The overflow controls are done only in the last segment because in our tests our segment size was 294911 byte for which it is enough to control
        // the last segment. However the segment size of the Rechnerhalle is too big but still we did not think that it will be necessary but since we can not test
        // these numbers because computation time takes weeks, we could not have the chance to test our theory. 
        // !!!
        if (high < segmentSize) { // overflow check
            high = UINT64_MAX;
            break;
        }
    }

    // Sieving last segment will be done in a seperate part because there must be control made for index 
    // if it overflows and exceeds the limits of uint64_t

    uint64_t tmp;

    memset(arr, true, segmentSize+1); 
    for (size_t i = 0; i < firstArrSize; i++) {
        uint64_t startValue = floor(low/prims[i]) * prims[i];
        // startValue calculation will not cause overflow becase the biggest prime number that have been accessed will be from the first sieved segment, therefore
        // the computation will not result in overflow.
        if (startValue < low) {
            tmp = startValue;
            tmp += prims[i];
            // First overflow check. Only increase if incrementing does not result in overflow.
            if (tmp >= startValue) {
                startValue += prims[i];
            }
        }
        for (uint64_t j=startValue; j<=high; j+=prims[i]) {
            // if adding prims[i] cause overflow, break the loop because all the other incrementations will also result in overflow
            // and continue with next element. 
            if (j < startValue)
                break;
            arr[j-low] = false;
        }
    }
    for (uint64_t i = low; i<=high; i++) {
        if (arr[i - low] == true) {
            prims[index] = i;
            index++;
            if (index == n) {
                break;
            }
        }
    }
    return index;
    // Sieving last segment ends here
}

// basic with brute force&trial division prime checker
size_t prim_V1(size_t n, uint64_t prims[n]){

    if(n == 0){
        return 0;
    }
    else{
        prims[0] = 2;
    
        if(n == 1) { return 1;}

        else if(n == 2) {prims[1] = 3;}

        // n > 2
        else{
            prims[1] = 3;
            uint64_t num = 5;
            int index = 2;
            size_t count = 2;

            while(count < n){

                if(num == UINT64_MAX){
                    return n;
                }
                if(checkPrime(num)){
                    count++;
                    prims[index++] = num;
                }
                num++;
            }
        }
    }
    return n;
}

// basic with '6k±1' prime checker(v2)
size_t prim_V2(size_t n, uint64_t prims[n]){

    if(n == 0){
        return 0;
    }
    else{
        prims[0] = 2;
    
        if(n == 1) { return 1;}

        else if(n == 2) {prims[1] = 3;}

        // n > 2
        else{
            prims[1] = 3;
            uint64_t num = 5;
            int index = 2;
            size_t count = 2;
           
            while(count < n){

                if(num == UINT64_MAX){
                    return n;
                }

                if(checkPrime_V2(num)){
                    count++;
                    prims[index++] = num;
                }
                num++;
            }
        }
    }
    return n;
}

// basic with mrp test
size_t prim_V3(size_t n, uint64_t prims[n]){

    if(n == 0){
        return 0;
    }
    else{
        prims[0] = 2;
    
        if(n == 1) { return 1;}

        else if(n == 2) {prims[1] = 3;}

        // n > 2
        else{
            prims[1] = 3;
            uint64_t num = 5;
            int index = 2;
            size_t count = 2;

            // selecting a respectively
            // source : https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test
            // prim <= 2047
            if(n < 310){
                 while(count < n){

                    if(num == UINT64_MAX){
                        return n;
                    }

                    if(checkPrime_V3(num,2)){
                        count++;
                        prims[index++] = num;
                    }
                num++;
                }
            }
            // prim <= 1373653
            else if(n < 105223){
                while(count < n){
                    if(num == UINT64_MAX){
                        return n;
                    }
                    if(checkPrime_V3(num,2) && checkPrime_V3(num,3)){
                        count++;
                        prims[index++] = num;
                    }
                num++;
                }

            }// prim <= 9080191
            else if (n < 607417){
                while(count < n){
                    if(num == UINT64_MAX){
                        return n;
                    }
                    if(checkPrime_V3(num,31) && checkPrime_V3(num,73)){
                        count++;
                        prims[index++] = num;
                    }
                num++;
                }
            }// prim <= 25326001
            else if (n < 1585371){
                while(count < n){
                    if(num == UINT64_MAX){
                        return n;
                    }
                    if(checkPrime_V3(num,2) && checkPrime_V3(num,3) && checkPrime_V3(num,5)){
                        count++;
                        prims[index++] = num;
                    }
                num++;
                }

            }else if (approximate(n) - 10000 < 3215031751){
                while(count < n){
                    if(num == UINT64_MAX){
                        return n;
                    }
                    if(checkPrime_V3(num,2) && checkPrime_V3(num,3)
                     && checkPrime_V3(num,5) && checkPrime_V3(num,7)){
                        count++;
                        prims[index++] = num;
                    }
                num++;
                }

            }else if (approximate(n) - 10000<  4759123141){
                while(count < n){
                    if(num == UINT64_MAX){
                        return n;
                    }
                    if(checkPrime_V3(num,2) && checkPrime_V3(num,7)
                     && checkPrime_V3(num,61)){
                        count++;
                        prims[index++] = num;
                    }
                num++;
                }  
            }else if (approximate(n) - 10000 < 1122004669633){
                while(count < n){
                        if(num == UINT64_MAX){
                        return n;
                    }
                    if(checkPrime_V3(num,2) && checkPrime_V3(num,13)
                     && checkPrime_V3(num,23) && checkPrime_V3(num,1662803)){
                        count++;
                        prims[index++] = num;
                    }
                num++;
                }

            }else if (approximate(n) - 10000 < 2152302898747){
                 while(count < n){
                        if(num == UINT64_MAX){
                        return n;
                    }
                    if(checkPrime_V3(num,2) && checkPrime_V3(num,3)
                     && checkPrime_V3(num,5) && checkPrime_V3(num,7)
                     && checkPrime_V3(num,11)){
                        count++;
                        prims[index++] = num;
                    }
                num++;
                }
            }else if(approximate(n) - 10000 < 3474749660383){
                while(count < n){
                    if(num == UINT64_MAX){
                        return n;
                    }
                    if(checkPrime_V3(num,2) && checkPrime_V3(num,3)
                     && checkPrime_V3(num,5) && checkPrime_V3(num,7)
                     && checkPrime_V3(num,11) && checkPrime_V3(num,13)){
                        count++;
                        prims[index++] = num;
                    }
                num++;
                }

            }else if(approximate(n) - 10000 < 341550071728321){
                while(count < n){
                    if(num == UINT64_MAX){
                        return n;
                    }
                    if(checkPrime_V3(num,2) && checkPrime_V3(num,3)
                     && checkPrime_V3(num,5) && checkPrime_V3(num,7)
                     && checkPrime_V3(num,11) && checkPrime_V3(num,13)
                     && checkPrime_V3(num,17)){
                        count++;
                        prims[index++] = num;
                    }
                num++;
                }

            }else if(approximate(n) - 10000 < 3825123056546413051){
                while(count < n){
                    if(num == UINT64_MAX){
                        return n;
                    }
                    if(checkPrime_V3(num,2) && checkPrime_V3(num,3)
                     && checkPrime_V3(num,5) && checkPrime_V3(num,7)
                     && checkPrime_V3(num,11) && checkPrime_V3(num,13)
                     && checkPrime_V3(num,17)  && checkPrime_V3(num,19)
                     && checkPrime_V3(num,23)){
                        count++;
                        prims[index++] = num;
                    }
                num++;
                }

            }else if (approximate(n) - 10000 < UINT64_MAX){
                 while(count < n){
                    if(num == UINT64_MAX){
                        return n;
                    }
                    if(checkPrime_V3(num,2) && checkPrime_V3(num,3)
                     && checkPrime_V3(num,5) && checkPrime_V3(num,7)
                     && checkPrime_V3(num,11) && checkPrime_V3(num,13)
                     && checkPrime_V3(num,17)  && checkPrime_V3(num,19)
                     && checkPrime_V3(num,23) && checkPrime_V3(num,29)
                    && checkPrime_V3(num,31) && checkPrime_V3(num,37)){
                        count++;
                        prims[index++] = num;
                    }
                num++;
                }
            }
        }
    }
    return n;
}

// Sieve of Eratosthenes with calloc optimization + checking evens only by using 2 + approximation used.
size_t prim_V4(size_t n, uint64_t prims[n]) {

    if(n == 0){
        return 0;
    }

    // until that number, every number must be checked. 
    uint64_t untill = approximate(n);

    // we use calloc instead of malloc + memset because of performance reasons. Therefore false means number is prime
    // and true means number is not prime.
    bool* boolArr = (bool*)calloc((untill+1),sizeof(bool));

    if(boolArr == NULL){
        fprintf(stderr,"Memory can not be allocated!\n");
        exit(0);
    }

    // start the loop with 2, but loop with 2 is not included with the main loop, because
    // all the even numbers are eliminated by that, so there is no need to look for 4, 6, 8, 10, etc.
    // after that main loop for the elimination of non-prime numbers will start with 3 and always
    // will be incremented by 2 to avoid all of the even numbers.
    size_t i = 2;
    if (boolArr[i] == false) {
    for (size_t j = i * i; j <= untill; j += i) {
        boolArr[j] = true;
        } 
    }

    // main loop for only odd numbers, because even numbers are eliminated.
    for (size_t i = 3; i * i <= untill; i+=2) { 
        if (boolArr[i] == false) {
            for (size_t j = i * i; j <= untill; j += i) {
                boolArr[j] = true;
            } 
        }
    }

    
    size_t index = 0; // create an index to iterate over prims[];
    // correctly place the prime numbers to the prims[]
    // start from the second index because 0 and 1 are not prime so there is no need to check these indexes. 
    for (uint64_t i = 2; i <= untill; i++) {
        if (boolArr[i] == false) {
            prims[index] = i;
            if (index == n) {
                break;
            }
            index++;
        }
    }
    
    return index;
}

// Sieve of Eratosthenes with SIMD + approx
// almost the same algorithm, but the difference is here malloc is used,
// because with SIMD instructions, the boolean pointer created is initialised.
size_t prim_V5(size_t n, uint64_t prims[n]) {

    uint64_t untill = approximate(n);

    bool* boolArr = (bool*)malloc((untill+1) * sizeof(bool));

    if(boolArr == NULL){
        fprintf(stderr,"Memory can not be allocated!\n");
        exit(0);
    }

    // Helper will be copied to boolArr to initialise its boolean values. All of the even indexes are marked as false
    // since they are even and not prime, all the odds are marked as true, which will be inside this method.
    bool* helper = (bool*) malloc(16 * sizeof(bool));
    if(helper == NULL){
        fprintf(stderr,"Memory can not be allocated!\n");
        exit(0);
    }
    *(helper+0)  = false;
    *(helper+1)  = true;
    *(helper+2)  = false;
    *(helper+3)  = true;
    *(helper+4)  = false;
    *(helper+5)  = true;
    *(helper+6)  = false;
    *(helper+7)  = true;
    *(helper+8)  = false;
    *(helper+9)  = true;
    *(helper+10) = false;
    *(helper+11) = true;
    *(helper+12) = false;
    *(helper+13) = true;
    *(helper+14) = false;
    *(helper+15) = true;

    const __m128i* helper_vec  = (__m128i*)helper; // pointer to the memory location of the source array
    __m128i* boolArr_vec = (__m128i*)boolArr; // pointer to the memory location of the destination array

    // try to select all even numbers as not-prim except 2 by SIMD.
    for (size_t i = 0; i < untill / 16; i+=1) {
        boolArr_vec[i] = _mm_loadu_si128(helper_vec);
    }
    
    size_t i = untill - (untill % 16);
    while(i <= untill) {
        if (i % 2 == 0) {
            boolArr[i] = false;
        } else {
            boolArr[i] = true;
        }
        i++;
    }
    
    
    // 2 is prime, so these are fixed. 0 and 1 are not fixed because iteration will start with index 2 of the boolArr.
    boolArr[2] = true;

    // main loop for only odd numbers, because even numbers are eliminated.
    for (size_t i = 3; i * i <= untill; i+=2) { 
        if (boolArr[i] == true) {
            for (size_t j = i * i; j < untill; j += i) {
                boolArr[j] = false;
            } 
        }
    }

    size_t index = 0; // create an index to iterate over prims[];

    //correctly place the prime numbers to the prims[] 
    for (size_t i = 2; i <= untill; i++) {
        if (boolArr[i] == true) {
            prims[index++] = i;
        }
        if (index == n) {
            return index;
        }
    }
    
    return index;
}

// sieve of atkin 
size_t prim_V6(size_t n, uint64_t prims[n]){

    if(n == 0){
        return 0;
    }
    else{
        SieveOfAtkin(n,prims,approximate(n));
    }
    return n;
}


// a table which can have the size of total prime numbers from 2 to 2^64 must be created before using this method.
// this method uses the primeNumbersTable and SISD instructions to load uint64_t prime numbers to the prims[].
size_t prim_V7(size_t n, uint64_t prims[n]) {

    if(n == 0){
        return 0;
    }

    for (size_t i = 0; i < n; i++) {
        prims[i] = table[i];
    }
    return n;
}

// a table which can have the size of total prime numbers from 2 to 2^64 must be created before using this method.
// uses the table to find first N prime numbers, while using SIMD instructions for optimization.
size_t prim_V8(size_t n, uint64_t prims[n]) {
    
    if(n == 0){
        return 0;
    }
    
    // n can be changed during the check of primality, so we introduce a new variable named size
    // so we can modify size easily
    size_t size = n;

    // checking if the size is odd or even
    // if odd, we decrement the size by 1 to make size even, so SIMD instructions work well
    if(size % 2 != 0){
        prims[size - 1] = table[size - 1];
        size--;
    }
    
    __m128i* table_vec = (__m128i*)table; // pointer to the memory location of the source array
    __m128i* prims_vec = (__m128i*)prims; // pointer to the memory location of the destination array
    
    for(size_t i = 0 ; i < size/2 ; i++){
        prims_vec[i] = _mm_loadu_si128(&table_vec[i]);
    }
    
    return n;
}




