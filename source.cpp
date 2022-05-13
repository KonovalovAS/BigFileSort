#include "myheader.h"

Solution::Solution(){
    prob = "data";
    sol = "result";
    split_counter = 0;
    threads_working = 0;
}

Solution::Solution( string problem_fname, string solution_fname ){
    prob = problem_fname;
    sol = solution_fname;
    split_counter = 0;
    threads_working = 0;
}

void Solution::split_to_sorted(FILE * f_ptr, int n, string &common_name){

    while(true){

        reading_mutex.lock();

            // reading another portion of numbers into vector
            vector<int> read(n); // underlying storage of std::vector is also an array
            size_t realsize = fread(read.data(), sizeof(read[0]), read.size(), f_ptr);

            if(realsize==0){
                // unlocking other threads before leaving the cycle-body:
                reading_mutex.unlock();
                break;
            }
            else if(realsize<n)
                read.resize(realsize);

            split_counter++;
            int current_index = split_counter;

        // if the cycle is not left, unlocking here:
        reading_mutex.unlock();

        sort(read.begin(),read.end());

        // getting ready to write into a new file
        string fname = common_name + to_string(current_index);
        FILE * out = fopen(fname.c_str(),"wb");
        fwrite(read.data(),sizeof(read[0]),realsize,out);
        fclose(out);

    }

}

void Solution::thread_run( FILE * problem ){

    string common_parts_name_prefix = "part_";
    int n_in_splitted = 10;
    split_to_sorted( problem, n_in_splitted, common_parts_name_prefix );

}

void Solution::Multithread_calculation( int req_num_treads ){

	int max_threads = thread::hardware_concurrency();
	int num_threads = min(max_threads,req_num_treads);

	// cout << num_threads << "\n";

    FILE * problem = fopen(prob.c_str(),"rb");

    // threads begin their work
	vector<thread> threads(num_threads-1);
    for(int i=0; i<num_threads-1; i++)
          threads[i] = thread(&Solution::thread_run, this, ref(problem));
    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
    // and they are done by now

    fclose(problem);

}

void Solution::merge_files(string fn1, string fn2){
    FILE * f1 = fopen(fn1.c_str(),"rb");
    FILE * f2 = fopen(fn2.c_str(),"rb");

    string outfname = ( *(fn1.end()-1)=='_' ) ?
                        string(fn1.begin(),fn1.end()-1) : (fn1 + "_");
    FILE * outf = fopen(outfname.c_str(),"wb");

    int i1, i2;
    int s_i1 = fread(&i1,sizeof(int),1,f1),
        s_i2 = fread(&i2,sizeof(int),1,f2);

    while( s_i1*s_i2>0 ){
        if(i2<i1){
            fwrite(&i2,sizeof(int),1,outf);
            s_i2 = fread(&i2,sizeof(int),1,f2);
        }
        else{
            fwrite(&i1,sizeof(int),1,outf);
            s_i1 = fread(&i1,sizeof(int),1,f1);
        }
    }
    if(s_i1==0)
        while(s_i2>0){
            fwrite(&i2,sizeof(int),1,outf);
            s_i2 = fread(&i2,sizeof(int),1,f2);
        }
    else
        while(s_i1>0){
            fwrite(&i1,sizeof(int),1,outf);
            s_i1 = fread(&i1,sizeof(int),1,f1);
        }

    fclose(outf);
    fclose(f1);
    fclose(f2);

    remove(fn1.c_str());
    remove(fn2.c_str());
}



my_queue::my_queue(){
    not_empty = false;
    expecting = false;
}

void my_queue::push( string& s ){
    if(expecting){
        Q.push( {tmp_hold, s} );
        expecting = false;
        not_empty = true;
    }
    else{
        tmp_hold = s;
        expecting = true;
    }
}

fpair my_queue::extract_front(){
    fpair tmp = Q.front();
    Q.pop();
    not_empty = !Q.empty();
    return tmp;
}

string my_queue::get_remains(){
    return tmp_hold;
}
//===

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









