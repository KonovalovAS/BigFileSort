#include "myheader.h"

void makefile(int n, string origin_name, uniform_int_distribution<> rnd, mt19937 u){

    FILE * f = fopen(origin_name.c_str(),"wb");
    for(int i=0; i<n; i++){
        int it = rnd(u);
        cout << it << " ";
        fwrite(&it,sizeof(int),1, f);
    }
    cout << "\n";
    fclose(f);
}

int main(){

    mt19937 Mersenne = mt19937( static_cast<mt19937::result_type>(time(nullptr)) );
    uniform_int_distribution<> rnd{0,500};

    // generating a file to work with
    int N(24),n(10);
    string origin_name = "data";
    makefile(N, origin_name, rnd, Mersenne);

    string res = "res";
    //Solution S();
    Solution Sol(origin_name,res);
    Sol.Multithread_calculation();

    Sol.demo();

    return 0;
}
