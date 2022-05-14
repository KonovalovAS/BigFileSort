#include "myheader.h"

Solution::Solution(){
    prob = "data";
    sol = "";
    split_counter = 0;
    threads_working = 0;
}

Solution::Solution( string problem_fname, string solution_fname ){
    prob = problem_fname;
    sol = solution_fname;
    split_counter = 0;
    threads_working = 0;
}

string Solution::Results_fname(){
    return sol;
}

void Solution::split_to_sorted(FILE * f_ptr, int n,
                               string &common_name, my_queue& Mqueue){

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

        queue_mutex_1.lock();
        Mqueue.push(fname);
        queue_mutex_1.unlock();

    }

}

void Solution::thread_run( FILE * problem, my_queue& Mqueue, int init_split_size ){

    string common_parts_name_prefix = "part_";
    int n_in_splitted = init_split_size;
    split_to_sorted( problem, n_in_splitted, common_parts_name_prefix, Mqueue );


    while(true){

        if( Mqueue.not_empty ){

            queue_mutex_2.lock();
            fpair fp = Mqueue.extract_front();
            threads_working ++;
            queue_mutex_2.unlock();

            string merged = merge_files(fp.fn1, fp.fn2);

            queue_mutex_2.lock();
            threads_working --;
            Mqueue.push(merged);
            queue_mutex_2.unlock();
        }
        else if(threads_working>0)
            this_thread::sleep_for(chrono::milliseconds(100));
        else
            break;
    }

}

void Solution::Multithread_calculation( int req_num_treads, int init_split_size ){

	int max_threads = thread::hardware_concurrency();
	int num_threads = min(max_threads,req_num_treads);

	//cout << "Num_threads: " << num_threads << "\n";

    FILE * problem = fopen(prob.c_str(),"rb");
    my_queue MyQueue;

    cout << "The process is on\n";
    // threads begin their work
	vector<thread> threads(num_threads-1);
    for(int i=0; i<num_threads-1; i++)
          threads[i] = thread(&Solution::thread_run, this, ref(problem), ref(MyQueue), ref(init_split_size));

    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
    // and they are done by now
    cout << "Done\n";

    fclose(problem);

    string tmp = MyQueue.get_remains();
    rename( tmp.c_str(), sol.c_str() );

}

string Solution::merge_files(string fn1, string fn2){
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

    return outfname;
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

/// my_queue

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

/// Some auxiliary stuff

void makefile(int n, string origin_name, uniform_int_distribution<> rnd, mt19937 u){

    FILE * f = fopen(origin_name.c_str(),"wb");
    for(int i=0; i<n; i++){
        int it = rnd(u);
        //cout << it << " ";
        fwrite(&it,sizeof(int),1, f);
    }
    //cout << "\n";
    fclose(f);
}

void show(string name){
    FILE * f = fopen(name.c_str(),"rb");

    int n = 10000;
    int * read = new int[n];
    int realsize = fread(read,sizeof(int),n,f);
    cout << "Show " << name << ":\n\trealsize: " << realsize << "\n";

    for(int i=0; i<realsize; i++)
        cout << read[i] << " ";
    cout << "\n";

    delete [] read;

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









