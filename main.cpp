#include "myheader.h"

int main(){

    mt19937 Mersenne = mt19937( static_cast<mt19937::result_type>(time(nullptr)) );
    uniform_int_distribution<> rnd{0,1000};

    // generating a file to work with
    int N(15000);
    string origin_name = "data",
           res_name = "res";
    makefile(N, origin_name, rnd, Mersenne);

    // solving
    Solution Sol(origin_name,res_name);
    Sol.Multithread_calculation(10);

    //show( res_name );
    cout << "The result is in the file:\n\t" << res_name;

    return 0;
}
