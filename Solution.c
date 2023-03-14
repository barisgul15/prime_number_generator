#include "config.h"

// usage messages to give hint how to use/run the program
const char* usage_msg = "\n"
    "Usage: %s --help                      Shows help messages\n"
    "   or: %s -h                          Shows help messages\n"
    "\n";

// help messages to give hint which arguments should be or can be used in the command line
const char* help_msg =
    "Mandatory argument:\n\n"
    "  -n<X>   Number of prime numbers to calculate. "
    "\n\n"
    "Optional arguments:\n\n"
    "  -V<X>    The version of implementation. (Default: X = 0)\n"
    "           Version 0 := Implementation of Segmented Sieve of Eratosthenes Algorithm (Better for large inputs)\n"  
    "           Version 1 := Brute Force with trial division prime checker (Not recommented for large inputs)\n"
    "           Version 2 := Brute Force with 6k±1 Number Theorem prime checker\n"
    "           Version 3 := Brute Force with Miller-Rabin primality test\n"   
    "           Version 4 := Implementation of Sieve of Eratosthenes Algorithm\n"
    "           Version 5 := Implementation of Sieve of Eratosthenes Algorithm via SIMD\n"
    "           Version 6 := Implementation of Sieve of Atkin Algorithm\n"
    "           Version 7 := Implementation via Look Up Table (SISD)\n"
    "           Version 8 := Implementation via Look Up Table (SIMD)\n\n"
    "  -B<X>    Execution time of the implementation is measured and returned.\n"
    "           X denotes the number of times the function is repeated. (Default: X = 1)\n\n"
    "  -T<X>    Execution time of all implementations is measured and compared.\n"
    "           X denotes the number of prime numbers to calculate.\n"
    "           This option must be used alone.\n"
    "           Usage: ./prog_name -T<X>\n\n"
    "  -C<X>    Correctness of all implementations are tested except LUT algorithms.\n"
    "           X denotes the number of prime numbers to calculate.\n"
    "           This option must be used alone.\n"
    "           Usage: ./prog_name -C<X>\n\n"
    "  -p       Prints the first n prime numbers, which are written into prims array respectively.\n"
    "           (Not usable with -T and -B options)\n\n"
    "  -h       A description of all the program options and usage examples are issued.\n\n"
    "  --help   A description of all the program options and usage examples are issued.\n\n";


// outputs the usage and help messages 
void print_usage(const char* prog_name){
    fprintf(stderr,usage_msg,prog_name,prog_name,prog_name,prog_name,prog_name);
}

void print_help(const char* prog_name){
    print_usage(prog_name);
    fprintf(stderr,"%s\n",help_msg);
}

int main(int argc, char** argv){

int opt;                        // storing the result of getopt in each iteration
int version = 0;                // storing version
int repeat = 1;                 // storing repeat time for option -B<repeat>
bool mandatory_given = false;   // checking if the mandatory argument is used in the command line 
bool marker = false;            // checking if the option -B is used 
bool printPrims = false;        // checking if the option -p is used
size_t n;                       // storing the first parameter of function prim
size_t g;                       // storing the first parameter of function prim for the option -T (execution time tests)
size_t g_new;                   // storing the first parameter of function prim for the option -C (correctness tests)
double time = 0;                // storing the time for the option -B
const char* prog_name = argv[0];// storing the program name : ./solution

char str[] = "--help";

    // checking if there is --help in argument line
    for(int i = 0 ; i  < argc ; i++){

        if(strcmp(argv[i],str) == 0){
            goto help;
            break;
        }       
    }

    // Reading the mandatory/optional arguments from command line
    while((opt = getopt(argc,argv,"T:V:B::C:n:hp")) != -1){
    
        switch (opt){
    
        // Decision of which version of the function is to be executed
        case 'V':
            version = atol(optarg);
            if(version < 0 || version > 8){
                fprintf(stderr,"Invalid Argument! There is no such a version!\n");
                return EXIT_FAILURE;
            }
            break;
        
        // Time measurement of the functions, that must be repeated n times
        case 'B':
            if(optarg != NULL){
                repeat = atol(optarg);
                if(repeat < 1){
                    fprintf(stderr,"Invalid Argument! Repetition number cannot be less than 1!\n");
                    return EXIT_FAILURE;
                }
            }
            marker = true;
            break;
        
        // parameter for calculating first n prime numbers
        case 'n': 
            n = atol(optarg);
            mandatory_given = true;
            break;
        
         // Description of all options
help:   case 'h':
            print_help(prog_name);
            return EXIT_SUCCESS;

        // Running execution time tests
        case 'T':
            g = atol(optarg);
            uint64_t* prims_test = (uint64_t*)malloc(g * sizeof(uint64_t));
            if(prims_test == NULL){
                fprintf(stderr,"Invalid Argument! Memory can not be allocated!\n");
                return EXIT_FAILURE;
            }
            compareTime(g,prims_test);
            free(prims_test);
            return EXIT_SUCCESS;

        // Running correctness tests
        case 'C':
            g_new = atol(optarg);
            uint64_t* prims_test_correct = (uint64_t*)malloc(g_new * sizeof(uint64_t));
            if(prims_test_correct == NULL){
                fprintf(stderr,"Invalid Argument! Memory can not be allocated!\n");
                return EXIT_FAILURE;
            }
            compareCorrectness(g_new,prims_test_correct);
            free(prims_test_correct);
            return EXIT_SUCCESS;
             
        // get the information whether -p option is used  
        case 'p':
            printPrims = true;
            break;

        // Not a valid option 
        default: 
            print_usage(prog_name);
            return EXIT_FAILURE;
        }
    }   
    
    // checking the mandatory argument
    if(!mandatory_given){
        fprintf(stderr,"\n-n is a mandatory argument!\n");
        print_usage(prog_name);
        return EXIT_FAILURE;
    }

    if(n == 0){
        fprintf(stderr,"\nSize is 0. Nothing is calculated!\n\n");
        return EXIT_FAILURE;
    }

    uint64_t* prims = (uint64_t*)malloc(n * sizeof(uint64_t));
    if(prims == NULL){
        fprintf(stderr,"Invalid Argument! Memory can not be allocated!\n");
        return EXIT_FAILURE;
    }
    size_t result;
    
    // Decision which version will be executed 
    switch(version){

        case 0:
            if(marker){
                time += get_time_prim(prim,n,prims,repeat);
            }else{
                result = prim(n,prims);
            }
            break;
            
        case 1:
            if(marker){
                time += get_time_prim(prim_V1,n,prims,repeat);
            }else{
                result = prim_V1(n,prims);
            }
            break;

        case 2:
            if(marker){
                time += get_time_prim(prim_V2,n,prims,repeat);
            } else {
                result = prim_V2(n,prims);
            }
            break;

        case 3:
            if(marker){
                time += get_time_prim(prim_V3,n,prims,repeat);
            }else{
                result = prim_V3(n,prims);
            }
            break;

        case 4:
            if(marker){
                time += get_time_prim(prim_V4,n,prims,repeat);
            }else{
                result = prim_V4(n,prims);
            }
            break;
        
        case 5:
            if(marker){
                time += get_time_prim(prim_V5,n,prims,repeat);
            }else{
                result = prim_V5(n,prims);
            }
            break;
        
        case 6:
            if(marker){
                time += get_time_prim(prim_V6,n,prims,repeat);
            }else{
                result = prim_V6(n,prims);
            }
            break;

        case 7:
            if(printPrims){
                printf("\nTable is being created...\n");
            }
            table = createTable(n);
            if(marker){
                time += get_time_prim(prim_V7,n,prims,repeat);
            }else{
                result = prim_V7(n,prims);
            }
            free(table);
            break;

        case 8:
            if(printPrims){
                printf("\nTable is being created...\n");
            }
            table = createTable(n);
            if(marker){
                time += get_time_prim(prim_V8,n,prims,repeat);
            }else{
                result = prim_V8(n,prims);
            }
            free(table);
            break;

        default: 
            return EXIT_FAILURE;
        }

    // printing execution time results 
    if(marker){
        printf("\nRepetition: %d\n",repeat);
        printf("First %zu prime numbers are calculated in %.7f seconds in total.\n",n,time);
        printf("First %zu prime numbers are calculated in %.7f seconds in average.\n\n",n,time/repeat);
    }
    //Printing results 
    else{
        if(result != 0 && printPrims){
            printf("\nFirst %zu prime numbers:\n",n);                
            printf("[");           
            for(size_t i = 0 ; i < result ; i++){
                if(i == n - 1){
                    printf("%"PRIu64"",prims[i]);
                    break;
                }
                printf("%"PRIu64", ",prims[i]);
            }
            printf("]");
            printf("\n\n");
        }
    }

    free(prims);
    return EXIT_SUCCESS;
}