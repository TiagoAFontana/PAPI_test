#include <iostream>
using namespace std;
#include <papi.h>
#include <stdio.h>      /* printf */
#include <assert.h>     /* assert */
#include <vector>
#include <valgrind/callgrind.h>


int cache_random_write_test(double *array, int size, int count) {
    int i;
    double a;

    for(i=0; i<count; i++)
    {
        array[rand()%size]=(double)i;
    }


    for(i=0; i<count; i++)
    {
        for(int index=0; index<size; index++)
        {
            array[index]=(double)i;
//            a = array[index];
        }
    }
    return 0;
}

#define ITERATIONS  1000
int main(){

    string PAPI_events_str[] = {
        "PAPI_L1_DCM",
        "PAPI_L1_ICM",
        "PAPI_L1_TCM",

        "PAPI_L2_DCM",
        "PAPI_L2_ICM",
        "PAPI_L2_TCM",

        "PAPI_L3_TCM"
    };
    int PAPI_events[] = {
        PAPI_L1_DCM,
        PAPI_L1_ICM,
        PAPI_L1_TCM,

        PAPI_L2_DCM,
        PAPI_L2_ICM,
        PAPI_L2_TCM,

        PAPI_L3_TCM
    };
    int events_size = 7;

    std::vector<long long> events;
    PAPI_library_init(PAPI_VER_CURRENT);


    int l1_size = 32*1024;
    int l1_line_size = 64*8;

    int l2_size = 256*1024;
    int l2_line_size = 64*8;

//    vector<double> vetor;
//    vetor.resize(l1_size);

    int vector_size = l1_size;
    double * vetor;
    vetor=(double*)calloc(vector_size,sizeof(double));

    int num_runs = 50;
    long long total = 0;
//    for(int i=0; i<num_runs; i++)
//    {

    PAPI_reset(*PAPI_events);
    PAPI_start_counters( PAPI_events, events_size );


    CALLGRIND_ZERO_STATS;
    CALLGRIND_START_INSTRUMENTATION;

//        cache_random_write_test(&vetor[0], vetor.size(), 1000000);
    cache_random_write_test(vetor, vector_size, ITERATIONS);

    CALLGRIND_DUMP_STATS;
    CALLGRIND_STOP_INSTRUMENTATION;

    long long counters[events_size];
    PAPI_stop_counters(counters, events_size);


//    std::cout << counters[0] << std::endl;
//    total += counters[0];

//        long long counters[events_size];
//        PAPI_read_counters( counters, events_size );
//        events = std::vector<long long>(counters, counters + sizeof counters / sizeof counters[0]);

    int index = 0;
    for(auto e : counters)
    {
        std::cout << PAPI_events_str[index] << " "<<e<<"\n";
        index++;
    }
    std::cout<<std::endl;

//    }

//    long long media = total / num_runs;
//    long long expected=(ITERATIONS*7)/8;

//    std::cout << "Média : \t" << total / num_runs << std::endl;
//    double error=(((double)media-expected)/expected)*100.0;

//    std::cout << "Esperado : \t" << expected << std::endl;
//    std::cout << "Diff : \t\t" << media - expected << std::endl;
//    std::cout << "Erro : \t\t" << error << " %" << std::endl;

}
