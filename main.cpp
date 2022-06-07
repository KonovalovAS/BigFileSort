#include "myheader.h"

const int MAX_INT = 2147463647;
const int ints_in_1MB = 262144;
const int MB_in_1GB = 1024;

int main(){

    mt19937 Mersenne = mt19937( static_cast<mt19937::result_type>(time(nullptr)) );
    uniform_int_distribution<> rnd{0,MAX_INT};

    // generating a file to work with
    int N(179*ints_in_1MB);
    //cin >> N;
    string origin_name = "data179MB",
           res_name = "res179MB";

    makefile(N, origin_name, rnd, Mersenne);

    auto start = clock();
    // solving

    Solution Sol(origin_name,res_name);
    Sol.Multithread_calculation(10,7*ints_in_1MB);

    cout << "The result is in the file:\n\t" << res_name;

    double t = (double)(clock() - start) / CLOCKS_PER_SEC;
    cout << "\nTime taken (seconds):\n\t" << t;

    return 0;
}
