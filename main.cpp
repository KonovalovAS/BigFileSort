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

void show(string name){
    FILE * f = fopen(name.c_str(),"rb");

    int n = 100;
    int * read = new int[n];
    int realsize = fread(read,sizeof(int),n,f);
    cout << "Show " << name << ":\n\trealsize: " << realsize << "\n";

    for(int i=0; i<realsize; i++)
        cout << read[i] << " ";
    cout << "\n";

    fclose(f);
}

void test_my_queue(){
    string a("a"), b("b"), c("c"), d("d"), e("e");

    my_queue MQ;

    cout << MQ.not_empty << " " << MQ.expecting << " "
         << MQ.get_remains() << "\n";

    MQ.push(a);

    cout << MQ.not_empty << " " << MQ.expecting << " "
         << MQ.get_remains() << "\n";

    MQ.push(b);

    cout << MQ.not_empty << " " << MQ.expecting << " "
         << MQ.get_remains() << "\n";

    MQ.push(c);

    cout << MQ.not_empty << " " << MQ.expecting << " "
         << MQ.get_remains() << "\n";

    fpair gf = MQ.extract_front();
    cout << gf.fn1 << " " << gf.fn2 << "\n";

    MQ.push(d);

    cout << MQ.not_empty << " " << MQ.expecting << " "
         << MQ.get_remains() << "\n";

    gf = MQ.extract_front();
    cout << gf.fn1 << " " << gf.fn2 << "\n";

    cout << MQ.not_empty << " " << MQ.expecting << " "
         << MQ.get_remains() << "\n";
}

int main(){

    mt19937 Mersenne = mt19937( static_cast<mt19937::result_type>(time(nullptr)) );
    uniform_int_distribution<> rnd{0,500};

    // generating a file to work with
    int N(24),n(10);
    string origin_name = "data";
    makefile(N, origin_name, rnd, Mersenne);

    //test_my_queue();

    /// testing reading and splitting

    string res = "res";
    //Solution S();
    Solution Sol(origin_name,res);
    Sol.Multithread_calculation(10);

    Sol.demo();

    /// testing merge-function
    /*
    Sol.merge_files("part_1","part_3");

    string nm = "part_1_";
    show(nm);
    */

    return 0;
}
