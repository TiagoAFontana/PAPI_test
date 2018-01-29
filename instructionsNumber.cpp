#include <iostream>

using namespace std;

#include <papi.h>
#include <stdio.h>      /* printf */
#include <assert.h>     /* assert */

//int main(int argc, char *argv[])
//{
//    cout << "Hello World!" << endl;
//    return 0;
//}


double a = 0.5, b = 2.2;
void do_flops( int n )
{
    int i;
    double c = 0.11;

    for ( i = 0; i < n; i++ )
    {
        c += a * b;
    }
//    dummy{ ( void * ) &c};
}


#define NUM_FLOPS 10000
#define NUM_EVENTS 7

main()
{
//    int Events[NUM_EVENTS] = {PAPI_TOT_INS};
    int Events[NUM_EVENTS]  = {
        PAPI_L1_DCM,
        PAPI_L1_ICM,
        PAPI_L1_TCM,

        PAPI_L2_DCM,
        PAPI_L2_ICM,
        PAPI_L2_TCM,

        PAPI_L3_TCM
    };
    long_long values[NUM_EVENTS];

/* Start counting events */
    assert(PAPI_start_counters(Events, NUM_EVENTS) == PAPI_OK);
//        handle_error(1);

/* Defined in tests/do_loops.c in the PAPI source distribution */
    do_flops(NUM_FLOPS);

/* Read the counters */
    assert(PAPI_read_counters(values, NUM_EVENTS) == PAPI_OK);
//        handle_error(1);

    printf("After reading the counters: %lld\n",values[0]);

    do_flops(NUM_FLOPS);

/* Add the counters */
    assert(PAPI_accum_counters(values, NUM_EVENTS) == PAPI_OK);
//        handle_error(1);
    printf("After adding the counters: %lld\n", values[0]);

    do_flops(NUM_FLOPS);

/* Stop counting events */
    assert(PAPI_stop_counters(values, NUM_EVENTS) == PAPI_OK);
//        handle_error(1);

    printf("After stopping the counters: %lld\n", values[0]);
}
