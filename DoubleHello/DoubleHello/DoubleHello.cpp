#include <iostream>
#include <thread>
using namespace std;

// Function to be executed by each thread
void printHelloWorld() 
{
    cout << "Hello World !\n" << endl;
}

int main() 
{
    thread thread1(printHelloWorld);
    thread thread2(printHelloWorld);

    thread1.join();
    thread2.join();

    return 0;
}