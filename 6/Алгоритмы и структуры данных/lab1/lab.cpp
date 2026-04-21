#include <iostream>
#include <vector>
#include <cstdlib>  // rand, srand
#include <ctime>    // time
#include <cmath>    // round
using namespace std;

int n, m; // matrix dimensions: n rows, m columns

// Function: request matrix size from the user
void inputSize() {
    cout << "Enter the number of rows n: ";
    cin >> n;
    while (n <= 0) {
        cout << "n must be greater than 0. Enter again: ";
        cin >> n;
    }
    cout << "Enter the number of columns m: ";
    cin >> m;
    while (m <= 0) {
        cout << "m must be greater than 0. Enter again: ";
        cin >> m;
    }
}

// Function: manual matrix input
void fillMatrix(vector<vector<int> >& A) {
    cout << "Enter the elements of the matrix " << n << "x" << m << ":\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << "A[" << i+1 << "][" << j+1 << "] = ";
            cin >> A[i][j];
        }
    }
}

// Function: fill matrix with random numbers (from -9 to 9)
void fillMatrixRandom(vector<vector<int> >& A) {
    srand((unsigned int)time(0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            A[i][j] = rand() % 19 - 9; // numbers from -9 to 9
        }
    }
    cout << "Matrix filled with random numbers.\n";
}

// Function: print matrix to the screen
void printMatrix(const vector<vector<int> >& A) {
    cout << "\nMatrix " << n << "x" << m << ":\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << A[i][j];
            if (j < m - 1) cout << "\t";
        }
        cout << "\n";
    }
}

// Function: calculate determinant using Gaussian elimination
// Works only for square matrices (n == m)
// Uses double internally for precise intermediate calculations
int calcDeterminant(const vector<vector<int> >& A) {

    // Copy matrix to double to avoid precision loss during division
    vector<vector<double> > B(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            B[i][j] = A[i][j];

    double det = 1.0;

    for (int col = 0; col < n; col++) {

        // Find a non-zero pivot element in the column
        int pivotRow = -1;
        for (int row = col; row < n; row++) {
            if (B[row][col] != 0) {
                pivotRow = row;
                break;
            }
        }

        // If the entire column is zero — determinant is 0
        if (pivotRow == -1) {
            return 0;
        }

        // Swap rows if necessary
        if (pivotRow != col) {
            swap(B[pivotRow], B[col]);
            det *= -1; // row swap changes sign
        }

        det *= B[col][col];

        // Eliminate elements below the pivot
        for (int row = col + 1; row < n; row++) {
            double factor = B[row][col] / B[col][col];
            for (int k = col; k < n; k++) {
                B[row][k] -= factor * B[col][k];
            }
        }
    }

    return (int)round(det); // determinant of integer matrix is integer
}

// Function: print result
void printResult(int det) {
    cout << "\nMatrix determinant = " << det << "\n";
}

// Program menu
void showMenu() {
    cout << "\n=== MENU ===\n";
    cout << "1. Enter matrix manually\n";
    cout << "2. Fill matrix with random numbers\n";
    cout << "3. Display matrix\n";
    cout << "4. Calculate determinant\n";
    cout << "0. Exit\n";
    cout << "Choose an option: ";
}

int main() {

    inputSize();
    vector<vector<int> > A(n, vector<int>(m, 0));

    bool filled = false;
    int choice;

    do {
        showMenu();
        cin >> choice;

        switch (choice) {

            case 1:
                fillMatrix(A);
                filled = true;
                cout << "Matrix entered.\n";
                break;

            case 2:
                fillMatrixRandom(A);
                filled = true;
                break;

            case 3:
                if (!filled) {
                    cout << "First fill the matrix (option 1 or 2).\n";
                } else {
                    printMatrix(A);
                }
                break;

            case 4:
                if (!filled) {
                    cout << "First fill the matrix (option 1 or 2).\n";
                }
                else if (n != m) {
                    cout << "The determinant exists only for square matrices (n == m).\n";
                    cout << "Current size: " << n << "x" << m << " — not square.\n";
                }
                else {
                    int det = calcDeterminant(A);
                    printResult(det);
                }
                break;

            case 0:
                cout << "Exiting the program.\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
