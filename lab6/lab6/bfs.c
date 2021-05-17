#include "bfs.h"
#define INF 9999999

void bfs(int[n][n], int, int[n]);
void wMatrix(int[n][n]);
void prims(int[n][n], int);

int globalCounter = 0;
int pog = 0;
int globalV = 0;
int primMatrix[n * n][2];
int Wt[n][n];
int B[n][n];

#define buttonMenu 102

HWND button;

void wMatrix(int A[n][n]) {
    int num, C[n][n], D[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            num = roundf((rand() / (float)RAND_MAX * 2) * 100) * A[i][j];
            Wt[i][j] = num;
            if (num == 0) B[i][j] = 0;
            else B[i][j] = 1;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (B[i][j] != B[j][i]) C[i][j] = 1;
            else C[i][j] = 0;

            if (B[i][j] == B[j][i] && B[i][j] == 1 && j <= i) D[i][j] = 1;
            else D[i][j] = 0;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Wt[i][j] = i == j ? 0 : (C[i][j] + D[i][j]) * Wt[i][j];
        }
    }
    simMatrix(Wt);
}

LRESULT CALLBACK modifiedGraphProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc; //ñîçäà¸ì êîíòåêñò óñòðîéñòâà
    PAINTSTRUCT ps; //ñîçäà¸ì ýêçåìïëÿð ñòðóêòóðû ãðàôè÷åñêîãî âûâîäà

    float koef = 1 - 5 * 0.005 - 0.05;

    int A[n][n];
    generateMatrix(A, koef);

    int simA[n][n];
    generateMatrix(simA, koef);

    simMatrix(simA);
    wMatrix(A);

    int startV = 7;
    globalV = startV;

    //bfs(A, startV, visited);
    globalCounter = 0;

    prims(Wt, startV);

    int Tree[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Tree[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        int from = primMatrix[i][0];
        int to = primMatrix[i][1];
        Tree[from][to] = 1;
    }

    float P = 1500.0;
    float side = P / 3;
    const step = P / n;

    // íà÷àëüíûå êîîðäèíàòû (1-é âåðøèíû)
    float startX = (width - side) / 2;
    float startY = width - (width - side * sqrt(3) / 2) / 2;

    // íà÷àëüíûå êîîðäèíàòû íåîðèåíòèðîâàííîãî ãðàôà
    float nonStartX = 2 * startX + side;
    switch (message) {
    case WM_CREATE:
        for (int i = 0; i < n * n; i++) {
            primMatrix[i][0] = -1;
            primMatrix[i][1] = -1;
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

            for (float curMargin = 0; curMargin <= side; curMargin += step) { // äâèãàåìñÿ ïî ïåðâîé ñòîðîíå òðåóãîëüíèêà (ïîä óãëîì 60 ãðàäóñîâ ââåðõ)
                verts[vertCounter].coords.x = newX;
                verts[vertCounter].coords.y = newY;
                verts[vertCounter].line = 1;
                newX += step * cos(PI / 3);
                newY += -step * sin(PI / 3);
                left = side - curMargin;
                vertCounter++;
            } // äâèãàåìñÿ ïî ïåðâîé ñòîðîíå òðåóãîëüíèêà

            newX = startX + side / 2;
            newY = startY - side * sin(PI / 3);

            newX += left * cos(PI / 3);
            newY += left * sin(PI / 3);

            for (float curMargin = left; curMargin <= side; curMargin += step) { // äâèãàåìñÿ ïî âòîðîé ñòîðîíå òðåóãîëüíèêà (ïîä óãëîì 60 ãðàäóñîâ âíèç)
                verts[vertCounter].coords.x = newX;
                verts[vertCounter].coords.y = newY;
                verts[vertCounter].line = 2;
                newX += step * cos(PI / 3);
                newY += step * sin(PI / 3);
                left = side - curMargin;
                vertCounter++;
            } // äâèãàåìñÿ ïî ïåðûîé ñòîðîíå òðåóãîëüíèêà

            newX = startX + side;
            newY = startY;

            for (; vertCounter < n; vertCounter++) { // äâèãàåìñÿ ïî îñíîâàíèþ òðåóãîëüíèêà
                newX -= step;
                verts[vertCounter].coords.x = newX;
                verts[vertCounter].coords.y = newY;
                verts[vertCounter].line = 3;
            } // äâèãàåìñÿ ïî îñíîâàíèþ òðåóãîëüíèêà

        }

        { // Âûâîäèì ñâÿçè îðèåíò. ãðàôà
            

            HPEN bigPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0)); // line
            HPEN bigPenLast = CreatePen(PS_SOLID, 2, RGB(144, 0, 255)); // line
           
            
           

            for (int i = 0; i < n; i++) { // oriented
                for (int j = 0; j < n; j++) {
                    HPEN newPen = CreatePen(PS_SOLID, 1, RGB(rand() % 200, rand() % 200, rand() % 200)); // line
                    SelectObject(hdc, newPen);
                    POINT from = verts[i].coords;
                    POINT to = verts[j].coords;
                    if (simA[i][j] == 1 && to.x >= from.x) { // åñëè åñòü ñâÿçü


                        for (int k = 0; k < pog; k++) {
                            int fromBig = primMatrix[k][0];
                            int toBig = primMatrix[k][1];
                            if ((i == fromBig && j == toBig) || (j == fromBig && i == toBig)) SelectObject(hdc, bigPenLast);
                        }
                        int fromBig = primMatrix[pog][0];
                        int toBig = primMatrix[pog][1];
                        if (fromBig == -1 && toBig == -1) {
                            EnableWindow(button, false);
                        }


                        if ((i == fromBig && j == toBig) || (j == fromBig && i == toBig)) SelectObject(hdc, bigPen);
                        if (i == j) { // ïåòëÿ
                            //SelectObject(hdc, KPen);
                            POINT center;
                            center.x = startX + side / 2;
                            center.y = startY - side * sqrt(3) / 4;
                            POINT loop = loopPOINT(center, to);
                            Arc(hdc, loop.x - loopRadius, loop.y - loopRadius, loop.x + loopRadius, loop.y + loopRadius, loop.x - loopRadius, loop.y - loopRadius, loop.x - loopRadius, loop.y - loopRadius); // ðèñóåè ïåòëþ
                        }
                        else {
                            if (abs(i - j) == 1 || (verts[i].line != verts[j].line)) { // åñëè ñîñåäíèå âåðøèíû èëè íå íà îäíîé ñòîðîíå òðåóãîëüíèêà
                                if (i == 0 && j == 8) {
                                    POINT move = middleDots(from, to); // íàõîäèì ïðîìåæóòî÷íóþ òî÷êó
                                    float newX = (from.x + to.x) / 2;
                                    float newY = (from.y + to.y) / 2;
                                    newX += move.x;
                                    newY -= move.y;
                                    MoveToEx(hdc, from.x, from.y, NULL);
                                    LineTo(hdc, newX, newY);  // Ðèñóåì
                                    LineTo(hdc, to.x, to.y); //  Ñâÿçü
                                    char name[4];

                                    snprintf(name, 4, "%d", Wt[i][j]);
                                    TextOutA(hdc, newX, newY, name, 3);
                                    continue;
                                }
                                MoveToEx(hdc, from.x, from.y, NULL);
                                LineTo(hdc, to.x, to.y); // ðèñóåì ïðÿìóþ ëèíèþ
                                float newX = (from.x + to.x) / 2;
                                float newY = (from.y + to.y) / 2;
                                char name[5];

                                int num = Wt[i][j];
                                int symbs = 0;
                                while (num != 0) {
                                    num /= 10;
                                    symbs++;
                                }
                                if (symbs == 0) symbs = 1;

                                if (i == 0 && j == 5) {
                                    snprintf(name, symbs + 1, "%d", Wt[i][j]);
                                    TextOutA(hdc, newX, newY - 25, name, symbs);
                                } else  if (i == 0 && j == 6) {
                                    snprintf(name, symbs + 1, "%d", Wt[i][j]);
                                    TextOutA(hdc, newX + 45, newY - 20, name, symbs);
                                } else if (i == 2 && j == 6) {
                                    snprintf(name, symbs + 1, "%d", Wt[i][j]);
                                    TextOutA(hdc, newX - 25, newY, name, symbs);
                                }
                                else {
                                    snprintf(name, symbs + 1, "%d", Wt[i][j]);
                                    TextOutA(hdc, newX, newY, name, symbs);
                                }

                               
                                
                                //snprintf(name, 5, "%d", Wt[i][j]);
                                //TextOutA(hdc, newX, newY, name, 2);
                               // POINT arrow = arrowCoords(from, to);
                               // POINT vertices[] = { {0, 0}, {0, 0}, {0, 0} };
                               // float alpha = atan2(to.y - from.y, to.x - from.x);
                               // for (int i = 0; i < 3; i++) {
                               //     vertices[i].x = arrowRadius * cos(alpha) + arrow.x;
                               //     vertices[i].y = arrowRadius * sin(alpha) + arrow.y;
                               //     alpha += (1.0 / 3.0) * (2 * PI);
                               // }
                               // Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0])); // ðèñóåì ñòðåëó
                            }
                            else {
                                POINT move = middleDots(from, to); // íàõîäèì ïðîìåæóòî÷íóþ òî÷êó
                                float newX = (from.x + to.x) / 2;
                                float newY = (from.y + to.y) / 2; 
                                newX += move.x;
                                newY -= move.y;
                                MoveToEx(hdc, from.x, from.y, NULL);
                                LineTo(hdc, newX, newY);  // Ðèñóåì
                                LineTo(hdc, to.x, to.y); //  Ñâÿçü
                                char name[4];

                                snprintf(name, 4, "%d", Wt[i][j]);
                                TextOutA(hdc, newX, newY, name, 3);
                             
                   
                                
                                
                               // POINT fromC = { newX, newY };
                               // POINT arrow = arrowCoords(fromC, to);
                               // POINT vertices[] = { {0, 0}, {0, 0}, {0, 0} };
                               // float alpha = atan2(to.y - fromC.y, to.x - fromC.x);
                               // for (int i = 0; i < 3; i++) {
                               //     vertices[i].x = arrowRadius * cos(alpha) + arrow.x;
                               //     vertices[i].y = arrowRadius * sin(alpha) + arrow.y;
                               //     alpha += (1.0 / 3.0) * (2 * PI);
                               // }
                               // Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0])); // ðèñóåì ñòðåëó
                            }
                        }
                        
                        }
                    }
                }
            SelectObject(hdc, BPen);

            for (int i = 0; i < n; i++) { // îòðèñîâêà âåðøèí
                char name[3];
                // symbolsToShow = i + 1 < 10 ? 1 : 2;
                snprintf(name, 3, "%d", i + 1);
                Ellipse(hdc, verts[i].coords.x - radius, verts[i].coords.y - radius, verts[i].coords.x + radius, verts[i].coords.y + radius);
                TextOutA(hdc, verts[i].coords.x - radius + 7 + (i + 1 < 10 ? 4 : 0), verts[i].coords.y - radius / 2, name, i + 1 < 10 ? 1 : 2);
            }
          } // oriented


        

        

        //char name[3];
        //POINT to = verts[startV].coords;

        

       


      // for (int i = 1; i < n; i++) { // отрисовываем порядок прохождения вершин
      //     for (int j = 1; j < n; j++) {
      //         char name[3];
      //         int newN = 0;
      //         for (int j = 0; bfsMatrix[j][0] != -1; j++) {
      //             if (bfsMatrix[j][1] == i) {
      //                 newN = j;
      //                 break;
      //             }
      //         }
      //          POINT to = verts[i].coords;
      // 
      //         snprintf(name, 3, "%d", newN + 2);
      //         TextOutA(hdc, to.x + 10, to.y, name, 2);
      //     }
      //     
      // }

       

        TextOutA(hdc, nonStartX - 100, 100, "W Matrix:", 9);
        POINT matrixA = { nonStartX, 100 };
        showMatrix(Wt, matrixA, hdc);

       

        // TextOutA(hdc, nonStartX + 300, 100, "A^2:", 4);
        // POINT matrixA2 = { nonStartX + 400, 100 };
        // showMatrix(A2, matrixA2, hdc);
        // 
        // TextOutA(hdc, nonStartX - 90, 350, "A^3:", 4);
        // POINT matrixA3 = { nonStartX, 350 };
        // showMatrix(A3, matrixA3, hdc);


        EndPaint(hwnd, &ps);//ìàëþâàííÿ çàê³í÷åíå
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

void prims(int matrix[n][n], int curr) {
    bool selected[n];
    for (int i = 0; i < n; i++) {
        selected[i] = false;
    }
    selected[curr] = true;
    int c = 0;

    while (c < n - 1) {
        int min = INF;
        int y = 0;
        int lastI = 0;
        int lastJ = 0;
        for (int i = 0; i < n; i++) {
            if (selected[i]) {
                for (int j = 0; j < n; j++) {
                    if (!selected[j] && (matrix[i][j] != 0) && (i != j)) {
                        if (min > matrix[i][j]) {
                            min = matrix[i][j];
                            y = j;
                            lastI = i;
                            lastJ = j;
                        } 
                    }
                }
            }
        } 
        selected[y] = true;
        primMatrix[globalCounter][0] = lastI;
        primMatrix[globalCounter++][1] = lastJ;
        c++;
    }
}



