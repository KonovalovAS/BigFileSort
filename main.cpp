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

mutex g_m;
static int counter = 0;

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

void read_n_write(FILE * f_ptr, int n, string &name){

    while(true){

        lock_guard<mutex> lock(g_m);
        string fname = name + to_string(counter);
        // reading another portion of numbers

        // reading into vector
        vector<int> read(n); // underlying storage of std::vector is also an array
        size_t realsize = fread(read.data(), sizeof(read[0]), read.size(), f_ptr);

        if(realsize==0)
            break;
        else if(realsize<n)
            read.resize(realsize);

        counter++;

        sort(read.begin(),read.end());

        // getting ready to write into a new file
        FILE * out = fopen(fname.c_str(),"wb");
        fwrite(read.data(),sizeof(read[0]),realsize,out);
        fclose(out);
    }
}

int main(){

    mt19937 Mersenne = mt19937( static_cast<mt19937::result_type>(time(nullptr)) );
    uniform_int_distribution<> rnd{0,500};

    // generating a file to work with
    int N(57),n(10);
    string origin_name = "data";
    makefile(N, origin_name, rnd, Mersenne);

    // opening the created file
    FILE * f = fopen(origin_name.c_str(),"rb");

    string common_name_ = "part_";

    // threads:
    thread t1 (read_n_write,ref(f),ref(n),ref(common_name_)),
           t2 (read_n_write,ref(f),ref(n),ref(common_name_));

    t1.join();
    t2.join();

    fclose(f);
    int rm = remove(origin_name.c_str());
    (rm) ? cout << "deletion failed\n" : cout << "deletion is complete\n";

    cout<< "\n\n";

    // checking the contents of new files
    for(int i=0; i<counter; i++){
        string fn = common_name_ + to_string(i);
        FILE * f_test = fopen(fn.c_str(),"rb");

        cout<<i<<"\n";
        int * read = new int[10];
        int num = fread(read,sizeof(int),10,f_test);

        for(int i=0; i<num; i++)
            cout << read[i] << " ";

        cout<<"\n";
        fclose(f_test);

        delete [] read;
    }

    return 0;
}
