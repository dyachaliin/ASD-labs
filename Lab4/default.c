#include "default.h"

LRESULT CALLBACK defaultGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    HDC hdc; //создаём контекст устройства
    PAINTSTRUCT ps; //создаём экземпляр структуры графического вывода

    float koef = 1 - 5 * 0.01 - 0.3;
    int A[n][n];
    generateMatrix(A, koef);

    int simA[n][n];
    generateMatrix(simA, koef);
    simMatrix(simA);


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

        int degrIn[n];
        int degrOut[n];
        int degr[n];
        
        for (int i = 0; i < n; i++) {
            degrIn[i] = 0;
            degrOut[i] = 0;
            degr[i] = 0;
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (A[i][j] == 1) {
                    degrOut[i]++;
                    degrIn[j]++;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= i; j++) {
                if (simA[i][j] == 1) {
                    degr[i]++;
                    degr[j]++;
                }
            }
        }

        bool isHomogen = true;
        int startH = degr[0];
        for (int i = 0; i < n; i++) {
            if (degr[i] != startH) isHomogen = false;
        }

        TextOutA(hdc, 10, 10, "Степени (вершина: степень):", 28);
        TextOutA(hdc, 10, 30, "Полустепени (вход):", 20);
        TextOutA(hdc, 10, 50, "Полустепени (выход):", 21);
        TextOutA(hdc, 10, 70, "Висячие:", 9);
        TextOutA(hdc, 10, 90, "Изолированные:", 15);
        TextOutA(hdc, 10, 110, isHomogen ? "Однородный" : "Не однородный", 14);

        int vis = 0;
        int iz = 0;

        for (int i = 0; i < n; i++) {
            char ar1[7];
            snprintf(ar1, 7, "%d: %d\t", i + 1, degr[i]);
            TextOutA(hdc, 210 + i * 60, 10, ar1, 5); // выводим связь

            char ar2[7];
            snprintf(ar2, 7, "%d: %d\t", i + 1, degrIn[i]);
            TextOutA(hdc, 210 + i * 60, 30, ar2, 5); // выводим связь

            char ar3[7];
            snprintf(ar3, 7, "%d: %d\t", i + 1, degrOut[i]);
            TextOutA(hdc, 210 + i * 60, 50, ar3, 5); // выводим связь

            if (degr[i] == 1) {
                char ar[2];
                snprintf(ar, 2, "%d\t", i + 1);
                TextOutA(hdc, 210 + vis * 60, 70, ar, 2); // выводим связь
                vis++;
            }

            if (degr[i] == 0) {
                char ar[2];
                snprintf(ar, 2, "%d\t", i + 1);
                TextOutA(hdc, 210 + iz * 60, 90, ar, 2); // выводим связь
                iz++;
            }
        }

        { // Выводим связи ориент. графа
            int counter = 0;
            int row = 0;
            for (int i = 0; i < n; i++) { // oriented
                for (int j = 0; j < n; j++) {
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

        VERTIC nonOrientedVerts[n];
        for (int i = 0; i < n; i++) { // создаём вершины неориентированного графа
            nonOrientedVerts[i].coords.x = verts[i].coords.x + (nonStartX - startX);
            nonOrientedVerts[i].coords.y = verts[i].coords.y;
            nonOrientedVerts[i].line = verts[i].line;
        }

        for (int i = 0; i < n; i++) { //non-oriented (отрисовка)
            for (int j = 0; j < n; j++) {
                SelectObject(hdc, KPen);
                POINT from = nonOrientedVerts[i].coords;
                POINT to = nonOrientedVerts[j].coords;
                if (simA[i][j] == 1 && to.x >= from.x) {
                    if (i == j) {
                        // строим петлю
                        POINT center;
                        center.x = nonStartX + side / 2;
                        center.y = startY - side * sqrt(3) / 4;
                        POINT loop = loopPOINT(center, to);
                        Arc(hdc, loop.x - loopRadius, loop.y - loopRadius, loop.x + loopRadius, loop.y + loopRadius, loop.x - loopRadius, loop.y - loopRadius, loop.x - loopRadius, loop.y - loopRadius);
                    }
                    else {
                        if (abs(i - j) == 1 || (nonOrientedVerts[i].line != nonOrientedVerts[j].line)) {
                            // строим прямую линию
                            MoveToEx(hdc, from.x, from.y, NULL);
                            LineTo(hdc, to.x, to.y);
                        }
                        else {
                            // ищем промежуточную точку
                            POINT move = middleDots(from, to);
                            float newX = (from.x + to.x) / 2;
                            float newY = (from.y + to.y) / 2;
                            newX += move.x;
                            newY -= move.y;
                            MoveToEx(hdc, from.x, from.y, NULL);
                            LineTo(hdc, newX, newY);
                            LineTo(hdc, to.x, to.y);
                        }
                    }

                }
            }
        }

        SelectObject(hdc, BPen);

        for (int i = 0; i < n; i++) { // отрисовка вершин
            char name[3];
            int symbolsToShow = i + 1 < 10 ? 1 : 2;
            snprintf(name, 3, "%d", i + 1);
            Ellipse(hdc, verts[i].coords.x - radius, verts[i].coords.y - radius, verts[i].coords.x + radius, verts[i].coords.y + radius);
            TextOutA(hdc, verts[i].coords.x - radius + 7 + (i + 1 < 10 ? 4 : 0), verts[i].coords.y - radius / 2, name, symbolsToShow);

            Ellipse(hdc, nonOrientedVerts[i].coords.x - radius, nonOrientedVerts[i].coords.y - radius, nonOrientedVerts[i].coords.x + radius, nonOrientedVerts[i].coords.y + radius);
            TextOutA(hdc, nonOrientedVerts[i].coords.x - radius + 7 + (i + 1 < 10 ? 4 : 0), nonOrientedVerts[i].coords.y - radius / 2, name, symbolsToShow);
        }



        EndPaint(hwnd, &ps);//малювання закінчене
        break;
    case WM_COMMAND:
        switch LOWORD(wParam) {

        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}