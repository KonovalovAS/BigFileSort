#include <iostream>
#include <fstream>
#include <stdio.h>
#include <random>
#include <time.h>
#include <thread>
#include <mutex>
#include <string>
#include <algorithm>

using namespace std;

class Solution{
public:
    Solution();
    Solution(string problem_fname, string solution_fname);
    void Multithread_calculation();

    void demo();

private:

    mutex common_mutex;
    string prob,
           sol;
    int split_counter;

    void split_to_sorted(FILE * f_ptr, int n, string &common_name);
    void merge_files(string fn1, string fn2);

};
