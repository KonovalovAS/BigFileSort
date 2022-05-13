#include <iostream>
#include <fstream>
#include <stdio.h>
#include <functional>

#include <algorithm>
#include <random>
#include <time.h>

#include <thread>
#include <mutex>

#include <string>
#include <vector>
#include <queue>

using namespace std;

struct fpair{
    string fn1, fn2;
};

class my_queue{
public:
    my_queue();

    void push( string& s );
    fpair extract_front();
    bool not_empty;

    string get_remains();
    bool expecting;

private:

    string tmp_hold;
    queue<fpair> Q;
};

class Solution{
public:
    Solution();
    Solution(string problem_fname, string solution_fname);
    void Multithread_calculation( int req_num_treads );

    string Results_fname();

    void demo();

    // temporary public:
    // nothing
    //string merge_files(string fn1, string fn2);

private:

    mutex reading_mutex,
          queue_mutex;

    string prob,
           sol;
    int split_counter;
    int threads_working;

    string merge_files(string fn1, string fn2);
    void split_to_sorted(FILE * f_ptr, int n,
                         string &common_name, my_queue& Mqueue);
    void thread_run( FILE * problem, my_queue& Mqueue );

};

///=================

void makefile(int n, string origin_name, uniform_int_distribution<> rnd, mt19937 u);

void show(string name);

void test_my_queue();
