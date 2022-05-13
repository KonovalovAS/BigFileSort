#include "myheader.h"

Solution::Solution(){
    prob = "data";
    sol = "result";
    split_counter = 0;
}

Solution::Solution( string problem_fname, string solution_fname ){
    prob = problem_fname;
    sol = solution_fname;
    split_counter = 0;
}

void Solution::split_to_sorted(FILE * f_ptr, int n, string &common_name){

    while(true){

        common_mutex.lock();

            // reading another portion of numbers into vector
            vector<int> read(n); // underlying storage of std::vector is also an array
            size_t realsize = fread(read.data(), sizeof(read[0]), read.size(), f_ptr);

            if(realsize==0)
                break;
            else if(realsize<n)
                read.resize(realsize);

            split_counter++;
            int current_index = split_counter;

        common_mutex.unlock();

        sort(read.begin(),read.end());

        // getting ready to write into a new file
        string fname = common_name + to_string(current_index);
        FILE * out = fopen(fname.c_str(),"wb");
        fwrite(read.data(),sizeof(read[0]),realsize,out);
        fclose(out);

    }

}

void Solution::Multithread_calculation(){


    FILE * problem = fopen(prob.c_str(),"rb");
    string common_parts_name_prefix = "part_";
    int n_in_splitted = 10;
    split_to_sorted( problem, n_in_splitted, common_parts_name_prefix );
    fclose(problem);


}

void Solution::merge_files(string fn1, string fn2){

}

void Solution::demo(){
    // checking the contents of new files
    cout << "\n\nDEMO\n\n";
    for(int i=1; i<split_counter+1; i++){
        string fn = "part_" + to_string(i);
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
}









