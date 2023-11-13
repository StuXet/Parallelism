#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

// Global mutex to ensure proper ordering
mutex mtx;

// Function to display even numbers
void displayEvenNumbers() {
    for (int i = 0; i <= 1000; i += 2) {
        // Lock the mutex before printing
        unique_lock<mutex> lock(mtx);
        cout << i << " ";
        // Unlock the mutex
        lock.unlock();
    }
}

// Function to display odd numbers
void displayOddNumbers() {
    for (int i = 1; i <= 1000; i += 2) {
        // Lock the mutex before printing
        unique_lock<mutex> lock(mtx);
        cout << i << " ";
        // Unlock the mutex
        lock.unlock();
    }
}

int main() {
    thread t1(displayEvenNumbers);
    thread t2(displayOddNumbers);

    t1.join();
    t2.join();

    return 0;
}