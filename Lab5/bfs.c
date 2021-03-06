#include "bfs.h"

void bfs(int[n][n], int, int[n]);

int globalCounter = 0;
int pog = 0;
int globalV = 0;
int bfsMatrix[n * n][2];

#define buttonMenu 102

HWND button;

LRESULT CALLBACK modifiedGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc; //??????? ???????? ??????????
    PAINTSTRUCT ps; //??????? ????????? ????????? ???????????? ??????

    float koef = 1 - 5 * 0.005 - 0.15;

    int A[n][n];
    generateMatrix(A, koef);

    int simA[n][n];
    generateMatrix(simA, koef);

    int startV = 0;
    int visited[n];
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
    }
    visited[startV] = 1;
    globalV = startV;

    bfs(A, startV, visited);
    globalCounter = 0;

    int Tree[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Tree[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        int from = bfsMatrix[i][0];
        int to = bfsMatrix[i][1];
        Tree[from][to] = 1;
    }

    float P = 1500.0;
    float side = P / 3;
    const step = P / n;

    // ????????? ?????????? (1-? ???????)
    float startX = (width - side) / 2;
    float startY = width - (width - side * sqrt(3) / 2) / 2;

    // ????????? ?????????? ?????????????????? ?????
    float nonStartX = 2 * startX + side;
    switch (message) {
    case WM_CREATE:
        for (int i = 0; i < n * n; i++) {
            bfsMatrix[i][0] = -1;
            bfsMatrix[i][1] = -1;
        }
        button = CreateWindowEx(
            0,
            L"BUTTON",
            L"NEXT",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
            25,
            25,
            150,
            24,
            hwnd,
            (HMENU)buttonMenu,
            GetModuleHandle(NULL),
            NULL);
        break;
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

            for (float curMargin = 0; curMargin <= side; curMargin += step) { // ????????? ?? ?????? ??????? ???????????? (??? ????? 60 ???????? ?????)
                verts[vertCounter].coords.x = newX;
                verts[vertCounter].coords.y = newY;
                verts[vertCounter].line = 1;
                newX += step * cos(PI / 3);
                newY += -step * sin(PI / 3);
                left = side - curMargin;
                vertCounter++;
            } // ????????? ?? ?????? ??????? ????????????

            newX = startX + side / 2;
            newY = startY - side * sin(PI / 3);

            newX += left * cos(PI / 3);
            newY += left * sin(PI / 3);

            for (float curMargin = left; curMargin <= side; curMargin += step) { // ????????? ?? ?????? ??????? ???????????? (??? ????? 60 ???????? ????)
                verts[vertCounter].coords.x = newX;
                verts[vertCounter].coords.y = newY;
                verts[vertCounter].line = 2;
                newX += step * cos(PI / 3);
                newY += step * sin(PI / 3);
                left = side - curMargin;
                vertCounter++;
            } // ????????? ?? ?????? ??????? ????????????

            newX = startX + side;
            newY = startY;

            for (; vertCounter < n; vertCounter++) { // ????????? ?? ????????? ????????????
                newX -= step;
                verts[vertCounter].coords.x = newX;
                verts[vertCounter].coords.y = newY;
                verts[vertCounter].line = 3;
            } // ????????? ?? ????????? ????????????

        }

        { // ??????? ????? ??????. ?????
            int counter = 0;
            int row = 0;

            HPEN bigPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0)); // line
            HPEN bigPenLast = CreatePen(PS_SOLID, 2, RGB(144, 0, 255)); // line
            for (int i = 0; i < n; i++) { // oriented
                for (int j = 0; j < n; j++) {
                    HPEN newPen = CreatePen(PS_SOLID, 1, RGB(rand() % 200, rand() % 200, rand() % 200)); // line
                    SelectObject(hdc, newPen);
                    POINT from = verts[i].coords;
                    POINT to = verts[j].coords;
                    if (A[i][j] == 1) { // ???? ???? ?????
                        for (int k = 0; k < pog; k++) {
                            int fromBig = bfsMatrix[k][0];
                            int toBig = bfsMatrix[k][1];
                            if (i == fromBig && j == toBig) SelectObject(hdc, bigPenLast);
                        }
                        int fromBig = bfsMatrix[pog][0];
                        int toBig = bfsMatrix[pog][1];
                        if (fromBig == -1 && toBig == -1) {
                            EnableWindow(button, false);
                        }
                        if (i == fromBig && j == toBig) SelectObject(hdc, bigPen);
                        
                        if (i == j) { // ?????
                            SelectObject(hdc, KPen);
                            POINT center;
                            center.x = startX + side / 2;
                            center.y = startY - side * sqrt(3) / 4;
                            POINT loop = loopPOINT(center, to);
                            Arc(hdc, loop.x - loopRadius, loop.y - loopRadius, loop.x + loopRadius, loop.y + loopRadius, loop.x - loopRadius, loop.y - loopRadius, loop.x - loopRadius, loop.y - loopRadius); // ?????? ?????
                        }
                        else {
                            if ((abs(i - j) == 1 || (verts[i].line != verts[j].line && i != 0 && j != 0)) && (i < j || A[j][i] == 0)) { // ???? ???????? ??????? ??? ?? ?? ????? ??????? ????????????
                                MoveToEx(hdc, from.x, from.y, NULL);
                                LineTo(hdc, to.x, to.y); // ?????? ?????? ?????
                                POINT arrow = arrowCoords(from, to);
                                POINT vertices[] = { {0, 0}, {0, 0}, {0, 0} };
                                float alpha = atan2(to.y - from.y, to.x - from.x);
                                for (int i = 0; i < 3; i++) {
                                    vertices[i].x = arrowRadius * cos(alpha) + arrow.x;
                                    vertices[i].y = arrowRadius * sin(alpha) + arrow.y;
                                    alpha += (1.0 / 3.0) * (2 * PI);
                                }
                                Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0])); // ?????? ??????
                            }
                            else {
                                POINT move = middleDots(from, to); // ??????? ????????????? ?????
                                float newX = (from.x + to.x) / 2;
                                float newY = (from.y + to.y) / 2;
                                newX -= move.x;
                                newY += move.y;
                                MoveToEx(hdc, from.x, from.y, NULL);
                                LineTo(hdc, newX, newY);  // ??????
                                LineTo(hdc, to.x, to.y); //  ?????
                                POINT fromC = { newX, newY };
                                POINT arrow = arrowCoords(fromC, to);
                                POINT vertices[] = { {0, 0}, {0, 0}, {0, 0} };
                                float alpha = atan2(to.y - fromC.y, to.x - fromC.x);
                                for (int i = 0; i < 3; i++) {
                                    vertices[i].x = arrowRadius * cos(alpha) + arrow.x;
                                    vertices[i].y = arrowRadius * sin(alpha) + arrow.y;
                                    alpha += (1.0 / 3.0) * (2 * PI);
                                }
                                Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0])); // ?????? ??????
                            }
                        }
                        counter += i < 9 ? 45 : 55;
                        if (120 + counter > width + 600) { // ????? ?? ????? ????
                            row += 30;
                            counter = 0;
                        }
                    }
                }
            } // oriented

        }

        SelectObject(hdc, BPen);

        for (int i = 0; i < n; i++) { // ????????? ??????
            char name[3];
            int symbolsToShow = i + 1 < 10 ? 1 : 2;
            snprintf(name, 3, "%d", i + 1);
            Ellipse(hdc, verts[i].coords.x - radius, verts[i].coords.y - radius, verts[i].coords.x + radius, verts[i].coords.y + radius);
            TextOutA(hdc, verts[i].coords.x - radius + 7 + (i + 1 < 10 ? 4 : 0), verts[i].coords.y - radius / 2, name, symbolsToShow);
        }

        char name[3];
        POINT to = verts[startV].coords;

        snprintf(name, 3, "%d", startV + 1);
        TextOutA(hdc, to.x + 10, to.y, name, 2);

        for (int i = 1; i < n; i++) { // ????????? ??????
            char name[3];
            int newN = 0;
            for (int j = 0; bfsMatrix[j][0] != -1; j++) {
                if (bfsMatrix[j][1] == i) {
                    newN = j;
                    break;
                }
            }
            POINT to = verts[i].coords;

            snprintf(name, 3, "%d", newN + 2);
            TextOutA(hdc, to.x + 10, to.y, name, 2);
        }

        TextOutA(hdc, nonStartX - 85, 100, "Matrix:", 7);
        POINT matrixA = { nonStartX, 100 };
        showMatrix(A, matrixA, hdc);

        TextOutA(hdc, nonStartX + 300, 100, "Tree:", 5);
        POINT matrixA2 = { nonStartX + 400, 100 };
        showMatrix(Tree, matrixA2, hdc);

        // TextOutA(hdc, nonStartX + 300, 100, "A^2:", 4);
        // POINT matrixA2 = { nonStartX + 400, 100 };
        // showMatrix(A2, matrixA2, hdc);
        // 
        // TextOutA(hdc, nonStartX - 90, 350, "A^3:", 4);
        // POINT matrixA3 = { nonStartX, 350 };
        // showMatrix(A3, matrixA3, hdc);

        
        EndPaint(hwnd, &ps);//????????? ?????????
        break;
    case WM_COMMAND:
        switch LOWORD(wParam) {
        case buttonMenu: 
            {
                pog++;
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateWindow(hwnd);
                break;
            }  

        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void bfs(int matrix[n][n], int curr, int visited[n]) {
    DWORD dwSize = 0;
    if (curr != -1) {
        for (int i = 0; i < n; i++) {
            if (matrix[curr][i] != 0) {
                if (visited[i] == 0) {
                    bfsMatrix[globalCounter][0] = curr;
                    bfsMatrix[globalCounter][1] = i;

                    globalCounter++;
                    visited[i] = 1;
                }
            }
        }
        bfs(matrix, -1, visited);
    }
    else {
        bool anyWays = false;
        for (int from = 0; from < n; from++) {
            for (int to = 0; to < n; to++) {
                if (visited[from] != 0 && visited[to] == 0 && matrix[from][to] != 0) {
                    bfsMatrix[globalCounter][0] = from;
                    bfsMatrix[globalCounter][1] = to;
                    globalCounter++;
                    anyWays = true;
                    visited[to] = 1;
                }
            }
        }
        if (anyWays) bfs(matrix, -1, visited);
    }
    
}