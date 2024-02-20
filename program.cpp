#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <omp.h>

using namespace std;

// Function to display a matrix
void display(double** matrix, int length) {
    // Loop for rows
    for (int p = 0; p < length; p++) {
        // Loop for columns
        for (int m = 0; m < length; m++) {
            cout << matrix[p][m] << " ";  // Display matrix element
        }
        cout << endl;  // Move to the next row
    }
}

// Function to multiply two matrices with optional transposition
void multiply(double** matrixA, double** matrixB, double** matrixC, int length, bool transpose = false) {
    #pragma omp parallel for
    // Loop for rows of the result matrix
    for (int p = 0; p < length; p++) {
        // Loop for columns of the result matrix
        for (int m = 0; m < length; m++) {
            double s = 0;
            // Loop for corresponding elements in the input matrices
            for (int d = 0; d < length; d++) {
                // Multiply and accumulate the product
                if (transpose) {
                    s += matrixA[p][d] * matrixB[m][d];  // Transpose 
                } else {
                    s += matrixA[p][d] * matrixB[d][m];
                }
            }
            matrixC[p][m] = s;  // Store the result in the output matrix
        }
    }
}

int main() {
    int size = 1200;
    
    // Allocate memory for matrices
    double** matrixA = new double*[size];
    double** matrixB = new double*[size];
    double** matrixC = new double*[size];
    double** result = new double*[size];
    // Loop to allocate memory for each matrix
    for (int p = 0; p < size; p++) {
        matrixA[p] = new double[size];
        matrixB[p] = new double[size];
        matrixC[p] = new double[size];
        result[p] = new double[size];
    }
    
    // Initialize matrices with random values
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-10, 10);
    // Loop through rows and columns to fill matrices with random values
    for (int p = 0; p < size; p++) {
        for (int m = 0; m < size; m++) {
            matrixA[p][m] = dis(gen);  // Random value for matrix A
            matrixB[p][m] = dis(gen);  // Random value for matrix B
        }
    }

    // Multiply matrices with optional transposition and measure execution time
    auto start = chrono::high_resolution_clock::now();

    // Choose whether to transpose matrixB before multiplication
    bool transposeB = false;

    // Conditional statement to switch between different multiplication methods
    if (transposeB) {
        multiply(matrixA, matrixB, matrixC, size, true);  // Transpose B
    } else {
        multiply(matrixA, matrixB, matrixC, size);  // No transpose
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "It takes " << duration.count() << " milliseconds to execute after multiplication" << endl;

    // Write output to file
    ofstream output("output.txt");
    // Loop through rows and columns to write the result to a file
    for (int p = 0; p < size; p++) {
        for (int m = 0; m < size; m++) {
            output << result[p][m] << " ";  // Write result to file
        }
        output << endl;  // Move to the next row in the file
    }
    output.close();
    
    // Free memory
    // Loop to deallocate memory for each matrix
    for (int p = 0; p < size; p++) {
        delete[] matrixA[p];
        delete[] matrixB[p];
        delete[] matrixC[p];
        delete[] result[p];
    }
    delete[] matrixA;
    delete[] matrixB;
    delete[] matrixC;
    delete[] result;
    
    return 0;
}
