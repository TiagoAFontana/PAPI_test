#include <iostream>
#include <papi.h>
#include <stdio.h>      /* printf */
#include <assert.h>     /* assert */
#include <vector>
#include <valgrind/callgrind.h>
#include <utility>
#include "metric.h"

using namespace std;

class Register
{
public:
    Register(double x, double y) {
        position = make_pair(x, y);
    }

    void setPosition(double x, double y){
        position = make_pair(x, y);
    }

    pair<double, double> getPosition(){
        return position;
    }

private:
    pair<double, double> position;
    double nome;
    long long a;
    long long b;
    long long c;
    long long d;
    long long e;
};


void read_objects(vector<Register> &registers, int iterations){
    pair<double, double> position;
    for (int i = 0; i < iterations; ++i)
    {
        position = registers[rand()%registers.size()].getPosition();
    }
}

void read_all_objects(vector<Register> &registers, int iterations){
    pair<double, double> position;
    for (int i = 0; i < iterations; ++i)
    {
        for (int j = 0; j < registers.size(); ++j)
        {
            position = registers[j].getPosition();
        }
    }
}

void write_objects(vector<Register> &registers, int iterations){

    for (int i = 0; i < iterations; ++i)
    {
        registers[rand()%registers.size()].setPosition((double)(rand()%100), (double)(rand()%100));
    }

}

void write_all_objects(vector<Register> &registers, int iterations){

    for (int i = 0; i < iterations; ++i)
    {
        for (int j = 0; j < registers.size(); ++j)
        {
            registers[j].setPosition((double)(rand()%100), (double)(rand()%100));
        }
    }
}



#define NUMBER_OBJECTS 870000
#define ITERATIONS 1
#define READ_TEST 1
#define WRITE_TEST 1
main()
{
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

    ophidian::experiments::Miss miss(PAPI_events, events_size, PAPI_events_str);

//    Zeus Information
//    int l1_size = 32*1024;
//    int l1_line_size = 64*8;
//    int l2_size = 256*1024;
//    int l2_line_size = 64*8;

    vector<Register> registers;
    registers.reserve(NUMBER_OBJECTS);
    for (int i = 0; i < NUMBER_OBJECTS; ++i)
    {
        registers.push_back(Register((double)(rand()%100), (double)(rand()%100)));
    }

    cout<<"---- INFO: ----"
        <<"\nNumber Objects: "<< NUMBER_OBJECTS
        << "\nIterations: " << ITERATIONS
        << "\nsize Object : " << sizeof(Register)
        << "\nsize Vector : " << sizeof(registers)
        << "\n------" <<endl;

    cout<<"numero de objetos criados : " << registers.size() << endl;


    //---------READ-----------
#if READ_TEST
    cout<<"\n\nRead test\n"<<endl;


    miss.start();

    CALLGRIND_ZERO_STATS;
    CALLGRIND_START_INSTRUMENTATION;

//    read_objects(registers, ITERATIONS);
    read_all_objects(registers, ITERATIONS);

    CALLGRIND_DUMP_STATS;
    CALLGRIND_STOP_INSTRUMENTATION;

    miss.end();
    miss.print_result();

    std::cout<<std::endl;

#endif


#if WRITE_TEST
    //---------WRITE-----------
    cout<<"\n\nWrite test\n"<<endl;

    miss.start();

    CALLGRIND_ZERO_STATS;
    CALLGRIND_START_INSTRUMENTATION;

//    write_objects(registers, ITERATIONS);
    write_all_objects(registers, ITERATIONS);

    CALLGRIND_DUMP_STATS;
    CALLGRIND_STOP_INSTRUMENTATION;

    miss.end();
    miss.print_result();

#endif

}
