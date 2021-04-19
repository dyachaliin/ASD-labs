#pragma once

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <io.h>  

#define width 750
#define n 10
#define PI 3.14159265359
#define radius 14 // радиус вершины
#define arrowRadius 6 // радиус стрелки (описанной окружности)
#define loopRadius 8 // радиус петли

void generateMatrix(int[n][n], float);

void simMatrix(int [n][n]);

void showMatrix(int [n][n], POINT, HDC);

void multiplyMatrixes(int[n][n], int[n][n], int[n][n]);

void matrixPow(int, int[n][n], int[n][n]);

void transMatrix(int[n][n]);

void sumMatrix(int[n][n], int[n][n], int[n][n]);

typedef struct VERTIC {
    POINT coords;
    int line;
} VERTIC;

POINT arrowCoords(POINT, POINT);

POINT loopPOINT(POINT, POINT);

POINT middleDots(POINT, POINT);
