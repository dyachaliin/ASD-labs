#include "graph.h"

void generateMatrix(int matrix[n][n], float koef) {
    srand(505);
    float T[n][n];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            T[i][j] = (rand() % 201) / 100.0;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float a = T[i][j] * koef;
            matrix[i][j] = a < 1.0 ? 0 : 1;
        }
    }
}

void simMatrix(int matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) {
                matrix[j][i] = 1;
            }
        }
    }
}

void showMatrix(int matrix[n][n], POINT start, HDC hdc) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            char sym[3];
            snprintf(sym, 3, "%d", matrix[i][j]);
            TextOutA(hdc, start.x + j * 20, start.y + i * 20, sym, 2 + (i > 9 ? 1 : 0));
        }
        printf("\n");
    }
}

void multiplyMatrixes(int a[n][n], int b[n][n], int result[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += a[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }
}

void matrixPow(int p, int a[n][n], int result[n][n]) {  
    int m[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            m[i][j] = a[i][j];
        }
    }
    
    for (int i = 1; i < p; i++) {
        int mCopy[n][n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                mCopy[i][j] = m[i][j];
            }
        }
        multiplyMatrixes(mCopy, a, m);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = m[i][j];
        }
    }
}

void transMatrix(int a[n][n]) {
    int m[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            m[i][j] = a[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = m[j][i];
        }
    }
}

void sumMatrix(int a[n][n], int b[n][n], int result[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

POINT arrowCoords(POINT from, POINT to) { // ?????????? ?????????? ??? ????? ??????? ???????
    float alpha = atan2(to.y - from.y, to.x - from.x);
    float distance = sqrt(pow((to.x - from.x), 2) + pow((to.y - from.y), 2));
    POINT arrow;
    arrow.x = from.x + (distance - radius - arrowRadius) * cos(alpha);
    arrow.y = from.y + (distance - radius - arrowRadius) * sin(alpha);
    return arrow;
}

POINT loopPOINT(POINT from, POINT to) { // ?????????? ?????????? ??? ????? ??????? ?????
    float alpha = atan2(to.y - from.y, to.x - from.x);
    float distance = sqrt(pow((to.x - from.x), 2) + pow((to.y - from.y), 2));
    POINT loop;
    loop.x = from.x + (distance + radius) * cos(alpha);
    loop.y = from.y + (distance + radius) * sin(alpha);
    return loop;
}

POINT middleDots(POINT from, POINT to) { // ?????????? ???????? ??? ?????????? ??????? (????????????? ?????)
    float alpha = atan2(to.y - from.y, to.x - from.x);
    POINT result;
    result.x = (radius * 3) * cos(PI / 2 - alpha);
    result.y = (radius * 5) * sin(PI / 2 - alpha);
    return result;
}
