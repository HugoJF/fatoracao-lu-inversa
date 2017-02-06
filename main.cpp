#include <iostream>
using namespace std;

void copyMatrix(double** origin, double** dest, int matrixSize) {
    for(int y = 0; y < matrixSize; y++) {
        for(int x = 0; x < matrixSize; x++) {
            dest[x][y] = origin[x][y];
        }
    }
}

void copyVector(double* origin, double* dest, int vectorSize) {
    for(int i = 0; i < vectorSize; i++) {
        dest[i] = origin[i];
    }
}

void fillMatrix(double** matrix, int matrixSize) {
    for(int y = 0; y < matrixSize; y++) {
        for(int x = 0; x < matrixSize; x++) {
            matrix[x][y] = 0;
        }
    }
}

void fillVector(double* vectorr, int vectorSize) {
    for(int i = 0; i < vectorSize; i++) {
        vectorr[i] = 0;
    }
}

void printMatrix(double** matrix, int matrixSize) {
    for(int y = 0; y < matrixSize; y++) {
        for(int x = 0; x < matrixSize; x++) {
            cout << matrix[x][y] << " ";
        }

        cout << endl;
    }
}

void printVector(double* vectorr, int vectorSize) {
    for(int i = 0; i < vectorSize; i++) {
        cout << vectorr[i] << endl;
    }
}


void initializeMatrix(double*** matrix, int n) {
    (*matrix) = new double*[n];

    for(int i = 0; i < n; i++) {
        (*matrix)[i] = new double[n];
    }
}

void initializeVector(double ** vectorr, int n) {
    (*vectorr) = new double[n];
}
void LUDecomp(double** matrix, double** matrixL, double** matrixU, int n) {
    fillMatrix(matrixU, n);
    // Copia matriz lida para matrizU, para manter a original e para poder trabalhar em cima da matrizU
    copyMatrix(matrix, matrixU, n);

    // Computa L e U
    for(int x = 0; x < n; x++) {
        matrixL[x][x] = 1; // Diagonal 1 da matriz L
        for(int y = x + 1; y < n; y++) { // Varre valores abaixo da diagonal
            double mult = matrixU[x][y] / matrixU[x][x]; // Calcula o multiplicador
            matrixL[x][y] = mult; // Coloca multiplicador na matriz L

            for(int z = 0; z < n; z++) { // Varre matriz horizontalmente para aplicar o multiplicador
                matrixU[z][y] = matrixU[z][y] - matrixU[z][x] * mult;
            }
        }
    }
}

void solveAxBProblemForward(double** matrix, double* xVector, double* bVector, int n) {
    double** tempMatrix;
    double* tempBVector;

    initializeMatrix(&tempMatrix, n);
    initializeVector(&tempBVector, n);

    copyMatrix(matrix, tempMatrix, n);
    copyVector(bVector, tempBVector, n);

    for(int i = 0; i < n; i++) {
        for(int y = i + 1; y < n; y++) {
            double mult = tempMatrix[i][y]; // Como a diagonal da matriz L eh 1, nao precisamos dividir para conseguir o multiplicador
            for(int x = 0; x < n; x++) {
                tempMatrix[x][y] = tempMatrix[x][y] - tempMatrix[x][i] * mult;
            }
            tempBVector[y] = tempBVector[y] - tempBVector[i] * mult;
        }
    }

    copyVector(tempBVector, xVector, n); // copiar vetor temporario pro resultado

    //printMatrix(tempMatrix, n);
}

void solveAnBProblemBack(double** matrix, double* xVector, double* bVector, int n) {
    double** tempMatrix;
    double* tempBVector;

    initializeMatrix(&tempMatrix, n);
    initializeVector(&tempBVector, n);

    copyMatrix(matrix, tempMatrix, n);
    copyVector(bVector, tempBVector, n);

    for(int i = n - 1; i >= 0; i--) {
        // Fazer pivo=1
        double mult = tempMatrix[i][i];
        for(int x = 0; x < n; x++) {
            tempMatrix[x][i] = tempMatrix[x][i] / mult;
        }

        tempBVector[i] = tempBVector[i] / mult;

        for(int y = i - 1; y >= 0; y--) {
            mult = tempMatrix[i][y];
            for(int x = 0; x < n; x++) {
                tempMatrix[x][y] = tempMatrix[x][y] - tempMatrix[x][i] * mult;
            }
            tempBVector[y] = tempBVector[y] - tempBVector[i] * mult;
        }
    }

    copyVector(tempBVector, xVector, n);

    //printMatrix(tempMatrix, n);
}

int main()
{
    cout << "Digite tamanho da matriz: ";
    int n;
    cin >> n;

    double** matrix;
    double** matrixL;
    double** matrixU;
    double** matrixInv;

    double* xVector;
    double* bVector;
    double* dVector;


    initializeMatrix(&matrix, n);
    initializeMatrix(&matrixL, n);
    initializeMatrix(&matrixU, n);
    initializeMatrix(&matrixInv, n);

    fillMatrix(matrix, n);
    fillMatrix(matrixL, n);
    fillMatrix(matrixU, n);
    fillMatrix(matrixInv, n);

    initializeVector(&xVector, n);
    initializeVector(&bVector, n);
    initializeVector(&dVector, n);

    // Leitura dos valores da matriz
    for(int x = 0; x < n; x++) {
        for(int y = 0; y < n; y++) {
            cout << "Digite valor de (" << x << ", " << y << "): ";
            cin >> matrix[y][x];
        }
    }

    // Printa matriz na tela
    cout << endl << endl << "Matriz original:" << endl;
    printMatrix(matrix, n);

    LUDecomp(matrix, matrixL, matrixU, n);

    for(int i = 0; i < n; i++) {
        xVector[i] = 0;
    }

    cout << endl << endl << "Matrix L: " << endl;
    printMatrix(matrixL, n);

    cout << endl << endl << "Matrix U: " << endl;
    printMatrix(matrixU, n);

    for(int i = 0; i < n; i++) {

        for(int z = 0; z < n; z++) {
            if(z == i) {
                bVector[z] = 1;
            } else {
                bVector[z] = 0;
            }
        }

        //cout << endl << endl << "Matriz L resolvida para D: " << endl;
        solveAxBProblemForward(matrixL, dVector, bVector, n);

        //cout << endl << endl << "Matriz U resolvida para X: " << endl;
        solveAnBProblemBack(matrixU, xVector, dVector, n);

        // Incorpora o vetor resultado na matriz resultado
        for(int z = 0; z < n; z++) {
            matrixInv[i][z] = xVector[z];
        }

    }

    // Printa resultado
    cout << endl << endl << "Matriz Inversa: " << endl;
    printMatrix(matrixInv, n);

    return 0;
}
