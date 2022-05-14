#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <string>
#include <stack>

using namespace std;

const int NUM_OF_TESTS = 10;
const int MAX_ARRAY_SIZE = 10;

//Helper functions & utility functions
vector<int> generateArrays(vector<int>& unsorted) {
	random_device rd; //obtain random # from hardware
	mt19937 gen(rd()); //seed the generator
	uniform_int_distribution<> distr(-100, 100); //range of random #s
	
	//Populate array with random values
	for (int i = 0; i < unsorted.size(); i++) {
		unsorted[i] = distr(gen);
	}

	//Print array
	//for (const auto& v : unsorted) {
	//	cout << v << endl;
	//}
	//cout << endl;

	return unsorted;
}

void outputInformation(vector<int>& kth_smallest_elem, double average_elapsed_time, int kth, int current_kth_array_value) {
	string kth_string_values[5] = { "0","n/4","n/2","3n/4","n"};
	
	cout << "Average Time To Find " << kth_string_values[current_kth_array_value] << "th Value Of [" << kth << "]: " << average_elapsed_time << " nanoseconds" << endl;
	cout << "Kth elements found after [" << NUM_OF_TESTS << "] tests: [" <<
			kth_smallest_elem[0] << ", " << 
		    kth_smallest_elem[1] << ", " << 
		    kth_smallest_elem[2] << ", ... , " << 
		    kth_smallest_elem[kth_smallest_elem.size() - 1] << "]" << endl << endl;
}

void printMenu() {
	cout
		<< "(1) Merge Sort            (Select-kth 1)" << endl
		<< "(2) Quicksort Iteratively (Select-kth 2)" << endl
		<< "(3) Quicksort Recursively (Select-kth 3)" << endl
		<< "(4) Median of Medians	  (Select-kth 4)" << endl
		<< endl << "Choose an algorithm to test: ";
}


//Merge Sort Functions
void MERGESORT_MERGE_ARRAYS(vector<int>& unsorted, int start, int mid, int end) {
	//Create temporary array that will hold merged elements
	vector<int> tempArr;

	//These values will change over time as we are merging different parts of the array
	int i, j;
	i = start;
	j = mid + 1;

	//Loop will merge arrays back into one from left to right
	while (i <= mid && j <= end) {
		if (unsorted[i] <= unsorted[j]) {
			tempArr.push_back(unsorted[i]);
			++i;
		}
		else {
			tempArr.push_back(unsorted[j]);
			++j;
		}
	}

	//If there are any leftover elements from the left sub array put those back in tempArr
	while (i <= mid) {
		tempArr.push_back(unsorted[i]);
		++i;
	}

	//If there are any leftover elements from the right sub array put those back in tempArr
	while (j <= end) {
		tempArr.push_back(unsorted[j]);
		++j;
	}

	//Copies information from tempArr back to the unsorted array
	for (int i = start; i <= end; ++i) {
		unsorted[i] = tempArr[i - start];
	}
}

void MERGESORT_RECURSIVE_CALL(vector<int>& unsorted, int start, int end) {
	//Recursively call mergesort to split input array into arrays of size one.
	if (start < end) {
		
		//Find midpoint of array
		int mid = (start + end) / 2;

		MERGESORT_RECURSIVE_CALL(unsorted, start, mid);
		MERGESORT_RECURSIVE_CALL(unsorted, mid + 1, end);
		MERGESORT_MERGE_ARRAYS(unsorted, start, mid, end);
	}
}

void MERGESORT() {
	
	//Make size of arrays change automatically during execution
	int array_sizes[8] = { 10,50,100,500,1000,5000,10000,50000 };
	int index = 0;

	//This loop controls the size of the arrays
	for (int size = array_sizes[index]; size <= MAX_ARRAY_SIZE; size = array_sizes[index]) {

		//Create arrays to hold elapsed times of different selections
		vector<double> elapsed_times(NUM_OF_TESTS, 0);
		vector<double> total_average_elapsed_times(5, 0);

		//Create array to hold the kth smallest element that are found in each test
		vector<int> kth_smallest_elem(NUM_OF_TESTS, 0);

		//Make selected kth value change automatically during execution
		int k_values[5] = { 0,						//0	   th
							floor(size / 4),		//n/4  th
							floor(size / 2),		//n/2  th
							floor((3 * size) / 4),	//3n/4 th
							floor(size - 1)};		//n	   th

		//Output what size of array we are currently on
		cout << endl << "Size of Array: [" << size << "]"<<endl;

		//This loop here makes sure the size of an array does not change before you iterate through all k_values and find their averages.
		int current_kth_array_value = 0;
		while (current_kth_array_value < 5) {
			
			//This loop controls how many times an array will be generated and tested
			for (int iteration = 0; iteration < NUM_OF_TESTS; iteration++) {

				//Generate Unsorted array
				vector<int> unsorted(size, 0);
				unsorted = generateArrays(unsorted);

				//Time point tracking start of algorithm operations
				auto start = chrono::steady_clock::now();

				//Merge Sort Algorithm  
				MERGESORT_RECURSIVE_CALL(unsorted, 0, size - 1); //(array, starting position, ending position)

				//Find kth element in sorted array. Add it to an array holding all selected kth values.
				int k = k_values[current_kth_array_value];
				kth_smallest_elem[iteration] = unsorted[k]; //might have a problem here with getting the sorted array

				//Time point tracking end of algorithm operations
				auto end = chrono::steady_clock::now();

				//Find difference between start and end time values for ONE kth value. Store that value in an array.
				double elapsed_time = double(chrono::duration_cast <chrono::nanoseconds> (end - start).count());
				elapsed_times[iteration] = elapsed_time;
			}

			//Finding average elapsed time of current kth value after NUM_OF_TESTS iterations.
			auto sum_of_elapsed_times = 0.0;
			for (auto& time : elapsed_times)
				sum_of_elapsed_times += time;
			auto average_elapsed_time = sum_of_elapsed_times / NUM_OF_TESTS; //average times of n = 10 and k = 0 for 10 tests

			//Output average_elapsed_time information and kth smallest elements that we found to the console
			int kth = k_values[current_kth_array_value]; //doing this so I can format output to automatically change during execution
			outputInformation(kth_smallest_elem, average_elapsed_time, kth, current_kth_array_value);

			current_kth_array_value++; //Make sure k_values array value increases so that value we are searching for changes during execution
		}
		index +=1;	//Make sure size of array increases
	}
}


//Quicksort Helper Functions
void SWAP(int& a, int& b) {
	int temp = a;
	a = b;
	b = temp;
}

int QUICKSORT_PARTITION(vector<int>& unsorted, int low, int high) {
	//Calculate pivotposition and element we are comparing the current element to
	int pivot_position = unsorted[high];
	int small_element = low - 1;
	
	//Checking if current element is < pivotposition
	for (int current_element = low; current_element <= high - 1; current_element++) {
		if (unsorted[current_element] < pivot_position) {
			small_element++;
			swap(unsorted[small_element], unsorted[current_element]);
		}
	}

	//This last swap handles putting the pivotposition at the correct position in the array
	swap(unsorted[small_element + 1], unsorted[high]);

	//Returns pivotpositions index after sorting has occurred
	return (small_element + 1);
}

//Quicksort Iterative Functions
void QUICKSORT_ITERATIVE_CALL(vector<int>& unsorted, int size, int k, int& returned_kth_value) {
	//Keeps track of start and end of sub arrays. Push those values to the stack.
	stack<pair<int, int>> stack;
	int low = 0;
	int high = size - 1;
	stack.push(make_pair(low, high));

	//Controls how many total iterations there will be
	while (!stack.empty()) {
		low = stack.top().first, high = stack.top().second;
		stack.pop();

		//Print array
		cout << endl << "Array before sorting" << endl;
		for (const auto& n : unsorted) {
			cout << n << endl;
		}

		//Call partition function and get index of the pivotposition
		int pivot_index = QUICKSORT_PARTITION(unsorted, low, high);
		cout << endl << "pivot index: " << pivot_index << endl;

		//Check if the pivot_index is == k to stop sorting early
		if (k == pivot_index) {
			returned_kth_value = unsorted[pivot_index];
			cout << endl << "k == pivotposition. go to next array" << endl;
			return;	//this return statement is working, but in the recursive version, it doesn't
		}

		//Print array
		cout << endl << "Array after sorting" << endl;
		for (const auto& n : unsorted) {
			cout << n << endl;
		}

		//Push values < than pivotposition to the stack
		if (pivot_index - 1 > low) {
			stack.push(make_pair(low, pivot_index - 1));
		}

		//Push values > than pivotposition to the stack
		if (pivot_index + 1 < high) {
			stack.push(make_pair(pivot_index + 1, high));
		}
	}
}

void QUICKSORT_ITERATIVE() {
	//Make size of arrays change automatically during execution
	int array_sizes[8] = { 10,50,100,500,1000,5000,10000,50000 };
	int index = 0;

	//This loop controls the size of the arrays
	for (int size = array_sizes[index]; size <= MAX_ARRAY_SIZE; size = array_sizes[index]) {

		//Create arrays to hold elapsed times of different selections
		vector<double> elapsed_times(NUM_OF_TESTS, 0);
		vector<double> total_average_elapsed_times(5, 0);

		//Create array to hold the kth smallest element that are found in each test
		vector<int> kth_smallest_elem(NUM_OF_TESTS, 0);

		//Make selected kth value change automatically during execution
		int k_values[5] = { 0,						//0	   th
							floor(size / 4),		//n/4  th
							floor(size / 2),		//n/2  th
							floor((3 * size) / 4),	//3n/4 th
							floor(size - 1) };		//n	   th

		//Output what size of array we are currently on
		cout << endl << "Size of Array: [" << size << "]" << endl;

		//This loop here makes sure the size of an array does not change before you iterate through all k_values and find their averages.
		int current_kth_array_value = 0;
		while (current_kth_array_value < 5) {

			//This loop controls how many times an array will be generated and tested
			for (int iteration = 0; iteration < NUM_OF_TESTS; iteration++) {

				//Generate Unsorted array
				vector<int> unsorted(size, 0);
				unsorted = generateArrays(unsorted);

				//Time point tracking start of algorithm operations
				auto start = chrono::steady_clock::now();

				//Quicksort Algorithm Call
				int k = k_values[current_kth_array_value];
				int returned_kth_value;
				QUICKSORT_ITERATIVE_CALL(unsorted, size, k, returned_kth_value); //(array, size, k_index, kth_value we are looking for)

				//Find kth element in sorted array. Add it to an array holding all selected kth values
				kth_smallest_elem[iteration] = returned_kth_value; //might have a problem here with getting the sorted array

				//Time point tracking end of algorithm operations
				auto end = chrono::steady_clock::now();

				//Find difference between start and end time values for ONE kth value. Store that value in an array.
				double elapsed_time = double(chrono::duration_cast <chrono::nanoseconds> (end - start).count());
				elapsed_times[iteration] = elapsed_time;
			}

			//Finding average elapsed time of current kth value after NUM_OF_TESTS iterations.
			auto sum_of_elapsed_times = 0.0;
			for (auto& time : elapsed_times)
				sum_of_elapsed_times += time;
			auto average_elapsed_time = sum_of_elapsed_times / NUM_OF_TESTS; //average times of n = 10 and k = 0 for 10 tests

			//Output average_elapsed_time information and kth smallest elements that we found to the console
			int kth = k_values[current_kth_array_value]; //doing this so I can format output to automatically change during execution
			outputInformation(kth_smallest_elem, average_elapsed_time, kth, current_kth_array_value);

			current_kth_array_value++; //Make sure k_values array value increases so that value we are searching for changes during execution
		}
		index += 1;	//Make sure size of array increases
	}
}


//Quicksort Recursive Functions
void QUICKSORT_RECURSIVE_CALL(vector<int>& unsorted, int low, int high, int k, int& returned_kth_value) {
	if (low < high) {
		int pivot_index = QUICKSORT_PARTITION(unsorted, low, high);

		//Check if k == pivotposition. This is not working correctly! Sometimes the return statement works, other times it doesnt.
		if (k == pivot_index) {
			returned_kth_value = unsorted[pivot_index];
			return;
		}
		/*else if (k < pivot_index) { //possible error here
			pivot_index = pivot_index - 1;
		}
		else {								//possible error here
			pivot_index = pivot_index + 1;
			k = k - pivot_index;
		}*/

		QUICKSORT_RECURSIVE_CALL(unsorted, low, pivot_index - 1, k, returned_kth_value); //possible error here:  pivot_index - 1
		QUICKSORT_RECURSIVE_CALL(unsorted, pivot_index + 1, high, k, returned_kth_value); //possible error here:  pivot_index + 1
	}
}

void QUICKSORT_RECURSIVE() {

	//Make size of arrays change automatically during execution
	int array_sizes[8] = { 10,50,100,500,1000,5000,10000,50000 };
	int index = 0;

	//This loop controls the size of the arrays
	for (int size = array_sizes[index]; size <= MAX_ARRAY_SIZE; size = array_sizes[index]) {

		//Create arrays to hold elapsed times of different selections
		vector<double> elapsed_times(NUM_OF_TESTS, 0);
		vector<double> total_average_elapsed_times(5, 0);

		//Create array to hold the kth smallest element that are found in each test
		vector<int> kth_smallest_elem(NUM_OF_TESTS, 0);

		//Make selected kth value change automatically during execution
		int k_values[5] = { 0,						//0	   th
							floor(size / 4),		//n/4  th
							floor(size / 2),		//n/2  th
							floor((3 * size) / 4),	//3n/4 th
							floor(size - 1) };		//n	   th

		//Output what size of array we are currently on
		cout << endl << "Size of Array: [" << size << "]" << endl;

		//This loop here makes sure the size of an array does not change before you iterate through all k_values and find their averages.
		int current_kth_array_value = 0;
		while (current_kth_array_value < 5) {

			//This loop controls how many times an array will be generated and tested
			for (int iteration = 0; iteration < NUM_OF_TESTS; iteration++) {

				//Generate Unsorted array
				vector<int> unsorted(size, 0);
				unsorted = generateArrays(unsorted);

				//Time point tracking start of algorithm operations
				auto start = chrono::steady_clock::now();

				//Quicksort Algorithm Call
				int k = k_values[current_kth_array_value];
				int returned_kth_value;
				QUICKSORT_RECURSIVE_CALL(unsorted, 0, size - 1, k, returned_kth_value); //(array, low position, high position, k_index, kth_value we are looking for)

				//Find kth element in sorted array. Add it to an array holding all selected kth values
				kth_smallest_elem[iteration] = returned_kth_value; //might have a problem here with getting the sorted array

				//Time point tracking end of algorithm operations
				auto end = chrono::steady_clock::now();

				//Find difference between start and end time values for ONE kth value. Store that value in an array.
				double elapsed_time = double(chrono::duration_cast <chrono::nanoseconds> (end - start).count());
				elapsed_times[iteration] = elapsed_time;
			}

			//Finding average elapsed time of current kth value after NUM_OF_TESTS iterations.
			auto sum_of_elapsed_times = 0.0;
			for (auto& time : elapsed_times)
				sum_of_elapsed_times += time;
			auto average_elapsed_time = sum_of_elapsed_times / NUM_OF_TESTS; //average times of n = 10 and k = 0 for 10 tests

			//Output average_elapsed_time information and kth smallest elements that we found to the console
			int kth = k_values[current_kth_array_value]; //doing this so I can format output to automatically change during execution
			outputInformation(kth_smallest_elem, average_elapsed_time, kth, current_kth_array_value);

			current_kth_array_value++; //Make sure k_values array value increases so that value we are searching for changes during execution
		}
		index += 1;	//Make sure size of array increases
	}
}


//Median Of Medians Functions
void MEDIAN_OF_MEDIAN() {

}


//MAIN
int main() {
	
	//Display print menu and get user input
	printMenu();
	int choice;
	cin >> choice, cout << endl;

	//Call chosen algorithm
	switch (choice) {
		case 1:
			MERGESORT();
			break;
		case 2:
			QUICKSORT_ITERATIVE();
			break;
		case 3:
			QUICKSORT_RECURSIVE();
			break;
		case 4:
			MEDIAN_OF_MEDIAN();
			break;
		default:
			break;
	}

	//Cin statement so that program won't end immediately after finishing calculations
	char stop;
	cout << "Press any key to stop program: ", cin >> stop;

	return 0;
}