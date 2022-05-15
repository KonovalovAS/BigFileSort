#include "myheader.h"

const MAX_INT = 2147463647;
const int ints_in_1MB = 262144;
const int MB_in_1GB = 1024;

int main(){

    mt19937 Mersenne = mt19937( static_cast<mt19937::result_type>(time(nullptr)) );
    uniform_int_distribution<> rnd{0,MAX_INT};

    // generating a file to work with
    int N(200);
    //cin >> N;
    string origin_name = "data1GB",
           res_name = "res1GB";

    // remove(origin_name.c_str());
    //makefile(N, origin_name, rnd, Mersenne);
    //cout << "done!\n";

    // remove(res_name.c_str());

    // solving
    Solution Sol(origin_name,res_name);
    Sol.Multithread_calculation(10,32*ints_in_1MB);

    //show( res_name );
    cout << "The result is in the file:\n\t" << res_name;

    return 0;
}
