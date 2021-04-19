#include "modified.h"

LRESULT CALLBACK modifiedGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    HDC hdc; //создаём контекст устройства
    PAINTSTRUCT ps; //создаём экземпляр структуры графического вывода

    float koef = 1 - 5 * 0.005 - 0.27;

    int A[n][n];
    generateMatrix(A, koef);

    int simA[n][n];
    generateMatrix(simA, koef);

    float P = 1500.0;
    float side = P / 3;
    const step = P / n;

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

        system("cls");

        VERTIC verts[n]; // array of vertices coordinates

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

            for (; vertCounter < n; vertCounter++) { // двигаемся по основанию треугольника
                newX -= step;
                verts[vertCounter].coords.x = newX;
                verts[vertCounter].coords.y = newY;
                verts[vertCounter].line = 3;
            } // двигаемся по основанию треугольника

        }

        { // Выводим связи ориент. графа
            int counter = 0;
            int row = 0;
            for (int i = 0; i < n; i++) { // oriented
                for (int j = 0; j < n; j++) {
                    HPEN newPen = CreatePen(PS_SOLID, 1, RGB(rand() % 200, rand() % 200, rand() % 200)); // line
                    SelectObject(hdc, newPen);
                    POINT from = verts[i].coords;
                    POINT to = verts[j].coords;
                    if (A[i][j] == 1) { // если есть связь
                        if (i == j) { // петля
                            SelectObject(hdc, KPen);
                            POINT center;
                            center.x = startX + side / 2;
                            center.y = startY - side * sqrt(3) / 4;
                            POINT loop = loopPOINT(center, to);
                            Arc(hdc, loop.x - loopRadius, loop.y - loopRadius, loop.x + loopRadius, loop.y + loopRadius, loop.x - loopRadius, loop.y - loopRadius, loop.x - loopRadius, loop.y - loopRadius); // рисуеи петлю
                        }
                        else {
                            if ((abs(i - j) == 1 || verts[i].line != verts[j].line) && (i < j || A[j][i] == 0)) { // если соседние вершины или не на одной стороне треугольника
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
                            else {
                                POINT move = middleDots(from, to); // находим промежуточную точку
                                float newX = (from.x + to.x) / 2;
                                float newY = (from.y + to.y) / 2;
                                newX += move.x;
                                newY -= move.y;
                                MoveToEx(hdc, from.x, from.y, NULL);
                                LineTo(hdc, newX, newY);  // Рисуем
                                LineTo(hdc, to.x, to.y); //  Связь
                                POINT fromC = { newX, newY };
                                POINT arrow = arrowCoords(fromC, to);
                                POINT vertices[] = { {0, 0}, {0, 0}, {0, 0} };
                                float alpha = atan2(to.y - fromC.y, to.x - fromC.x);
                                for (int i = 0; i < 3; i++) {
                                    vertices[i].x = arrowRadius * cos(alpha) + arrow.x;
                                    vertices[i].y = arrowRadius * sin(alpha) + arrow.y;
                                    alpha += (1.0 / 3.0) * (2 * PI);
                                }
                                Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0])); // рисуем стрелу
                            }
                        }
                        counter += i < 9 ? 45 : 55;
                        if (120 + counter > width + 600) { // вышли за конец окна
                            row += 30;
                            counter = 0;
                        }
                    }
                }
            } // oriented

        }

        int A2[n][n];
        matrixPow(2, A, A2);

        int A3[n][n];
        matrixPow(3, A, A3);

        { // cчитаем и выводим пути     
            int ways2[n * n][3];
            int done = 0;
            for (int i = 0; i < n * n; i++) { // создаём матрицу для путей
                for (int j = 0; j < 3; j++) {
                    ways2[i][j] = 0;
                }
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    int waysN = A2[i][j];
                    if (waysN != 0) {
                        for (int k = 0; k < n; k++) {
                            if (A[i][k] && A[k][j]) {
                                ways2[done][0] = i + 1;
                                ways2[done][1] = k + 1;
                                ways2[done][2] = j + 1;      
                                done++;
                            } 
                        }
                    }
                }
            }

            int ways3[n * n * n][4];
            for (int i = 0; i < n * n * n; i++) { // создаём матрицу для путей
                for (int j = 0; j < 4; j++) {
                    ways3[i][j] = 0;
                }
            }

            done = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    int waysN = A3[i][j];
                    if (waysN != 0) {
                        for (int k = 0; ways2[k][0] <= i + 1; k++) {
                            if (ways2[k][0] == i + 1) {
                                int a = i + 1;
                                int b = ways2[k][1];
                                int c = ways2[k][2];
                                int d = j + 1;
                                if (A[c - 1][d - 1] != 0) {
                                    ways3[done][0] = a;
                                    ways3[done][1] = b;
                                    ways3[done][2] = c;
                                    ways3[done][3] = d;
                                    done++;
                                }
                                
                            }
                        }
                    }
                }
            }

            int wayX = 0;

            LPCWSTR lpBuff = L"Пути длинной 2:\n";
            DWORD dwSize = 0;
            WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), lpBuff, 17, &dwSize, NULL);

            for (int i = 0; ways2[i][0] != 0; i++) {
                char name[14];
                int symbolsToShow = 6;
                int d1 = ways2[i][0];
                int d2 = ways2[i][1];
                int d3 = ways2[i][2];
                while (d1 > 0) {
                    d1 /= 10;
                    symbolsToShow++;
                }
                while (d2 > 0) {
                    d2 /= 10;
                    symbolsToShow++;
                }
                while (d3 > 0) {
                    d3 /= 10;
                    symbolsToShow++;
                }
                snprintf(name, 14, "%d->%d->%d \t", ways2[i][0], ways2[i][1], ways2[i][2]);
                wchar_t wString[14];
                MultiByteToWideChar(CP_ACP, 0, name, -1, wString, 14);
                WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), wString, symbolsToShow, &dwSize, NULL);

                wayX++;
                if (wayX % 6 == 0) {
                    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), "\n", 1, &dwSize, NULL);
                }
            }

            LPCWSTR lpBuff2 = L"\nПути длинной 3:\n";
            WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), lpBuff2, 18, &dwSize, NULL);
            wayX = 0;
            for (int i = 0; ways3[i][0] != 0; i++) {
                char name[18];
                int symbolsToShow = 8;
                int d1 = ways3[i][0];
                int d2 = ways3[i][1];
                int d3 = ways3[i][2];
                int d4 = ways3[i][3];
                while (d1 > 0) {
                    d1 /= 10;
                    symbolsToShow++;
                }
                while (d2 > 0) {
                    d2 /= 10;
                    symbolsToShow++;
                }
                while (d3 > 0) {
                    d3 /= 10;
                    symbolsToShow++;
                }
                while (d4 > 0) {
                    d4 /= 10;
                    symbolsToShow++;
                }
                snprintf(name, 18, "%d->%d->%d->%d \t", ways3[i][0], ways3[i][1], ways3[i][2], ways3[i][3]);
                wchar_t wString[18];
                MultiByteToWideChar(CP_ACP, 0, name, -1, wString, 18);
                WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), wString, symbolsToShow, &dwSize, NULL);

                wayX++;
                if (wayX % 6 == 0) {
                    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), "\n", 1, &dwSize, NULL);
                }
            }
        }

        

        //char name[5];
        //snprintf(name, 5, "(%d)", N);
        //TextOutA(hdc, 10 + 14 * 8, 10, name, 5);

        SelectObject(hdc, BPen);

        for (int i = 0; i < n; i++) { // отрисовка вершин
            char name[3];
            int symbolsToShow = i + 1 < 10 ? 1 : 2;
            snprintf(name, 3, "%d", i + 1);
            Ellipse(hdc, verts[i].coords.x - radius, verts[i].coords.y - radius, verts[i].coords.x + radius, verts[i].coords.y + radius);
            TextOutA(hdc, verts[i].coords.x - radius + 7 + (i + 1 < 10 ? 4 : 0), verts[i].coords.y - radius / 2, name, symbolsToShow);
        }
        
        TextOutA(hdc, nonStartX - 50, 200, "Modified matrix:", 17);
        POINT matrixText = { nonStartX + 70, 200 };
        showMatrix(A, matrixText, hdc);
        

        EndPaint(hwnd, &ps);//малювання закінчене
        LockWindowUpdate(hwnd);
        break;
    case WM_COMMAND:
        switch LOWORD(wParam) {

        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}