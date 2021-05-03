#include "trees.h"

LRESULT CALLBACK condensationGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc; //создаём контекст устройства
    PAINTSTRUCT ps; //создаём экземпляр структуры графического вывода

    float koef = 1 - 5 * 0.005 - 0.27;

    int A[n][n];
    generateMatrix(A, koef);

    int simA[n][n];
    generateMatrix(simA, koef);

    float P = 1500.0;
    float side = P / 3;

    // начальные координаты (1-й вершины)
    float startX = (width - side) / 2;
    float startY = width - (width - side * sqrt(3) / 2) / 2;

    // начальные координаты неориентированного графа
    float nonStartX = 2 * startX + side;
    switch (message) {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255)); // border
        HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5)); // line

        int dos[n][n];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                dos[i][j] = i == j ? 1 : 0;
            }
        }

        int A2[n][n];
        matrixPow(2, A, A2);

        int A3[n][n];
        matrixPow(3, A, A3);

        for (int i = 1; i <= n; i++) {
            int currMatrix[n][n];
            matrixPow(i, A, currMatrix);
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    if (currMatrix[j][k] != 0) {
                        dos[j][k] = 1;
                    }
                }
            }
        }
        //char name[5];
        //snprintf(name, 5, "(%d)", N);
        //TextOutA(hdc, 10 + 14 * 8, 10, name, 5);

        int dosT[n][n];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                dosT[i][j] = dos[i][j];
            }
        }

        transMatrix(dosT);

        int zvMatrix[n][n];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                zvMatrix[i][j] = dos[i][j] * dosT[i][j];
            }
        }

        int V[n][n];
        int usedArr[n];
        int usedIndex = 0;
        int vIndex = 0;

        for (int i = 0; i < n; i++) {
            usedArr[i] = 0;
            for (int j = 0; j < n; j++) {
                V[i][j] = 0;
            }
        }

        for (int i = 0; i < n; i++) {
            bool isUsed = false;
            for (int j = 0; j < n; j++) {
                if (usedArr[j] == i + 1) isUsed = true;
            }

            if (isUsed) continue;

            int currI = 0;
            for (int j = i; j < n; j++) {
                if (zvMatrix[i][j] != 0) {
                    usedArr[usedIndex++] = j + 1;
                    V[vIndex][currI++] = j + 1;
               }
            }
            vIndex++;
        }

        int componentsN = 0;

        for (int i = 0; i < n; i++) {
            if(V[i][0] != 0) componentsN++;
        }

        SelectObject(hdc, BPen);

        VERTIC verts[n]; // array of vertices coordinates

        float step = P / componentsN;

        // calculate vertices coordinates
        {
            float newX = startX;
            float newY = startY;
            int vertCounter = 0;
            float left = 0;

            for (float curMargin = 0; curMargin <= side; curMargin += step) { // двигаемся по первой стороне треугольника (под углом 60 градусов вверх)
                verts[vertCounter].coords.x = newX;
                verts[vertCounter].coords.y = newY;
                verts[vertCounter].line = 1;
                newX += step * cos(PI / 3);
                newY += -step * sin(PI / 3);
                left = side - curMargin;
                vertCounter++;
            } // двигаемся по первой стороне треугольника

            newX = startX + side / 2;
            newY = startY - side * sin(PI / 3);

            newX += left * cos(PI / 3);
            newY += left * sin(PI / 3);

            for (float curMargin = left; curMargin <= side; curMargin += step) { // двигаемся по второй стороне треугольника (под углом 60 градусов вниз)
                verts[vertCounter].coords.x = newX;
                verts[vertCounter].coords.y = newY;
                verts[vertCounter].line = 2;
                newX += step * cos(PI / 3);
                newY += step * sin(PI / 3);
                left = side - curMargin;
                vertCounter++;
            } // двигаемся по перыой стороне треугольника

            newX = startX + side;
            newY = startY;

            for (; vertCounter < componentsN; vertCounter++) { // двигаемся по основанию треугольника
                newX -= step;
                verts[vertCounter].coords.x = newX;
                verts[vertCounter].coords.y = newY;
                verts[vertCounter].line = 3;
            } // двигаемся по основанию треугольника

        }

        int condMatrix[n][n];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i > componentsN - 1 || j > componentsN - 1)
                    condMatrix[i][j] = -1;
                else condMatrix[i][j] = 0;
            }
        }

        int vis = 0;
        int iz = 0;
  
        for (int i = 0; i < componentsN; i++) {
            char ar[17];
            snprintf(ar, 17, "Компонента %d:\t", i + 1);
            TextOutA(hdc, 10, 10 + i * 20, ar, 15);
            for (int j = 0; V[i][j] != 0; j++) {
                char ar2[4];
                snprintf(ar2, 4, "%d, ", V[i][j]);
                TextOutA(hdc, 120 + j * 20, 10 + i * 20, ar2, 4);
                int currVert = V[i][j];
                // для каждого элемента компоненты пробежимся по другим компонентам
                for (int k = 0; k < componentsN; k++) {
                    if(k != i) {
                        for (int to = 0; V[k][to] != 0; to++) {
                            int toVert = V[k][to];
                            if (A[currVert - 1][toVert - 1] != 0) {
                                condMatrix[i][k] = 1;
                            }
                        }
                    }
                }
            }
        }


        { // Выводим связи ориент. графа
            for (int i = 0; i < componentsN; i++) { // oriented
                for (int j = 0; j < componentsN; j++) {
                    HPEN newPen = CreatePen(PS_SOLID, 1, RGB(rand() % 200, rand() % 200, rand() % 200)); // line
                    SelectObject(hdc, newPen);
                    POINT from = verts[i].coords;
                    POINT to = verts[j].coords;
                    if (condMatrix[i][j] == 1) { // если есть связь
                        if (i == j) { // петля
                            SelectObject(hdc, KPen);
                            POINT center;
                            center.x = startX + side / 2;
                            center.y = startY - side * sqrt(3) / 4;
                            POINT loop = loopPOINT(center, to);
                            Arc(hdc, loop.x - loopRadius, loop.y - loopRadius, loop.x + loopRadius, loop.y + loopRadius, loop.x - loopRadius, loop.y - loopRadius, loop.x - loopRadius, loop.y - loopRadius); // рисуеи петлю
                        }
                        else {
                            MoveToEx(hdc, from.x, from.y, NULL);
                            LineTo(hdc, to.x, to.y); // рисуем прямую линию
                            POINT arrow = arrowCoords(from, to);
                            POINT vertices[] = { {0, 0}, {0, 0}, {0, 0} };
                            float alpha = atan2(to.y - from.y, to.x - from.x);
                            for (int i = 0; i < 3; i++) {
                                vertices[i].x = arrowRadius * cos(alpha) + arrow.x;
                                vertices[i].y = arrowRadius * sin(alpha) + arrow.y;
                                alpha += (1.0 / 3.0) * (2 * PI);
                            }
                            Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0])); // рисуем стрелу
                        }
                    }
                }
            } // oriented

        }

        for (int i = 0; i < n; i++) { // отрисовка вершин
            char name[3];
            int symbolsToShow = i + 1 < 10 ? 1 : 2;
            snprintf(name, 3, "%d", i + 1);
            Ellipse(hdc, verts[i].coords.x - radius, verts[i].coords.y - radius, verts[i].coords.x + radius, verts[i].coords.y + radius);
            TextOutA(hdc, verts[i].coords.x - radius + 7 + (i + 1 < 10 ? 4 : 0), verts[i].coords.y - radius / 2, name, symbolsToShow);
        }

        

        TextOutA(hdc, nonStartX - 170, 100, "Матрица смежности:", 19);
        POINT matrixA = { nonStartX, 100 } ;
        showMatrix(A, matrixA, hdc);

        TextOutA(hdc, nonStartX + 220, 100, "Матрица достижимости:", 22);
        POINT matrixDos = { nonStartX + 400, 100 };
        showMatrix(dos, matrixDos, hdc);

        TextOutA(hdc, nonStartX - 170, 350, "Матрица связности:", 19);
        POINT matrixZv = { nonStartX, 350 };
        showMatrix(zvMatrix, matrixZv, hdc);

        TextOutA(hdc, nonStartX + 220, 350, "Матрица гр. конд.:", 19);
        POINT matrixCd = { nonStartX + 400, 350 };
        showMatrix(condMatrix, matrixCd, hdc);


        EndPaint(hwnd, &ps);//малювання закінчене
        break;
    case WM_COMMAND:
        switch LOWORD(wParam) {

        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}