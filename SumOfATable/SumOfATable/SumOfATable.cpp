#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

// Function to compute the sum of elements in a sequential manner
int sequentialSum(const vector<int>& arr) 
{
    int sum = 0;
    for (int element : arr)
    {
        sum += element;
    }
    return sum;
}

// Function for each thread to compute the sum of its assigned sub-table
int threadLocalSum(const vector<int>& arr, int start, int end) 
{
    int localSum = 0;
    for (int i = start; i < end; ++i) 
    {
        localSum += arr[i];
    }
    return localSum;
}

// Function for each thread to update the global sum using mutex
void globalSumWithMutex(const vector<int>& arr, int start, int end, int& globalSum, mutex& mtx) 
{
    int localSum = 0;
    for (int i = start; i < end; ++i) 
    {
        localSum += arr[i];
    }

    // Lock the mutex before updating the global sum
    lock_guard<mutex> lock(mtx);
    globalSum += localSum;
}

int main() 
{
    const int n = 1000; // Number of elements in the table
    const int m = 4;    // Number of threads

    // Create a vector of n elements with random values for demonstration
    vector<int> table(n);
    for (int i = 0; i < n; ++i) 
    {
        table[i] = i + 1;
    }

    // 1. Sequential version
    int seqSum = sequentialSum(table);
    cout << "Sequential Sum: " << seqSum << endl;

    // 2. Version using thread-local variables
    int localSum = 0;
    vector<thread> threads;

    for (int i = 0; i < m; ++i) 
    {
        int start = i * (n / m);
        int end = (i == m - 1) ? n : (i + 1) * (n / m);

        // Launch a thread for each sub-table
        threads.emplace_back(threadLocalSum, ref(table), start, end);
    }

    // Join threads and accumulate thread-local sums
    for (thread& t : threads)
    {
        t.join();
    }

    for (int i = 0; i < m; ++i)
    {
        localSum += threadLocalSum(table, i * (n / m), (i == m - 1) ? n : (i + 1) * (n / m));
    }

    cout << "Thread-Local Sum: " << localSum << endl;

    // 3. Version using a global sum variable and mutexes
    int globalSum = 0;
    mutex mutex;

    threads.clear();

    for (int i = 0; i < m; ++i) 
    {
        int start = i * (n / m);
        int end = (i == m - 1) ? n : (i + 1) * (n / m);

        // Launch a thread for each sub-table with mutex protection
        threads.emplace_back(globalSumWithMutex, ref(table), start, end, ref(globalSum), ref(mutex));
    }

    // Join threads
    for (thread& t : threads)
    {
        t.join();
    }

    cout << "Global Sum with Mutex: " << globalSum << endl;

    return 0;
}