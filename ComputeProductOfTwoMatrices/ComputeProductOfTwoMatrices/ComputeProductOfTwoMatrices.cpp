#include <iostream>
#include <vector>
#include <thread>
using namespace std;

const int N = 3; // Size of matrices (N x N)

// Function to compute the product of two matrices sequentially
void multiplySequential(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<std::vector<int>>& P)
{
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            P[i][j] = 0;
            for (int k = 0; k < N; ++k) 
            {
                P[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Function to compute a single element of the product matrix in parallel
void multiplyParallel(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& P, int row, int col)
{
    for (int k = 0; k < N; ++k) 
    {
        P[row][col] += A[row][k] * B[k][col];
    }
}

int main()
{
    // Initialize matrices A, B, and P
    vector<vector<int>> A = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    vector<vector<int>> B = { {9, 8, 7}, {6, 5, 4}, {3, 2, 1} };
    vector<vector<int>> P(N, vector<int>(N, 0));

    // 1. Sequential version
    multiplySequential(A, B, P);

    // Display the result of the sequential version
    cout << "Sequential Result (P = A x B):" << endl;
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j)
        {
            cout << P[i][j] << " ";
        }
        cout << endl;
    }

    // 2. Parallel version
    vector<thread> threads;

    // Launch threads to compute each element of the product matrix
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j)
        {
            threads.emplace_back(multiplyParallel, ref(A), ref(B), ref(P), i, j);
        }
    }

    // Wait for all threads to finish
    for (auto& thread : threads) 
    {
        thread.join();
    }

    // Display the result of the parallel version
    cout << "\nParallel Result (P = A x B):" << endl;
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            cout << P[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
