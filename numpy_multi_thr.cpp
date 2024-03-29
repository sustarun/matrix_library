// #include <iostream>
// #include <list>
// #include <memory>
// #include <chrono>
// #include <cassert>
#include <thread>
#include <bits/stdc++.h>
using namespace std;
#define PI 3.14159265358979323846
template <class T>
string list_to_string(list<T> l){
    string res = "[";
    for (auto val : l){
        res += to_string(val) + ",";
    }
    res += "]";
    return res;
}
template <class T>
string vec_to_string(vector<T> v){
	string res = "[";
	if(v.size()>0)res+=to_string(v[0]);
	for (int i=1; i<v.size(); i++){
		res += "," + to_string(v[i]) ;
	}
	res += "]";
	return res;
}
template <class T>
T prod_elems_in_list(list<T> &l){
    T res = T(1);
    for(auto elem : l){
        res *= elem;
    }
    return res;
}
template <class T>
T prod_elems_in_vector(vector<T> &v){
	T res = T(1);
	for (auto elem:v){
		res *= elem;
	}
	return res;
}
template <class T>
T validate_vec(vector<T> &v){
	for(auto elem:v){
		if (elem <= 0){
			return false;
		}
	}
	return true;
}
template <class T>
list<T> make_list_from_arr(T *arr){
    list<T> res(arr, arr+sizeof(arr)/sizeof(T));
    return res;
}
vector<unsigned long long int> prods(vector<unsigned int> shape){
	vector<unsigned long long int> prod_vec;
	unsigned long long int prod = 1;
	unsigned int dimns = shape.size();
	for (int i = 0; i < dimns; ++i)
	{
		prod *= shape[dimns-i-1];
		prod_vec.push_back(prod);
	}
	return prod_vec;
}
template <class T>
void adh(T* lhs, T* rhs0, T* rhs1, unsigned int num_elems){
	for(; num_elems > 0; num_elems--){
		*lhs = *rhs0 + *rhs1;
		lhs++;rhs0++;rhs1++;
	}
	return;
}

vector<unsigned long long int> ret_cum_shape(vector<unsigned int> & shape){
	vector<unsigned long long int> res(shape.size(), 1);
	for (int i = shape.size()-2; i>= 0; i--){
		res[i] = res[i+1]*(shape[i+1]);
	}
	return res;
}

vector<unsigned long long int> ret_cum_shape2(vector<unsigned int> & shape){
	vector<unsigned long long int> res(shape.size(), 1);
	for (int i = shape.size()-2; i>= 0; i--){
		res[i] = res[i+1]*(shape[i]);
	}
	return res;
}

string to_string(complex<double> &a){
	return "(" + to_string( real(a)) +    " + " + to_string(imag(a))  +    " i )" ;
	// return "(" + to_string( real(a) ==  (double ) 0  ? 0 : real(a) ) +    " + " + to_string(imag(a))  +    " i )" ;
}

template<class T>
void separate (vector<T> &a, int offset, int n) {
    vector<T> b(n/2);  			// get temp heap storage
    for(int i=0; i<n/2; i++)    // copy all odd elements to heap storage
        b[i] = a[offset+ i*2+1];
    for(int i=0; i<n/2; i++)    // copy all even elements to lower-half of a[]
        a[offset+ i] = a[offset+ i*2];
    for(int i=0; i<n/2; i++)    // copy all odd (from heap) to upper-half of a[]
        a[offset + i+n/2] = b[i];
    // delete[] b;                 // delete heap storage
}
template<class T>
void fft2 (vector<T> &X, int offset,int N) {
    if(N < 2) {
    } else {
        separate(X,offset, N);      // all evens to lower half, all odds to upper half
        fft2(X, offset,     N/2);   // recurse even items
        fft2(X, offset + N/2, N/2);   // recurse odd  items
        for(int k=0; k<N/2; k++) {
            T e = X[offset + k    ];   // even
            T o = X[offset + k+N/2];   // odd
            T w = exp( complex<double>(0,-2.*PI*k/N) );
            X[offset + k    ] = e + w * o;
            X[offset + k+N/2] = e - w * o;
        }
    }
}

template<class T>
class gen_arr{
	vector<unsigned int> shape;
	vector<unsigned long long int> cumulative_shape;
	int ndim;
	shared_ptr<T> arr;
	int arr_len;
	int offset;
	static void add_helper(T* lhs, T* rhs0, T* rhs1, unsigned int num_elems){
		for(int i = 0; i < num_elems; i++){
			lhs[i] = rhs0[i] + rhs1[i];
		}
		return;
	}
	static void sub_helper(T* lhs, T* rhs0, T* rhs1, unsigned int num_elems){
		for(int i = 0; i < num_elems; i++){
			lhs[i] = rhs0[i] - rhs1[i];
		}
		return;
	}
	static void mul_helper(T* lhs, T* rhs0, T* rhs1, unsigned int num_elems){
		for(int i = 0; i < num_elems; i++){
			lhs[i] = rhs0[i] * rhs1[i];
		}
		return;
	}
	static void div_helper(T* lhs, T* rhs0, T* rhs1, unsigned int num_elems){
		for(int i = 0; i < num_elems; i++){
			lhs[i] = rhs0[i] / rhs1[i];
		}
		return;
	}
	static void fn_helper(T* lhs, T* rhs0, T* rhs1, unsigned int num_elems, function<T(T,T)> &fn){
		for(int i = 0; i < num_elems; i++){
			lhs[i] = fn(rhs0[i], rhs1[i]);
		}
		return;
	}
	static void fn_helper(T* lhs, T* rhs0, unsigned int num_elems, function<T(T)> &fn){
		for(int i = 0; i < num_elems; i++){
			lhs[i] = fn(rhs0[i]);
		}
		return;
	}
	static T basic_op_add(T a, T b){
		return a + b;
	}
	static T basic_op_sub(T a, T b){
		return a - b;
	}
	static T basic_op_mul(T a, T b){
		return a * b;
	}
	static T basic_op_div(T a, T b){
		return a / b;
	}

public:
	gen_arr(){
		this->arr.reset();
		this->ndim = 0;
		this->arr_len = 0;
		this->offset = 0;
	}
	void init(vector<unsigned int> shape, bool alloc=true){
		assert(validate_vec(shape));assert(validate_vec(shape));
		this->shape = shape;
		this->cumulative_shape = ret_cum_shape(this->shape);
		this->ndim = shape.size();
		this->arr_len = prod_elems_in_vector(shape);
		if (alloc){
			this->arr = shared_ptr<T>(new T[arr_len], std::default_delete<T[]>());
		}
		else{
			this->arr.reset();
		}
		this->offset = 0;
	}
	gen_arr(vector<unsigned int> shape, T init_val, bool do_init=true, bool alloc=true){
		if (! alloc){
			printf("non alloc constructor called\n");
		}
		init(shape, alloc);
		if (do_init && alloc){
			fill_n(arr.get(), this->arr_len, init_val);
		}
	}
	// gen_arr(vector<unsigned int> shape, T init_val){
	// 	// assert(validate_vec(shape));
	// 	init(shape, true);
	// 	fill_n(arr.get(), this->arr_len, init_val);
	// 	cout << endl;
	// }
	gen_arr(vector<unsigned int> shape){
		init(shape);
	}
	string print_stats(){
		string res = "";
		res = res + "shape:\t" + vec_to_string(this->shape)+";\t";
		res = res + "ndim:\t" + to_string(this->ndim)+";\t";
		res = res + "arr_len:\t" + to_string(this->arr_len)+";\t";
		res = res + "offset:\t" + to_string(this->offset)+";\t";
		return res;
	}
	gen_arr & multi_threaded_op(gen_arr<T>  & rhs, int num_thr, function<T(T,T)> fn){
		assert(this->shape == rhs.shape);
		gen_arr<T> *res = new gen_arr<T>;
		res->init(this->shape, true);
		// gen_arr<T> res(this->shape);
		int num_elem = this->arr_len;
		int load_per_thr = num_elem / num_thr;
		int num_left = num_elem - load_per_thr * num_thr;
		int num_thrs = num_left > 0 ? num_thr + 1 : num_thr;
		// printf("num_elem:%d;load_per_thr:%d;num_left:%d;num_thr:%d,num_thrs:%d\n", num_elem, load_per_thr, num_left, num_thr, num_thrs);
		thread workers[num_thrs];
		// gen_arr<T> res(this->shape);
		T *lhsptr, *rhs0ptr, *rhs1ptr;
		lhsptr = res->arr.get();
		rhs0ptr = this->arr.get()+this->offset;
		rhs1ptr = rhs.arr.get()+rhs.offset;
		for (int i = 0; i < num_thr; i++){
			workers[i] = thread(&gen_arr<T>::fn_helper, lhsptr, rhs0ptr, rhs1ptr, load_per_thr, fn);
			// workers[i] = thread(&gen_arr<T>::add_helper, lhsptr, rhs0ptr, rhs1ptr, load_per_thr);
			lhsptr+= load_per_thr;
			rhs0ptr += load_per_thr;
			rhs1ptr += load_per_thr;
		}
		if (num_left > 0){
			workers[num_thr] = thread(&gen_arr<T>::fn_helper, lhsptr, rhs0ptr, rhs1ptr, num_left, fn);
			// workers[num_thr] = thread(&gen_arr<T>::add_helper, lhsptr, rhs0ptr, rhs1ptr, num_left);
		}
		for (int i = 0; i < num_thrs; i++){
			workers[i].join();
		}
		return *res;
	}

	gen_arr & multi_threaded_op(int num_thr, function<T(T)> fn){
		gen_arr<T> *res = new gen_arr<T>;
		res->init(this->shape, true);
		// gen_arr<T> res(this->shape);
		int num_elem = this->arr_len;
		int load_per_thr = num_elem / num_thr;
		int num_left = num_elem - load_per_thr * num_thr;
		int num_thrs = num_left > 0 ? num_thr + 1 : num_thr;
		// printf("num_elem:%d;load_per_thr:%d;num_left:%d;num_thr:%d,num_thrs:%d\n", num_elem, load_per_thr, num_left, num_thr, num_thrs);
		thread workers[num_thrs];
		// gen_arr<T> res(this->shape);
		T *lhsptr, *rhs0ptr;
		lhsptr = res->arr.get();
		rhs0ptr = this->arr.get()+this->offset;
		for (int i = 0; i < num_thr; i++){
			workers[i] = thread(&gen_arr<T>::fn_helper, lhsptr, rhs0ptr, load_per_thr, fn);
			// workers[i] = thread(&gen_arr<T>::add_helper, lhsptr, rhs0ptr, rhs1ptr, load_per_thr);
			lhsptr+= load_per_thr;
			rhs0ptr += load_per_thr;
		}
		if (num_left > 0){
			workers[num_thr] = thread(&gen_arr<T>::fn_helper, lhsptr, rhs0ptr, num_left, fn);
			// workers[num_thr] = thread(&gen_arr<T>::add_helper, lhsptr, rhs0ptr, rhs1ptr, num_left);
		}
		for (int i = 0; i < num_thrs; i++){
			workers[i].join();
		}
		return *res;
	}
	gen_arr & single_threaded_op(function<T(T)> fn){
		gen_arr<T> *res = new gen_arr<T>;
		res->init(this->shape, true);
		int num_elem = this->arr_len;
		T* lhsptr, *rhs0ptr;
		lhsptr = res->arr.get();
		rhs0ptr = this->arr.get()+this->offset;
		fn_helper(lhsptr, rhs0ptr, num_elem, fn);
		return *res;
	}
	gen_arr & multi_threaded_op2(gen_arr<T>  & rhs, int num_thr, string name){
		assert(this->shape == rhs.shape);
		gen_arr<T> *res = new gen_arr(this->shape);
		// gen_arr<T> res(this->shape);
		int num_elem = this->arr_len;
		int load_per_thr = num_elem / num_thr;
		int num_left = num_elem - load_per_thr * num_thr;
		int num_thrs = num_left > 0 ? num_thr + 1 : num_thr;
		// printf("num_elem:%d;load_per_thr:%d;num_left:%d;num_thr:%d,num_thrs:%d\n", num_elem, load_per_thr, num_left, num_thr, num_thrs);
		thread workers[num_thrs];
		// gen_arr<T> res(this->shape);
		T *lhsptr, *rhs0ptr, *rhs1ptr;
		lhsptr = res->arr.get();
		rhs0ptr = this->arr.get()+this->offset;
		rhs1ptr = rhs.arr.get()+rhs.offset;
		function<void(T*,T*, T*, unsigned int)> fun_ptr;
		// void (fun_ptr(T*,T*, T*, unsigned int));
		// T* lhs, T* rhs0, T* rhs1, unsigned int num_elems
		if (name=="add"){
			fun_ptr= gen_arr<T>::add_helper;
		}
		else if (name=="sub"){
			fun_ptr = gen_arr<T>::sub_helper;
		}
		else if (name=="mul"){
			fun_ptr = gen_arr<T>::mul_helper;
		}
		else {
			fun_ptr = gen_arr<T>::div_helper;
		}
		for (int i = 0; i < num_thr; i++){
			workers[i] = thread(fun_ptr, lhsptr, rhs0ptr, rhs1ptr, load_per_thr);
			// workers[i] = thread(&gen_arr<T>::add_helper, lhsptr, rhs0ptr, rhs1ptr, load_per_thr);
			lhsptr+= load_per_thr;
			rhs0ptr += load_per_thr;
			rhs1ptr += load_per_thr;
		}
		if (num_left > 0){
			workers[num_thr] = thread(&adh<T>, lhsptr, rhs0ptr, rhs1ptr, num_left);
			// workers[num_thr] = thread(&gen_arr<T>::add_helper, lhsptr, rhs0ptr, rhs1ptr, num_left);
		}
		for (int i = 0; i < num_thrs; i++){
			workers[i].join();
		}
		return *res;
	}
	gen_arr & multi_threaded_add(gen_arr<T>  & rhs, int num_thr){
		return multi_threaded_op2(rhs, num_thr, "add");
	}
	gen_arr & multi_threaded_sub(gen_arr<T> &rhs, int num_thr){
		return multi_threaded_op2(rhs, num_thr, "sub");
	}
	gen_arr & multi_threaded_mul(gen_arr<T> &rhs, int num_thr){
		return multi_threaded_op2(rhs, num_thr, "mul");
	}
	gen_arr & multi_threaded_div(gen_arr<T> &rhs, int num_thr){
		return multi_threaded_op2(rhs, num_thr, "div");
	}

	static void min_max_thr(T* rhs, T *res, int num_elem, string fn){
		*res = *rhs;
		for(int i = 0; i < num_elem; i++)
		{	
			if(fn == "min"){
				if(*rhs < *res)
					*res = *rhs;
			}

			else if(fn == "max"){
				if (*rhs > *res)
					*res = *rhs;
			}
			rhs++;
		}

		
	}

	T  min_max_value(int num_thr, string fn){
		int num_elem = this->arr_len/num_thr;

		int num_left = arr_len - num_elem * num_thr;
		int num_thrs = num_left > 0 ? num_thr + 1 : num_thr;

		vector<unsigned int> res_shape;
		
		res_shape.push_back(1);
		res_shape.push_back(num_thrs);
		gen_arr *res_arr = new gen_arr(res_shape);

		thread workers[num_thrs];
		T *rhsptr, *res;
		res = res_arr->arr.get();

		rhsptr = this->arr.get() + this->offset;

		T ans = *rhsptr;

		for(int i = 0; i < num_thr; i++)
		{
			workers[i] = thread(&min_max_thr, rhsptr, res, num_elem,fn);
			rhsptr += num_elem;
			res++;
			
		}

		if(num_left > 0){
			workers[num_thr] = thread(&min_max_thr, rhsptr, res, num_left, fn);
			res++;
		}

		res -= num_thrs;

		for (int i = 0; i < num_thrs; i++)
		{
			workers[i].join();
			if(fn == "min"){
				if(*res<ans)
					ans = *res;
			}

			else if (fn == "max"){
				if (*res > ans)
					ans = *res;
			}			

			res++;
		}
		

		return ans;
	}

	T min_value(int num_thr){
		return min_max_value(num_thr, "min");
	}

	T max_value(int num_thr){
		return min_max_value(num_thr, "max");
	}

	static void trace_single_thr(T* inp, T* res, int load, int m, int jump){
		
		for(int i = 0; i < load; i++)
		{
			T temp = (T) 0;
			for(int j = 0; j < m; j++)
			{
				temp += *inp;
				inp += jump;
			}

			inp -= jump*m; inp++;
			*res = temp;
			res++;
			
		}

		
	}

	static void add_single_col(T *inp, T *arr, int jump, int load_col){

		
		T temp = (T) 0;
		for (int j = 0; j < load_col; j++)
		{
			temp += *inp;
			inp += jump;
		}

		*arr = temp;
	
	}

	gen_arr trace(int num_thr){
		vector<unsigned int> res_shape;
		vector<unsigned int> temp = this->shape;

		int m1 = temp.front(); temp.erase(temp.begin());
		int m2 = temp.front(); temp.erase(temp.begin());

		int m = min(m1, m2);

		res_shape.push_back(1);
		while(!temp.empty()){
			res_shape.push_back(temp.front());
			temp.erase(temp.begin());
		}

		gen_arr<T> *res = new gen_arr(res_shape);

		int num_elem = this->arr_len;

		T *lhsptr, *rhsptr;
		lhsptr = res->arr.get();
		rhsptr = this->arr.get() + this->offset;

		int res_shape_prod = prod_elems_in_vector(res->shape) ;
		int load = res_shape_prod/ num_thr;
		int load_left = res_shape_prod - (load * num_thr);

		if(load > 0){
			int num_thrs = load_left > 0 ? num_thr + 1 : num_thr;
			thread workers[num_thrs];

			long long int jump = res_shape_prod * (m2 + 1);
			for (int i = 0; i < num_thr; i++)
			{
				workers[i] = thread(&trace_single_thr, rhsptr, lhsptr, load, m, jump);
				rhsptr += load;
				lhsptr += load;
			}

			if(load_left > 0){
				workers[num_thr] = thread(&trace_single_thr, rhsptr, lhsptr, load_left, m, jump);
			}

			for (int i = 0; i < num_thrs; i++)
			{	
				workers[i].join();
			}
		}

		else{
			load = num_thr / res_shape_prod;
			load_left = num_thr - load * res_shape_prod;
			int col = load_left == 0 ? res_shape_prod : res_shape_prod -1;

			thread workers[num_thr];

			int jump = res_shape_prod * (m2 + 1);

			T arr[num_thr];

			int thr = 0;

			for(int  j = 0; j < col; j++){
				int load_col = m / load;
				int load_col_left = m - load_col * (load - 1);

				for (int i = 0; i < load - 1; i++)
				{
					workers[thr] = thread(&add_single_col, rhsptr, &(arr[thr]), jump, load_col);
					rhsptr += jump * load_col;
					thr++;
				}

				workers[thr] = thread(&add_single_col, rhsptr, &(arr[thr]), jump, load_col_left);
				thr++;

				rhsptr -= jump * load_col * (load - 1);
				rhsptr++;
			}

			if(load_left > 0){
				load += load_left;
				int load_col = ceil(m / load);
				int load_col_left = m - load_col * (load - 1);

				for (int i = 0; i < load - 1; i++)
				{
					workers[thr] = thread(&add_single_col, rhsptr, &(arr[thr]), jump, load_col);
					rhsptr += jump * load_col;
					thr++;
					
				}

				workers[thr] = thread(&add_single_col, rhsptr, &(arr[thr]), jump, load_col_left);
				thr++;
				

				rhsptr -= jump * load_col * (load - 1);
				rhsptr++;
						
			}


			load = num_thr / res_shape_prod;
			for (int k = 0; k < num_thr; k++)
			{
				workers[k].join();
				if (k % load == 0 and k != 0 and k <= col*load){
					int a = k - load;
					*lhsptr = (T)0;
					for(int j = 0; j < load; j++){
						*lhsptr += arr[a+j];
					}
					lhsptr++;
					
				}

				if(k == num_thr-1){
					load += load_left;
					int a = k - load + 1;
					*lhsptr = (T)0;
					for (int j = 0; j < load; j++)
					{
						*lhsptr += arr[a + j];
					}
					lhsptr++;
				}


			}
		}
		return *res;
	}

	static void sum_axis0_thr(T *rhs, T *res, int num_elem)
	{
		*res = *rhs;
		rhs++;
		for (int i = 1; i < num_elem; i++)
		{
			*res += *rhs;
			rhs++;
		}
	}

	static void add_single_thr(T *inp, T *res, int load, int m, int jump, int pos, int pos_jmp)
{
		for (int i = 0; i < load; i++)
		{
			T temp = *inp;
			inp += jump;
			for (int j = 1; j < m; j++)
			{
				temp += *inp;
				inp += jump;
			}
			
			pos++;
			inp -= jump * m;
			if (pos >= jump){
				inp += (pos_jmp);
				pos = pos % jump;
			}
			else{
				inp++;
			}
			*res = temp;
			res++;
		}
	}

	gen_arr sum(int num_thr, int axis = -1,  string mean = "no mean"){

		int dim = this->ndim;

		assert(axis < dim);
		assert(axis >= -1 );


		if(axis == -1){

			int num_elem = this->arr_len / num_thr;
			int num_left = arr_len - num_elem * num_thr;
			int num_thrs = num_left > 0 ? num_thr + 1 : num_thr;

			vector<unsigned int> res_shape;

			thread workers[num_thrs];
			T *rhsptr, *res, *ans;
			rhsptr = this->arr.get() + this->offset;

			res_shape.push_back(1);
			res_shape.push_back(num_thrs);
			gen_arr *res_arr = new gen_arr(res_shape);

			vector<unsigned int> ans_shape;
			ans_shape.push_back(1);
			gen_arr *ans_arr = new gen_arr(ans_shape);

			res = res_arr->arr.get();
			ans = ans_arr->arr.get();
			for (int i = 0; i < num_thr; i++)
			{
				workers[i] = thread(&sum_axis0_thr, rhsptr, res, num_elem);
				rhsptr += num_elem;
				res++;
			}

			if (num_left > 0)
			{
				workers[num_thr] = thread(&sum_axis0_thr, rhsptr, res, num_left);
				res++;
			}

			res -= num_thrs;

			for (int i = 0; i < num_thrs; i++)
			{
				workers[i].join();
				if(i == 0)
					*ans = *res;
				else
					*ans += *res;

				res++;
			}

			if(mean == "with mean"){
				*ans /= this->arr_len;
			}

			return *ans_arr;
		}
		else{
			vector<unsigned int> res_shape;
			vector<unsigned int> temp = this->shape;

			int m ;
			int sz = temp.size();		
			for(int i = 0; i < sz; i++){
				if(i==axis){
					m=temp.front();
				}
				else{
					res_shape.push_back(temp.front());
				}
				temp.erase(temp.begin());	
			}

			gen_arr<T> *res = new gen_arr(res_shape);

			int num_elem = this->arr_len;

			T *lhsptr, *rhsptr;
			lhsptr = res->arr.get();
			rhsptr = this->arr.get() + this->offset;

			int res_shape_prod = prod_elems_in_vector(res->shape);
			int load = res_shape_prod / num_thr;
			int load_left = res_shape_prod - (load * num_thr);

			temp = this->shape;
			
			for(int i = 0; i <= axis; i++){
				temp.erase(temp.begin());
			}
			long long int jump = prod_elems_in_vector(temp);

			if (load > 0)
			{	
				int num_thrs = load_left > 0 ? num_thr + 1 : num_thr;
				thread workers[num_thrs];
				int pos = 0;
				int pos_jmp = (m-1)*jump +1 ;
				for (int i = 0; i < num_thr; i++)
				{
					workers[i] = thread(&add_single_thr, rhsptr, lhsptr, load, m, jump, pos, pos_jmp);
					pos+=load;
					lhsptr += load;
					if(pos >= jump){
						int ps_jp = pos/jump;
						int jp = load - ps_jp;
						pos = pos %jump;

						rhsptr += (pos_jmp*ps_jp);
						rhsptr += jp;
					}
					else{
						rhsptr += load;
					}
				}

				if (load_left > 0)
				{
					workers[num_thr] = thread(&add_single_thr, rhsptr, lhsptr, load_left, m, jump, pos, pos_jmp);
				}

				for (int i = 0; i < num_thrs; i++)
				{
					workers[i].join();
				}
			}

			else
			{
				cout<<"enter"<<endl;
				load = num_thr / res_shape_prod;
				load_left = num_thr - load * res_shape_prod;
				int col = load_left == 0 ? res_shape_prod : res_shape_prod - 1;

				thread workers[num_thr];

				T arr[num_thr];

				int thr = 0;

				int pos = 0;
				int pos_jmp = (m-1)*jump + 1;

				for (int j = 0; j < col; j++)
				{
					int load_col = m / load;
					int load_col_left = m - load_col * (load - 1);

					for (int i = 0; i < load - 1; i++)
					{
						workers[thr] = thread(&add_single_col, rhsptr, &(arr[thr]), jump, load_col);
						rhsptr += jump * load_col;
						thr++;
					}

					workers[thr] = thread(&add_single_col, rhsptr, &(arr[thr]), jump, load_col_left);
					thr++;

					rhsptr -= jump * load_col * (load - 1);
					pos++;
					if(pos >= jump){
						rhsptr+= pos_jmp;
						pos = pos%jump;
					}
					else{
						rhsptr++;
					}
				}
				if (load_left > 0)
				{
					load += load_left;
					int load_col = m / load;
					int load_col_left = m - load_col * (load - 1);
					cout<<load_col_left<<endl<<load_col<<endl<<load<<endl;

					for (int i = 0; i < load - 1; i++)
					{
						workers[thr] = thread(&add_single_col, rhsptr, &(arr[thr]), jump, load_col);
						rhsptr += jump * load_col;
						thr++;
					}

					workers[thr] = thread(&add_single_col, rhsptr, &(arr[thr]), jump, load_col_left);
				}

				load = num_thr / res_shape_prod;
				for (int k = 0; k < num_thr; k++)
				{
					workers[k].join();
					if (k % load == 0 and k != 0 and k <= col * load)
					{
						int a = k - load;
						*lhsptr = (T) 0;
						for (int j = 0; j < load; j++)
						{
							*lhsptr += arr[a + j];
						}
						if(mean == "with mean"){
							*lhsptr /= m;
						}
						lhsptr++;
					}

					if (k == num_thr - 1)
					{
						load += load_left;
						int a = k - load + 1;
						*lhsptr = (T)0;
						for (int j = 0; j < load; j++)
						{
							*lhsptr += arr[a + j];
						}
						if (mean == "with mean")
						{
							*lhsptr /= m;
						}
						lhsptr++;
					}
				}
			}
			return *res;
		}
	}

	gen_arr mean(int num_thr, int axis = -1)
	{
		assert(axis >= -1);
		assert(axis < this->ndim);

		gen_arr res = this->sum(num_thr, axis, "with mean");

		return res;
	}
	/* gen_arr multi_threaded_add(gen_arr<T>  & rhs, int num_thr){
		assert(this->shape == rhs.shape);
		gen_arr<T> res(this->shape);
		int num_elem = this->arr_len;
		int load_per_thr = num_elem / num_thr;
		int num_left = num_elem - load_per_thr * num_thr;
		int num_thrs = num_left > 0 ? num_thr + 1 : num_thr;
		printf("num_elem:%d;load_per_thr:%d;num_left:%d;num_thr:%d,num_thrs:%d\n", num_elem, load_per_thr, num_left, num_thr, num_thrs);
		thread workers[num_thrs];
		// gen_arr<T> res(this->shape);
		T *lhsptr, *rhs0ptr, *rhs1ptr;
		lhsptr = res.arr.get();
		rhs0ptr = this->arr.get()+this->offset;
		rhs1ptr = rhs.arr.get()+rhs.offset;
		for (int i = 0; i < num_thr; i++){
			workers[i] = thread(&adh<T>, lhsptr, rhs0ptr, rhs1ptr, load_per_thr);
			// workers[i] = thread(&gen_arr<T>::add_helper, lhsptr, rhs0ptr, rhs1ptr, load_per_thr);
			lhsptr+= load_per_thr;
			rhs0ptr += load_per_thr;
			rhs1ptr += load_per_thr;
		}
		if (num_left > 0){
			workers[num_thr] = thread(&adh<T>, lhsptr, rhs0ptr, rhs1ptr, num_left);
			// workers[num_thr] = thread(&gen_arr<T>::add_helper, lhsptr, rhs0ptr, rhs1ptr, num_left);
		}
		for (int i = 0; i < num_thrs; i++){
			workers[i].join();
		}
		return res;
	} */
	gen_arr add(gen_arr<T> const &rhs){
		assert(this->shape == rhs.shape);
		gen_arr<T> res(this->shape);
		int ilhs = 0, irhs0 = this->offset, irhs1 = rhs.offset;
		T *ptrlhs = res.arr.get(), *ptrrhs0 = this->arr.get(), *ptrrhs1 = rhs.arr.get();
		for (; ilhs < this->arr_len; ilhs++, irhs0++, irhs1++){
			ptrlhs[ilhs] = ptrrhs0[irhs0] + ptrrhs1[irhs1];
			// res.arr.get()[ilhs] = this->arr.get()[irhs0] + rhs.arr.get()[irhs1];
		}
		return res;
	}
	gen_arr sub(gen_arr<T> const &rhs){
		assert(this->shape == rhs.shape);
		gen_arr<T> res(this->shape);
		int ilhs = 0, irhs0 = this->offset, irhs1 = rhs.offset;
		T *ptrlhs = res.arr.get(), *ptrrhs0 = this->arr.get(), *ptrrhs1 = rhs.arr.get();
		for (; ilhs < this->arr_len; ilhs++, irhs0++, irhs1++){
			ptrlhs[ilhs] = ptrrhs0[irhs0] - ptrrhs1[irhs1];
			// res.arr.get()[ilhs] = this->arr.get()[irhs0] + rhs.arr.get()[irhs1];
		}
		return res;
	}
	gen_arr mul(gen_arr<T> const &rhs){
		assert(this->shape == rhs.shape);
		gen_arr<T> res(this->shape);
		int ilhs = 0, irhs0 = this->offset, irhs1 = rhs.offset;
		T *ptrlhs = res.arr.get(), *ptrrhs0 = this->arr.get(), *ptrrhs1 = rhs.arr.get();
		for (; ilhs < this->arr_len; ilhs++, irhs0++, irhs1++){
			ptrlhs[ilhs] = ptrrhs0[irhs0] * ptrrhs1[irhs1];
			// res.arr.get()[ilhs] = this->arr.get()[irhs0] + rhs.arr.get()[irhs1];
		}
		return res;
	}
	gen_arr div(gen_arr<T> const &rhs){
		assert(this->shape == rhs.shape);
		gen_arr<T> res(this->shape);
		int ilhs = 0, irhs0 = this->offset, irhs1 = rhs.offset;
		T *ptrlhs = res.arr.get(), *ptrrhs0 = this->arr.get(), *ptrrhs1 = rhs.arr.get();
		for (; ilhs < this->arr_len; ilhs++, irhs0++, irhs1++){
			ptrlhs[ilhs] = ptrrhs0[irhs0] / ptrrhs1[irhs1];
			// res.arr.get()[ilhs] = this->arr.get()[irhs0] + rhs.arr.get()[irhs1];
		}
		return res;
	}
	gen_arr  operator+(gen_arr const &rhs){
        return this->add(rhs);
    }
	gen_arr  operator-(gen_arr const &rhs){
        return this->sub(rhs);
    }
	gen_arr  operator*(gen_arr const &rhs){
        return this->mul(rhs);
    }
	gen_arr  operator/(gen_arr const &rhs){
        return this->div(rhs);
    }
	static void transpose_helper(T*lhsptr, T*rhsptr, int ts0, int ts1, int outnum){
		for (int i = 0; i < outnum; i++){
			for (int j = 0; j < ts1; j++){
				*lhsptr = *rhsptr;
				lhsptr += ts0;
				rhsptr++;
			}
			lhsptr -= ts0 * ts1 - 1;
		}
	}
	gen_arr & transpose_mt(int num_thr){
		assert(this->ndim == 2);
		assert(num_thr > 0);
		gen_arr<T> *rest = new gen_arr<T>;
		gen_arr<T> &res = *rest; // actual copying doesnt happen
		gen_arr<T> &me = *this;
		vector<unsigned int> newshape = shape;
		newshape[0] = this->shape[1];
		newshape[1] = this->shape[0];
		res.init(newshape, true);
		res.cumulative_shape = ret_cum_shape(res.shape);
		T *lhsptr = res.arr.get() + res.offset, *rhs0ptr = this->arr.get() + this->offset;
		int num_col = this->shape[0];
		int load_per_thr = num_col / num_thr;
		int num_left = num_col - load_per_thr * num_thr;
		int num_thrs = num_left > 0 ? num_thr + 1 : num_thr;
		// printf("num_col:%d;load_per_thr:%d;num_left:%d;num_thr:%d,num_thrs:%d\n", num_col, load_per_thr, num_left, num_thr, num_thrs);
		thread workers[num_thrs];
		int ts0 = this->shape[0];
		int ts1 = this->shape[1];
		for (int i = 0; i < num_thr; i++){
			workers[i] = thread(&gen_arr<T>::transpose_helper, lhsptr, rhs0ptr, ts0, ts1, load_per_thr);
			// workers[i] = thread(&gen_arr<T>::add_helper, lhsptr, rhs0ptr, rhs1ptr, load_per_thr);
			lhsptr += load_per_thr;
			rhs0ptr += load_per_thr*ts1;
		}
		if (num_left > 0){
			workers[num_thr] = thread(&gen_arr<T>::transpose_helper, lhsptr, rhs0ptr, ts0, ts1, num_left);
			lhsptr += num_left; // not needed, just for completeness
			rhs0ptr += num_left * ts1;
			// workers[num_thr] = thread(&gen_arr<T>::add_helper, lhsptr, rhs0ptr, rhs1ptr, num_left);
		}
		for (int i = 0; i < num_thrs; i++){
			workers[i].join();
		}
		return res;
	}
	gen_arr & transpose(){
		assert(this->ndim == 2);
		gen_arr<T> *rest = new gen_arr<T>;
		gen_arr<T> &res = *rest; // actual copying doesnt happen
		gen_arr<T> &me = *this;
		vector<unsigned int> newshape = shape;
		newshape[0] = this->shape[1];
		newshape[1] = this->shape[0];
		res.init(newshape, true);
		res.cumulative_shape = ret_cum_shape(res.shape);
		T *lhsptr = res.arr.get()+res.offset, *rhsptr = this->arr.get()+this->offset;
		register int ts0 = this->shape[0];
		register int ts1 = this->shape[1];
		for (int i = 0; i < ts0; i++){
			for (int j = 0; j < ts1; j++){
				*lhsptr = *rhsptr;
				lhsptr += ts0;
				rhsptr++;
				// res[j].getval(i) = *rhsptr;  // this also works but is slower
				// res[j][i].getval() = me[i][j].getval(); // this works but is much slower
			}
			lhsptr -= ts0*ts1 - 1;
		}
		return res;
	}
	void reshape(vector<unsigned int> newshape){
		assert(validate_vec(newshape));
		vector<unsigned long long int> newcumshape = ret_cum_shape(newshape);
		assert(this->arr_len == newcumshape[0]*newshape[0]);
		this->shape = newshape;
		this->cumulative_shape = newcumshape;
		this->ndim = this->shape.size();
		return;
	}
	gen_arr & operator=(gen_arr const & rhs){

		int prev_arr_len = this->arr_len;
		bool isparent = sizeof(this->arr)/sizeof(T) == this->arr_len;
		if(prev_arr_len != 0 || !isparent){
			assert(this->shape == rhs.shape);
			memcpy(this->arr.get()+ this->offset, rhs.arr.get() + rhs.offset, this->arr_len * sizeof(T));
			return * this;
		}
		this->shape = rhs.shape;
		this->cumulative_shape = rhs.cumulative_shape;
		this->ndim = rhs.ndim;
		this->arr_len = rhs.arr_len;
		this->arr = shared_ptr<T>(new T[arr_len], std::default_delete<T>());
		memcpy(this->arr.get()+ this->offset, rhs.arr.get() + rhs.offset, this->arr_len * sizeof(T));
		this->offset = 0;
		return *this;

		//new	
	}
	gen_arr& operator[](int index){ // see getval
		assert(index >= 0);
        if (this->ndim > 0){
			vector<unsigned int> new_shape(this->shape.begin()+1, this->shape.end());
			T temp;
			gen_arr<T> *res = new gen_arr<T>;
			res->init(new_shape, false);
			res->arr = this->arr;
			res->arr_len = this->cumulative_shape[0];
			int new_offset = this->offset + this->cumulative_shape[0]*index;
			res->offset = new_offset;
			return *res;
		}
		else{
			printf("this->ndim should be a positive int\n");
			assert(this->ndim > 0);
		}
    }
	T & getval(int index){
		assert(index >= 0);
		assert(this->arr_len > index);
		assert(this->ndim == 1);
		return this->arr.get()[index+this->offset];
	}
	T & getval(){
		assert(this->ndim == 0);
		return this->arr.get()[this->offset];
	}
	gen_arr & matmul(gen_arr<T> & rhs){
		assert(this->ndim == 2 && rhs.ndim == 2);
		int m1 = this->shape[0], n1 = this->shape[1];
		int m2 = rhs.shape[0], n2 = rhs.shape[1];
		assert(n1 == m2);
		vector<unsigned int> new_shape;
		new_shape.push_back(m1); new_shape.push_back(n2);
		gen_arr<T> *ress = new gen_arr<T>;
		ress->init(new_shape, true);
		gen_arr<T> &res = *ress;
		// printf("matmul: res defined\n");
		int sum;
		int rhs0off = this->offset, rhs1off = rhs.offset;
		int rhs0sh0 = this->cumulative_shape[0], rhs1sh0 = rhs.cumulative_shape[0];
		// cout << "rhs.cumulative_shape = " << vec_to_string(rhs.cumulative_shape) << endl;
		// cout << "rhs.shape = " << vec_to_string(rhs.shape) << endl;
		T *ptrlhs = res.arr.get(), *ptrrhs0 = this->arr.get(), *ptrrhs1 = rhs.arr.get();
		ptrlhs = ptrlhs + res.offset;
		for (int ii = 0; ii < m1; ii++){
			for(int jj = 0; jj < n2; jj++){
				register T sum = 0;
				for (int kk = 0; kk < n1; kk++){
					sum += ptrrhs0[rhs0off+ii*rhs0sh0+kk]*ptrrhs1[rhs1off+kk*rhs1sh0+jj];
				}
				*ptrlhs = sum;
				ptrlhs++;
			}
		}
		return res;
	}
	gen_arr & matmul4(gen_arr<T> &rhs){ // 3X faster than matmul
		assert(this->ndim == 2 && rhs.ndim == 2);
		int m1 = this->shape[0], n1 = this->shape[1];
		int m2 = rhs.shape[0], n2 = rhs.shape[1];
		assert(n1 == m2);
		vector<unsigned int> new_shape;
		new_shape.push_back(m1); new_shape.push_back(n2);
		gen_arr<T> *ress = new gen_arr<T>;
		ress->init(new_shape, true);
		gen_arr<T> &res = *ress;
		// gen_arr<T> rhstpose = rhs.transpose_mt(4);
		gen_arr<T> rhstpose = rhs.transpose();
		int rhs0off = this->offset, rhs1off = rhstpose.offset;
		int rhs0sh0 = this->cumulative_shape[0], rhs1sh0 = rhstpose.cumulative_shape[0];
		T *ptrlhs = res.arr.get(), *ptrrhs0 = this->arr.get(), *ptrrhs1 = rhstpose.arr.get();
		ptrlhs = ptrlhs + res.offset;
		T * ptrrhs2 = ptrrhs1;
		for (int ii = 0; ii < m1; ii++){
			ptrrhs1 = ptrrhs2;
			for (int jj=0; jj < n2; jj++){
				register T sum = 0;
				for (int kk = 0; kk < n1; kk++){
					sum += ptrrhs0[rhs0off+ii*rhs0sh0+kk]* ( * ptrrhs1 ++) ;
					// sum += ptrrhs0[rhs0off+ii*rhs0sh0+kk]*ptrrhs1[rhs1off+jj*rhs1sh0+kk];
				}
				* (ptrlhs ++) = sum ;
				// *ptrlhs = sum;
				// ptrlhs++;
			}
		}
		return res;
	}
	static void matmul_helper(T* lhsptr, T* rhs0ptr, T* rhs1ptr, int r0nr, int r0nc, int r1nr){
		// r0nr is number of rows represented in rhs0ptr
		// r0nc is number of cols represented in rhs0ptr
		// r1nr is number of rows represented in rhs1ptr
		for (int ii = 0; ii < r0nr; ii++){
			for (int jj = 0; jj < r1nr; jj++){
				register T sum = 0;
				for (int kk = 0; kk < r0nc; kk++){
					sum += rhs0ptr[ii*r0nc+kk]*rhs1ptr[jj*r0nc+kk];
				}
				*lhsptr = sum;
				lhsptr++;
			}
		}
	}
	gen_arr matmul_mt(gen_arr<T> &rhs, int num_thr){
		assert(this->ndim == 2 && rhs.ndim == 2);
		int m1 = this->shape[0], n1 = this->shape[1];
		int m2 = rhs.shape[0], n2 = rhs.shape[1];
		assert(n1 == m2);
		vector<unsigned int> new_shape;
		new_shape.push_back(m1);
		new_shape.push_back(n2);
		gen_arr<T> *ress = new gen_arr<T>;
		ress->init(new_shape, true);
		gen_arr<T> &res = *ress;
		gen_arr<T> rhstpose = rhs.transpose_mt(num_thr);
		int rhs0off = this->offset, rhs1off = rhstpose.offset;
		int rhs0sh0 = this->cumulative_shape[0], rhs1sh0 = rhstpose.cumulative_shape[0];
		T *ptrlhs = res.arr.get(), *ptrrhs0 = this->arr.get(), *ptrrhs1 = rhstpose.arr.get();
		ptrlhs = ptrlhs + res.offset;

		int num_col = this->shape[0];
		int load_per_thr = num_col / num_thr;
		int num_left = num_col - load_per_thr * num_thr;
		int num_thrs = num_left > 0 ? num_thr + 1 : num_thr;
		// printf("num_col:%d;load_per_thr:%d;num_left:%d;num_thr:%d,num_thrs:%d\n", num_col, load_per_thr, num_left, num_thr, num_thrs);
		thread workers[num_thrs];
		int ts0 = this->shape[0];
		int ts1 = this->shape[1];
		for (int i = 0; i < num_thr; i++){
			// workers[i] = thread(&gen_arr<T>::transpose_helper, lhsptr, rhs0ptr, ts0, ts1, load_per_thr);
			workers[i] = thread(&gen_arr<T>::matmul_helper, ptrlhs, ptrrhs0, ptrrhs1, load_per_thr, n1, n2);
			ptrlhs += load_per_thr*n2;
			ptrrhs0 += load_per_thr*n1;
		}
		if (num_left > 0){
			workers[num_thr] = thread(&gen_arr<T>::matmul_helper, ptrlhs, ptrrhs0, ptrrhs1, num_left, n1, n2);
			// workers[num_thr] = thread(&gen_arr<T>::transpose_helper, lhsptr, rhs0ptr, ts0, ts1, num_left);
		}
		for (int i = 0; i < num_thrs; i++){
			workers[i].join();
		}
		return res;
	}

	gen_arr matmul2(gen_arr<T> &rhs){
		assert(this->ndim == 2 && rhs.ndim == 2);
		int m1 = this->shape[0], n1 = this->shape[1];
		int m2 = rhs.shape[0], n2 = rhs.shape[1];
		assert(n1 == m2);
		vector<unsigned int> new_shape;
		new_shape.push_back(m1);
		new_shape.push_back(n2);
		gen_arr<T> res(new_shape, 0, false, true);
		// printf("matmul: res defined\n");
		int sum;
		int rhs0off = this->offset, rhs1off = rhs.offset;
		int rhs0sh0 = this->cumulative_shape[0], rhs1sh0 = rhs.cumulative_shape[0];
		T *ptrlhs = res.arr.get(), *ptrrhs0 = this->arr.get(), *ptrrhs1 = rhs.arr.get();
		ptrlhs = ptrlhs + res.offset;
		for (int ii = 0; ii < m1; ii++){	
			int iim2 = ii* m2;
			T t = ptrrhs0[rhs0off + ii * rhs0sh0] ;
			for (int jj = 0;  jj < n2 ; jj++)
				*(ptrlhs + ii* n2 + jj) = t * ptrrhs1[rhs1off + jj];
			for (int kk = 1; kk < n1; kk++)
			{
				for (int jj = 0; jj < n2; jj++)
				*(ptrlhs + iim2 + jj) += ptrrhs0[rhs0off + ii * rhs0sh0 + kk] * ptrrhs1[rhs1off + kk * rhs1sh0 + jj];
				}
		}
		return res;
	}

	gen_arr matmul3(gen_arr<T> &rhs)
	{
		assert(this->ndim == 2 && rhs.ndim == 2);
		int m1 = this->shape[0], n1 = this->shape[1];
		int m2 = rhs.shape[0], n2 = rhs.shape[1];
		assert(n1 == m2);
		vector<unsigned int> new_shape;
		new_shape.push_back(m1);
		new_shape.push_back(n2);
		gen_arr<T> res(new_shape, 0, false, true);
		// printf("matmul: res defined\n");
		int sum;
		int rhs0off = this->offset, rhs1off = rhs.offset;
		int rhs0sh0 = this->cumulative_shape[0], rhs1sh0 = rhs.cumulative_shape[0];
		T *ptrlhs = res.arr.get(), *ptrrhs0 = this->arr.get(), *ptrrhs1 = rhs.arr.get();
		ptrlhs = ptrlhs + res.offset;

		T * Bcolj = new T[n1];
		for (int jj = 0; jj < n2; jj++)
		{
			for (int kk = 0; kk < n1; kk++)
				Bcolj[kk] = ptrrhs1[rhs1off + kk * rhs1sh0 + jj];

			for (int ii = 0; ii < m1; ii++){
					register T s = 0;
					for (int kk = 0; kk < n1; kk++)
						s += ptrrhs0[rhs0off + ii * rhs0sh0 + kk] * Bcolj[kk];
					ptrlhs[ii* m1 + jj] = s;
			}
		}

		return res;
	}

	static void conv_helper(int start, int end, T* ptrlhs, T* ptrrhs0, T* ptrrhs1, 
		vector<unsigned int>lhs_shape, vector<unsigned int>rhs0_shape, vector<unsigned int>rhs1_shape, 
		vector<unsigned long long int>lhs_cush, vector<unsigned long long int>rhs0_cush, vector<unsigned long long int>rhs1_cush, 
		int lhsoff, int rhs0off, int rhs1off, int lhs_len, int rhs0_len, int rhs1_len, int ndim)
	{
		// cout<<"conv_helper called"<<endl;
		if(rhs0_len < rhs1_len){
			int lhs_i[ndim], rhs1_i[ndim], rhs1_i1[ndim];
			
			for (int i = start; i < end; ++i)
			{
				int indx = i;
				for (int l = 0; l < ndim; ++l)
				{
					lhs_i[l] = indx/lhs_cush[l];
					indx = indx % lhs_cush[l];
					rhs1_i[l] = lhs_i[l] - ((rhs0_shape[l]-1));
					rhs1_i1[l] = rhs1_i[l];
				}
				// for(ii = 0; ii<ndim; ii++){

				// }
				for (int j = 0; j < rhs0_len; ++j)
				{
					if(j != 0){
						// int ll;
						int state = 0;
						for (int ll = 0; ll < ndim; ++ll)
						{
							// cout<<"rhs1_cush[ll] = "<<rhs1_cush[ll]<<endl;
							if(state == 0){
								if(j % rhs0_cush[ll] == 0){
									rhs1_i[ll] += 1;
									// break;
									state = 1;
								}
							}
							else if(state == 1){
								rhs1_i[ll] = rhs1_i1[ll];
							}
						}
					}
					int kk = 0;
					int k = 0;
					for (kk = 0; kk < ndim; ++kk)
					{
						// lhs_i[l] = indx/lhs_cush[l];
						// indx = indx % lhs_cush[l];
						// rhs1_i[l] = lhs_i[l] - ((rhs0_shape[l]-1));
						if(rhs1_i[kk] < 0 || rhs1_i[kk] >= rhs1_shape[kk]){
							break;
						}
						k += rhs1_i[kk]*rhs1_cush[kk];
					}

					// for(int k =k_start; k < rh1_len; ++k){
					
					if(kk >= ndim){
						*(ptrlhs+lhsoff+i) += ptrrhs1[rhs1off+k]*ptrrhs0[rhs0off+j];
					}
					// k++;
					// }
				}
			}
		}
		else if(rhs0_len >= rhs1_len){
			// cout<<"here11111"<<endl;
			int lhs_i[ndim], rhs0_i[ndim], rhs0_i1[ndim];
			
			for (int i = start; i < end; ++i)
			{
				int indx = i;
				for (int l = 0; l < ndim; ++l)
				{
					lhs_i[l] = indx/lhs_cush[l];
					indx = indx % lhs_cush[l];
					rhs0_i[l] = lhs_i[l] - (rhs1_shape[l]-1);
					rhs0_i1[l] = rhs0_i[l];
					// k += rhs1_i[l]*rhs1_cush[l];
				}
				// cout<<rhs0_i[0]<<" "<<rhs0_i[1]<<endl;
				// for(ii = 0; ii<ndim; ii++){

				// }
				for (int j = 0; j < rhs1_len; ++j)
				{
					if(j != 0){
						int state = 0;
						for (int ll = 0; ll < ndim; ++ll)
						{
							// cout<<"rhs1_cush[ll] = "<<rhs1_cush[ll]<<endl;
							if(state == 0){
								if(j % rhs1_cush[ll] == 0){
									rhs0_i[ll] += 1;
									// break;
									state = 1;
								}
							}
							else if(state == 1){
								rhs0_i[ll] = rhs0_i1[ll];
							}
						}
					}
					int kk = 0;
					int k = 0;
					for (kk = 0; kk < ndim; ++kk)
					{
						// lhs_i[l] = indx/lhs_cush[l];
						// indx = indx % lhs_cush[l];
						// rhs1_i[l] = lhs_i[l] - ((rhs0_shape[l]-1));
						if(rhs0_i[kk] < 0 || rhs0_i[kk] >= rhs0_shape[kk]){
							break;
						}
						k += rhs0_i[kk]*rhs0_cush[kk];
					}

					// for(int k =k_start; k < rh1_len; ++k){
					// cout<<rhs0_i[0]<<" "<<rhs0_i[1]<<endl;
					// cout<<"kk = "<<kk<<endl;
					if(kk >= ndim){
						// cout<<"i = "<<i<<endl;
						*(ptrlhs+lhsoff+i) += ptrrhs0[rhs0off+k]*ptrrhs1[rhs1off+j];
					}
					// k++;
					// }
				}
			}
		}
	}

	gen_arr multi_conv(gen_arr<T> & rhs, int num_thr)
	{
		int rhs0_ndim = this->ndim;
		int rhs1_ndim = rhs.ndim;
		assert(rhs0_ndim == rhs1_ndim);
		vector<unsigned int> new_shape;
		vector<unsigned int> rhs0_shape = this->shape;
		vector<unsigned int> rhs1_shape = rhs.shape;
		for (int i = 0; i < rhs0_ndim; ++i)
		{
			new_shape.push_back(rhs0_shape[i] + rhs1_shape[i] - 1);
		}
		gen_arr<T> res(new_shape, 0, true, true);
		// printf("conv: res defined\n");
		// int sum;
		int rhs0off = this->offset, rhs1off = rhs.offset, lhsoff = res.offset;
		vector<unsigned long long int> rhs0_cush = this->cumulative_shape, rhs1_cush = rhs.cumulative_shape, lhs_cush = res.cumulative_shape;
		// cout << "rhs.cumulative_shape = " << vec_to_string(rhs.cumulative_shape) << endl;
		// cout << "rhs.shape = " << vec_to_string(rhs.shape) << endl;
		T *ptrlhs = res.arr.get(), *ptrrhs0 = this->arr.get(), *ptrrhs1 = rhs.arr.get();
		// ptrlhs = ptrlhs + res.offset;
		int rhs1_len = rhs.arr_len;
		int rhs0_len = this->arr_len;
		int lhs_len = res.arr_len;

		// int num_col = this->shape[0];
		int load_per_thr = lhs_len / num_thr;
		int num_left = lhs_len - load_per_thr * num_thr;
		int num_thrs = num_left > 0 ? num_thr + 1 : num_thr;
		// printf("num_col:%d;load_per_thr:%d;num_left:%d;num_thr:%d,num_thrs:%d\n", num_col, load_per_thr, num_left, num_thr, num_thrs);
		thread workers[num_thrs];
		// int ts0 = this->shape[0];
		// int ts1 = this->shape[1];
		int start = 0;
		int end = 0;
		for (int i = 0; i < num_thr; i++){
			// workers[i] = thread(&gen_arr<T>::transpose_helper, lhsptr, rhs0ptr, ts0, ts1, load_per_thr);
			end += load_per_thr;
			workers[i] = thread(&gen_arr<T>::conv_helper, start, end, ptrlhs, ptrrhs0, ptrrhs1, 
				new_shape, rhs0_shape, rhs1_shape, lhs_cush, rhs0_cush, rhs1_cush, lhsoff, rhs0off, rhs1off,
				lhs_len, rhs0_len, rhs1_len, rhs0_ndim);
			start = end;
		}
		if (num_left > 0){
			end += num_left;
			workers[num_thr] = thread(&gen_arr<T>::conv_helper, start, end, ptrlhs, ptrrhs0, ptrrhs1, 
				new_shape, rhs0_shape, rhs1_shape, lhs_cush, rhs0_cush, rhs1_cush, lhsoff, rhs0off, rhs1off,
				lhs_len, rhs0_len, rhs1_len, rhs0_ndim);
			// workers[num_thr] = thread(&gen_arr<T>::transpose_helper, lhsptr, rhs0ptr, ts0, ts1, num_left);
		}
		for (int i = 0; i < num_thrs; i++){
			workers[i].join();
		}
		return res;	
	}

	gen_arr conv(const gen_arr<T> & rhs){
		int rhs0_ndim = this->ndim;
		int rhs1_ndim = rhs.ndim;
		assert(rhs0_ndim == rhs1_ndim);
		vector<unsigned int> new_shape;
		vector<unsigned int> rhs0_shape = this->shape;
		vector<unsigned int> rhs1_shape = rhs.shape;
		// int m0 = this->shape[0], n0 = this->shape[1];
		// int m1 = rhs.shape[0], n1 = rhs.shape[1];
		// assert(n1 == m2);
		// new_shape.push_back(m1); new_shape.push_back(n2);
		// vector<unsigned int> new_shape;
		for (int i = 0; i < rhs0_ndim; ++i)
		{
			new_shape.push_back(rhs0_shape[i] + rhs1_shape[i] - 1);
		}
		gen_arr<T> res(new_shape, 0, true, true);
		// printf("conv: res defined\n");
		// int sum;
		int rhs0off = this->offset, rhs1off = rhs.offset;
		vector<unsigned long long int> rhs0_cush = this->cumulative_shape, rhs1_cush = rhs.cumulative_shape, lhs_cush = res.cumulative_shape;
		// cout << "rhs.cumulative_shape = " << vec_to_string(rhs.cumulative_shape) << endl;
		// cout << "rhs.shape = " << vec_to_string(rhs.shape) << endl;
		T *ptrlhs = res.arr.get(), *ptrrhs0 = this->arr.get(), *ptrrhs1 = rhs.arr.get();
		// ptrlhs = ptrlhs + res.offset;
		int rhs1_len = rhs.arr_len;
		int rhs0_len = this->arr_len;
		// for (int rhs0_i = 0; rhs0_i < count; ++rhs0_i)
		// {
		// 	/* code */
		// }
		for (int rhs1_indx = 0; rhs1_indx < rhs1_len; ++rhs1_indx)
		{
			// int temp1 = *(ptrrhs1+rhs1off+rhs1_indx);
			// int temp1 = ptrrhs1[rhs1off+rhs1_indx];
			for (int rhs0_indx = 0; rhs0_indx < rhs0_len; ++rhs0_indx)
			{
				// int temp2 = *(ptrrhs0+rhs0off+rhs0_indx);
				// int temp2 = ptrrhs0[rhs0off+rhs0_indx];
				// int temp3 = temp1*temp2;
				// int offset = (rhs1_len-i-1)+j;	// for 1-D
				int offset = 0;	// for N-D
				int rhs0_j = rhs0_indx;
				int rhs1_j = rhs1_indx;
				for (int k = 0; k < rhs0_ndim; ++k)
				{
					int rhs0_i = rhs0_j/rhs0_cush[k];
					int rhs1_i = rhs1_j/rhs1_cush[k];
					rhs0_j = rhs0_j % rhs0_cush[k];
					rhs1_j = rhs1_j % rhs1_cush[k];
					int lhs_i = (rhs1_shape[k] - rhs1_i - 1) + rhs0_i;
					offset += lhs_i*lhs_cush[k];
				}
				ptrlhs[offset] += ptrrhs1[rhs1off+rhs1_indx]*ptrrhs0[rhs0off+rhs0_indx];
			}
		}
		return res;
	}

	string str(){
		cout << "str entered\n";
		auto prod_vec = prods(shape);
		// vector<unsigned long long int> prod_vec = ret_cum_shape(shape);
		// reverse(prod_vec.begin(), prod_vec.end());
		int dimns = shape.size();
		string res = "";
		for (int i = 0; i < prod_vec[dimns-1]; ++i){
			int count = 0;
			for (int j = 0; j < dimns; ++j){
				if(i % prod_vec[j] == 0){
					count++;
				}
				else{
					break;
				}
			}
			if(count){
				for (int j = 0; j < dimns; ++j){
					if(j >= dimns-count){
						res += "[";
					}
					else{
						res += " ";
					}
				}
			}
			res += to_string(this->arr.get()[i+ this->offset]);
			count = 0;
			for (int j = 0; j < dimns; ++j){
				if((i+1) % prod_vec[j] == 0){
					count++;
					res += "]";
				}
				else{
					break;
				}
			}
			if(count){
				res += "\n";
			}
			else{
				res+= " ";
			}
		}
		return res;
	}
	string dummy_dump(){
		string res = "[";
		int ulim = this->offset + this->arr_len;
		int* ptrarr = this->arr.get();
		for (int i = this->offset; i < ulim; i++){
			res += to_string(ptrarr[i]) + ",";
		}
		res[res.size()-1] = ']';
		return res;
	}
	vector<unsigned int> get_shape(){
		return this->shape;
	}
	gen_arr dft(){
		int ts=this->shape[0];
		gen_arr<complex<double> > w (vector<unsigned int > (1,ts));
		T *ptrw = w.arr.get() ;
		for(int i=0;i<ts;i++){
			*ptrw =0;
			T *ptr_mat = this->arr.get()  + this->offset;
			for(int j=0;j<ts;j++){
				*ptrw += (* ptr_mat++ )*  polar(1.0,-2*PI*i*j/ts);
			}
			*ptrw++;
		}
		return  w;
	}
	vector<T> gen1_to_vector(){
		int size= this->get_shape()[0];
		vector<T> res(size);
		T * ptr = this->arr.get() + this->offset;
		for(int i=0;i<size;i++){
			res[i] = * ptr ++;
		}
		return res;
	}
	gen_arr<T> vector_gen1(vector<T> & v){
		int size = v.size();
		gen_arr< T> res (vector<unsigned int> (1,size));
		T * ptr = res.arr.get();
		for(int i=0;i<size;i++){
			 * ptr ++ = v[i];
		}
		return res;
	}
	gen_arr<T> fft(){
		assert(this->shape.size() == 1  );
		int size = this->shape[0];
		assert(__builtin_popcount(size) == 1);
		vector<T> ans= gen1_to_vector();
		fft2(ans, 0, size);
		return vector_gen1(ans);
	}
	gen_arr dft_2(){
		int s1=this->shape[0];
		int s2=this->shape[1];
		double powe=-2.0*PI;
		gen_arr< T > w(this->shape);
		T *ptrw=w.arr.get();
		double i_by_s1,j_by_s2;
		double sqrt_mn = sqrt( s1*s2*1.0);
		for (int i = 0; i < s1; ++i)
		{
			for (int j = 0; j < s2; ++j)
			{	//w[i][j].getval()=0;
				*ptrw=0;
				i_by_s1=i*1.0/s1;
				j_by_s2=j*1.0/s2;
				T * ptr_mat = this->arr.get() + offset;
				register T sum =0;
				for(int k=0;k<s1;k++){
					for(int l=0;l<s2;l++){
						sum += (* ptr_mat++ ) * polar(1.0,powe*(i_by_s1*k+j_by_s2*l));
					}
				}
				*ptrw++ = sum/sqrt_mn;
			}
		}
		return w;
	}
		gen_arr fft_2d (){
		unsigned int s1=this->shape[0];
		unsigned int s2=this->shape[1];
		double sq=sqrt(s1*s2);
		gen_arr<complex<double> > ans(this->shape);
		gen_arr<complex <double> >  transposed = this->transpose();
		T *ptr_mat = ans.arr.get();
		T *ptr_trans = transposed.arr.get();
		for (int i = 0; i < s2; ++i)
		{
			gen_arr<complex <double> > d =transposed[i].fft();
			memcpy(ptr_trans, d.arr.get(), d.arr_len  * sizeof(T) );
			ptr_trans+=s1;
			// for (int j = 0; j < s1; ++j)
			// {	//cout<<d[j].getval()<<" ";
			// 	// *ptr_trans=d[j].getval();
			// 	// *ptr_trans++;
			// }
		}
		gen_arr<complex <double> >  ntransposed = transposed.transpose();
		for (int i = 0; i < s1; ++i)
		{	
			gen_arr<complex <double> > d =ntransposed[i].fft();
			T * ptr_d = d.arr.get();
			for (int j = 0; j < s2; ++j)
				* ptr_mat++ = * (ptr_d ++)/sq;
		}
		return ans;
		
	}
	
	static void multi_dft_helper(T* ptrw,T* ptr_mat,int start,int end,int ts){
		for(int i=start;i<end;i++){
			T* ptr_mat_1=ptr_mat;
			for(int j=0;j<ts;j++){
					*ptrw += (* ptr_mat_1++ )*  polar(1.0,-2*PI*i*j/ts);
			}
				*ptrw++;
		}
	}

	gen_arr  multi_dft(int n){
		unsigned int ts=this->shape[0];
		gen_arr<T> w (vector<unsigned int > (1,ts));
		T *ptrw = w.arr.get() ;
		
		int n_4=ts/n;
		thread workers[n];
		T *ptr_mat = this->arr.get() + this->offset;
		for (int i = 0; i < n-1; ++i)
		{	
			workers[i]=thread(&gen_arr<T>::multi_dft_helper,ptrw,ptr_mat,i*n_4,i*n_4+n_4,ts);
			ptrw+=n_4;
		}
		// int n_4_1=ts-(n-1)*n_4;
		workers[n-1]=thread(&gen_arr<T>::multi_dft_helper,ptrw,ptr_mat,(n-1)*n_4,ts,ts);
		for (int i = 0; i < n; ++i)
			workers[i].join();
		return  w;
	}

	static void multi_dft2d_helper(T* ptrw,T* ptr_mat,int start,int end,int s1,int s2){
		double powe=-2.0*PI;
		double mn = s1*s2*1.0;
		double i_by_s1,j_by_s2;
		for (int i = start; i < end; ++i)
		{	for (int j = 0; j < s2; ++j)
			{	*ptrw=0;
				i_by_s1=i*1.0/s1;
				j_by_s2=j*1.0/s2;
				// T * ptr_mat = this->arr.get() + offset;
				T* ptr_mat1=ptr_mat;
				for(int k=0;k<s1;k++){
					for(int l=0;l<s2;l++){
						*ptrw+= (* ptr_mat1++ ) *polar(1.0,powe*(i_by_s1*k+j_by_s2*l));
					}
				}
				*ptrw/=sqrt(mn);
				*ptrw++;
			}
		}
	}
	gen_arr multi_dft_2d(int n){
		unsigned int s1=this->shape[0];
		unsigned int s2=this->shape[1];
		gen_arr<T> w (this->shape);
		T *ptrw = w.arr.get() ;
		int n_4=s1/n;
		thread workers[n];
		int ns2=n_4*s2;
		T *ptr_mat = this->arr.get() + this->offset;
		for (int i = 0; i < n-1; ++i)
		{	
			workers[i]=thread(&gen_arr<T>::multi_dft2d_helper,ptrw,ptr_mat,i*n_4,i*n_4+n_4,s1,s2);
			ptrw+=ns2;
		}
		workers[n-1]=thread(&gen_arr<T>::multi_dft2d_helper,ptrw,ptr_mat,(n-1)*n_4,s1,s1,s2);
		for (int i = 0; i < n; ++i)
			workers[i].join();
		return w;
	}
	
	static void multi_fft2d_helper1(gen_arr<T> *transposed, T* ptr_trans,int start, int end,unsigned int s1){
		for (int i = start; i < end; ++i)
		{	gen_arr<T> d =(* transposed)[i].fft();
			memcpy(ptr_trans, d.arr.get(), d.arr_len  * sizeof(T) );
			ptr_trans+=s1;
		}
	}

	static void multi_fft2d_helper2(T* ptr_mat,gen_arr * ntransposed,int start,int end,unsigned int s2,double sq){
		for (int i = start; i < end; ++i)
		{	
			gen_arr<complex <double> > d =(* ntransposed)[i].fft();
			T * ptr_d = d.arr.get();
			for (int j = 0; j < s2; ++j)
			{	
				* ptr_mat++ = * (ptr_d ++)/sq;
			}
		}
	}

	gen_arr multi_fft_2d(int n){
		unsigned int s1=this->shape[0];
		unsigned int s2=this->shape[1];
		double sq=sqrt(s1*s2);
		gen_arr<complex<double> > ans(this->shape);
		gen_arr<complex <double> >  transposed = this->transpose();
		T *ptr_mat = ans.arr.get();
		T *ptr_trans = transposed.arr.get();
	    	unsigned int n_s1=n/s1;
		unsigned int n_s2=n/s2;
		thread workers[n];
		for (int i = 0; i < n-1; ++i)
		{	
			workers[i]=thread(&gen_arr<T>::multi_fft2d_helper1,&transposed,ptr_trans,i*n_s2,i*n_s2+n_s2,s1);
			ptr_trans+=n_s2;
		}
		
		workers[n-1]=thread(&gen_arr<T>::multi_fft2d_helper1,&transposed,ptr_trans,(n-1)*n_s2,s2,s1);
		for(int i=0;i<n;i++)
			workers[i].join();

		gen_arr<complex <double> >  ntransposed = transposed.transpose();
		for (int i = 0; i < n-1; ++i)
		{	
			workers[i]=thread(&gen_arr<T>::multi_fft2d_helper2,ptr_mat,&ntransposed,i*n_s1,i*n_s1+n_s1,s2,sq);
			ptr_mat+=n_s1;
		}
		workers[n-1]=thread(&gen_arr<T>::multi_fft2d_helper2,ptr_mat,&ntransposed,(n-1)*n_s1,s1,s2,sq);
		for(int i=0;i<n;i++)
			workers[i].join();

		return ans;
	}
	
	T element_abs_sum_for_check(){
		register T sum = 0;
		T *ptrw = this->arr.get() + this->offset;
		int size = this->cumulative_shape[0] * this->shape[0];
		for (int i = 0; i < size; i++)
			sum += abs(*ptrw++);
		return sum;
	}
};
// add, add_mt, div, div_mt, matmul, matmul{2..4}, matmul_mt, tpose, tpose_mt
double ourTimer(string fname, unsigned int size, unsigned int num_thr=1){
	vector<unsigned int> shape;
	shape.push_back(size);
	shape.push_back(size);
	auto t_stamp31 = chrono::high_resolution_clock::now();
	gen_arr<int> a3(shape, 9), b3(shape, 2);
	auto t_stamp32 = chrono::high_resolution_clock::now();
	if (fname=="add"){
		gen_arr<int> c3 = a3.add(b3);
	}
	else if (fname=="add_mt"){
		gen_arr<int> c3 = a3.multi_threaded_add(b3, num_thr);
	}
	else if (fname=="div"){
		gen_arr<int> c3 = a3.div(b3);
	}
	else if (fname=="div_mt"){
		gen_arr<int> c3 = a3.multi_threaded_div(b3, num_thr);
	}
	else if (fname=="matmul"){
		gen_arr<int> c3 = a3.matmul(b3);
	}
	else if (fname=="matmul2"){
		gen_arr<int> c3 = a3.matmul2(b3);
	}
	else if (fname=="matmul3"){
		gen_arr<int> c3 = a3.matmul3(b3);
	}
	else if (fname=="matmul4"){
		gen_arr<int> c3 = a3.matmul4(b3);
	}
	else if (fname=="matmul_mt"){
		gen_arr<int> c3 = a3.matmul_mt(b3, num_thr);
	}
	else if (fname=="tpose"){
		gen_arr<int> c3 = a3.transpose();
	}
	else if (fname=="tpose_mt"){
		gen_arr<int> c3 = a3. transpose_mt(num_thr);
	}
	else{
		return -1.0;
	}
	auto t_stamp33 = chrono::high_resolution_clock::now();
	auto alloc_time321 = chrono::duration_cast<chrono::duration<double>>(t_stamp32 - t_stamp31).count();
	auto alloc_time332 = chrono::duration_cast<chrono::duration<double>>(t_stamp33 - t_stamp32).count();
	return alloc_time332;
	// printf("Time(in seconds): Alloc:%f;\tComputation (tpose multi):%f;\n", alloc_time321, alloc_time332);
}

int driver(string querytype){
	cout << "driver: " << querytype << endl;
	unsigned int sizes[4] = {10, 500, 1000, 10000};
	if (querytype == "add" || querytype == "div" || querytype == "tpose"){
		string qmt = querytype + "_mt";
		ofstream file;
		file.open(querytype+".csv", ios::trunc | ios::out);
		for (int i = 0; i < 4; i++){
			file << sizes[i] << ",";
			double t = ourTimer(querytype, sizes[i]);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 2);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 4);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 8);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 16);
			file << t << ",";
			file << "\n";
		}
		file.close();
	}
	else if (querytype == "matmul"){
		string qmt = querytype + "_mt";
		string q2 = querytype + "2", q3 = querytype + "3", q4 = querytype + "4";
		ofstream file;
		file.open(querytype+".csv", ios::trunc | ios::out);
		for (int i = 0; i < 3; i++){
			printf("driver: matmul: size: %d\n", sizes[i]);
			file << sizes[i] << ",";
			double t = ourTimer(querytype, sizes[i]);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 2);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 4);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 4);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 8);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 16);
			file << t << ",";
			t = ourTimer(q2, sizes[i]);
			file << t << ",";
			t = ourTimer(q3, sizes[i]);
			file << t << ",";
			t = ourTimer(q4, sizes[i]);
			file << t << ",";
			file << "\n";
		}
		file.close();
	}

	else if (querytype == "conv"){
		string qmt = querytype + "_mt";
		string q2 = querytype;//, q3 = querytype + "3", q4 = querytype + "4";
		ofstream file;
		file.open(querytype+".csv", ios::trunc | ios::out);
		for (int i = 0; i < 3; i++){
			// printf("driver: matmul: size: %d\n", sizes[i]);
			file << sizes[i] << ",";
			double t = ourTimer(qmt, sizes[i]);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 2);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 4);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 4);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 8);
			file << t << ",";
			t = ourTimer(qmt, sizes[i], 16);
			file << t << ",";
			t = ourTimer(q2, sizes[i]);
			file << t << ",";
			// t = ourTimer(q3, sizes[i]);
			// file << t << ",";
			// t = ourTimer(q4, sizes[i]);
			// file << t << ",";
			file << "\n";
		}
		file.close();
	}
}

int main(){
	// driver("add");
	// driver("div");
	// driver("tpose");
	// driver("matmul");s
	driver("conv");
}

int old_main(int argc, char* argv[]){
	int num_thr = 4;
	if (argc >= 2){
		num_thr = atoi(argv[1]);
	}

	vector<unsigned int> shape0;
	vector<unsigned int> shape1;
	shape0.push_back(60);
	shape0.push_back(60);
	shape0.push_back(50);
	shape0.push_back(50);

	gen_arr<int > v0(shape0,(0.0,0.0));

	// int temp = 0;
	// for(int i = 0; i < 600; i++){
	// 	for(int j = 0; j < 600; j++){
	// 		for (int k = 0; k < 5; k++){
	// 			for (int l = 0; l < 5; l++){
	// 				v[i][j][k][l].getval() = temp;
	// 				temp++;
	// 			}
	// 		}
	// 	}
	// }

	v0[1][1][1][1].getval() = -58;
	

	// v[1][2].getval()=70;
	// v[1][3].getval()=80;
	// v[1][4].getval()=90;
	// v[2][2].getval()=90;
	// v[2][3].getval()=100;
	// v[2][4].getval()=110;
	// v[3][2].getval()=110;
	// v[3][3].getval()=120;
	// v[3][4].getval()=130;
	// v[4][2].getval()=130;
	// v[4][3].getval()=140;
	// v[4][4].getval()=150;

	// gen_arr<complex<double> > ans = v.dft_2();
	int aa = v0.min_value(1);
	int ab = v0.max_value(1);

	gen_arr< int> ans = v0.mean(200,2);

	// cout<<ans.shape<<endl;
	cout<<aa<<endl<<ab<<endl;
	cout<<vec_to_string(ans.get_shape())<<endl;
	// cout<<ans.str();


	vector<unsigned int> shape2;
	shape1.push_back(2);
	shape1.push_back(3);
	shape2.push_back(3);
	shape2.push_back(4);

	gen_arr<int> v(shape1,0);
	gen_arr<int> vv(shape2,0);

	v[1][2].getval()= 7;
	v[1][0].getval()=8;
	v[1][1].getval()=9;
	v[0][2].getval()=9;
	v[0][1].getval()=0;
	v[0][0].getval()=1;
	vv[1][2].getval()=1;
	vv[1][3].getval()=2;
	vv[1][1].getval()=3;
	vv[1][2].getval()=3;
	vv[1][3].getval()=4;
	vv[1][1].getval()=5;
	
	cout<<v.str();
	cout<<vv.str();
	cout<<v.matmul(vv).str();
	
	// gen_arr<complex<double> > ans = v.dft_2();
	// gen_arr<complex<double> > ans2 = ans.dft_2();
	// ans = v[1];
	// ans = v;

	// cout<<ans.str();
	
	// cout<<"sdsdd \n\n\n\n";
	// // ans = v;
	
	// // cout<<ans.str();
	// cout<<v[4].str();
	// cout<<"sdsdd \n\n\n\n";
	// ans[0] = v[4];
	// cout<<"sdsdd \n\n\n\n";
	// cout<<ans[0].str();

	// cout<<ans2[7].str();
	// cout<<"sdsdd \n\n\n\n";
	// ans[0] = ans2[7];
	// cout<<ans[0].str();
	// cout<<ans[1].str();
	// cout<<ans[2].str();
	// cout<<ans[3].str();

	// // int shapearr[2] = {4,1};
	vector<vector<unsigned int> > shapes;
	shapes.push_back(vector<unsigned int> ());
	shapes[0].push_back(1000);shapes[0].push_back(1000);
	shapes.push_back(vector<unsigned int> ());

	shapes[1].push_back(100);shapes[1].push_back(100);
	
	shapes.push_back(vector<unsigned int> ());
	shapes[2].push_back(3);shapes[2].push_back(3);
	// // shapes[0].push_back(1000);shapes[0].push_back(10000);
	// // vector<unsigned int> shape(shapearr, shapearr+sizeof(shapearr)/sizeof(shapearr[0]));
	auto t_stamp01 = chrono::high_resolution_clock::now();
	gen_arr<int > a0(shapes[0], 9), b0(shapes[0], 2);
	// gen_arr<int> a0(shapes[0], 9), b0(shapes[0], 2);
	auto t_stamp02 = chrono::high_resolution_clock::now();
	// gen_arr<int> c0 = a0 + b0;
	auto t_stamp03 = chrono::high_resolution_clock::now();
	auto alloc_time021 = chrono::duration_cast<chrono::duration<double>>(t_stamp02 - t_stamp01).count();
	auto alloc_time032 = chrono::duration_cast<chrono::duration<double>>(t_stamp03 - t_stamp02).count();

	// cout << "c0.shape" << vec_to_string(c0.get_shape()) << endl;
	printf("Time(in seconds): Alloc:%f;\tComputation(add):%f;\n", alloc_time021, alloc_time032);

	auto t_stamp11 = chrono::high_resolution_clock::now();
	gen_arr<complex<double>> a1(shapes[1], 3), b1(shapes[1], 7);
	a1[0][0].getval() = 4;b1[1][0].getval() = 0;
	auto t_stamp12 = chrono::high_resolution_clock::now();
	// auto c1 = a1.matmul4(b1);
	auto c1 = a1.dft_2();
	// auto c1 = a1.matmul_mt(b1, num_thr);
	// auto c1 = a1.multi_threaded_add2(b1, num_thr);
	// auto c1 = a1.multi_threaded_op2(b1, num_thr, "add");
	// auto c1 = a1.multi_threaded_add(b1, num_thr); 
	// auto c1 = a1.multi_threaded_add(b1, num_thr);
	// // printf("c2 calculated\n");
    auto t_stamp13 = chrono::high_resolution_clock::now();
	auto alloc_time121 = chrono::duration_cast<chrono::duration<double>>(t_stamp12 - t_stamp11).count();
	auto alloc_time132 = chrono::duration_cast<chrono::duration<double>>(t_stamp13 - t_stamp12).count();
	// cout << "fooo\n";
	// cout << "c1.shape" << vec_to_string(c1.get_shape()) << endl;
	printf("Time(in seconds): Alloc:%f;\tComputation(matmul):%f;\n", alloc_time121, alloc_time132);

	cout << "a1's dump: \n" << a1.str() << endl; // a2.dummy_dump()
	cout << "b1's dump: \n" << b1.str() << endl;
	cout << "c1's dump: \n" << c1.str() << endl;
	auto t_stamp21 = chrono::high_resolution_clock::now();
	gen_arr<int> a2(shapes[1], 9), b2(shapes[1], 2);
	a2[2][0].getval() = 0;
	// cout << "a2.shape" << vec_to_string(a2.get_shape()) << endl;
	auto t_stamp22 = chrono::high_resolution_clock::now();
	gen_arr<int> c2 = a2.transpose();
	auto t_stamp23 = chrono::high_resolution_clock::now();
	auto alloc_time221 = chrono::duration_cast<chrono::duration<double>>(t_stamp22 - t_stamp21).count();
	auto alloc_time232 = chrono::duration_cast<chrono::duration<double>>(t_stamp23 - t_stamp22).count();

	cout << "c2.shape" << vec_to_string(c2.get_shape()) << endl;
	printf("Time(in seconds): Alloc:%f;\tComputation (tpose single):%f;\n", alloc_time221, alloc_time232);
	// cout << "a's shape " << vec_to_string(a2.get_shape()) << endl;
	// cout << "a2's dump: \n" << a2.str() << endl; // a2.dummy_dump()
	// cout << "b2's dump: \n" << b2.str() << endl;
	// cout << "c2's dump: \n" << c2.str() << endl;
	auto t_stamp31 = chrono::high_resolution_clock::now();
	gen_arr<int> a3(shapes[1], 9), b3(shapes[1], 2);
	// a3[2][0].getval() = 0;
	// cout << "a2.shape" << vec_to_string(a2.get_shape()) << endl;
	auto t_stamp32 = chrono::high_resolution_clock::now();
	gen_arr<int> c3 = a3.transpose_mt(num_thr);
	auto t_stamp33 = chrono::high_resolution_clock::now();
	auto alloc_time321 = chrono::duration_cast<chrono::duration<double>>(t_stamp32 - t_stamp31).count();
	auto alloc_time332 = chrono::duration_cast<chrono::duration<double>>(t_stamp33 - t_stamp32).count();
	
	cout << "c2.shape" << vec_to_string(c2.get_shape()) << endl;
	printf("Time(in seconds): Alloc:%f;\tComputation (tpose multi):%f;\n", alloc_time321, alloc_time332);

	gen_arr<int> bb0(shapes[2], 9);
	vector<unsigned int> shape4 = bb0.get_shape();
	reverse(shape4.begin(), shape4.end());
	bb0[1][1].getval() = 0;
	cout << "a3's dump: \n" << bb0.str() << endl;
	bb0.reshape(shape4);
	cout << "a3's dump after reshape: \n" << bb0.str() << endl;

	// printf("Time(in seconds): Alloc:%f;\tComputation (single):%f;\tComputation(%d):%f;\n", alloc_time321, alloc_time332, num_thr, alloc_time332);
	


	return 0;
}
