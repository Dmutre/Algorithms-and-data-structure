#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include<conio.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Declare our buttons
HWND buttonDrawDirect, buttonDrawUnd, buttonPower;

char ProgName[] = "Лабораторна робота 4";

void arrow(float fi, int px, int py, HDC hdc){
    int lx, ly, rx, ry;
    lx = px - 15 * cos(fi + 0.3);
    rx = px - 15 * cos(fi - 0.3);
    ly = py - 15 * sin(fi + 0.3);
    ry = py - 15 * sin(fi - 0.3);
    MoveToEx(hdc, px, py, NULL);
    LineTo(hdc, lx, ly);
    MoveToEx(hdc, px, py, NULL);
    LineTo(hdc, rx, ry);
}

void drawArrow(int fromx, int fromy, int tox, int toy, int r, HDC hdc){
    float angleRad = atan2((toy-fromy), (tox-fromx));
    float px = tox - r*cos(angleRad);
    float py = toy - r*sin(angleRad);
    arrow(angleRad, px, py, hdc);
}

float** randm(int n) {
    srand(2113);
    float** arr = (float**)malloc(n * sizeof(float*));
    for(int i = 0; i < n; i++){
        arr[i] = (float*)malloc(n * sizeof(float));
        for(int j = 0; j < n; j++){
            arr[i][j] = ((float)rand() / (float)RAND_MAX) * 2.0;
        }
    }

    return arr;
}

float** mulmr(float c, float** mat, int n) {
    float** res = (float**)malloc(n * sizeof(float*));
    for(int i = 0; i < n; i++){
        res[i] = (float*)malloc(n * sizeof(float));
        for(int j = 0; j < n; j++){
            res[i][j] = floor(mat[i][j] * c);
        }
    }

    return res;
}

char** symbolArray(int N){
    char** array = malloc(N * sizeof(char*));
    for(int i = 0; i < N; i++) {
        array[i] = malloc(3 * sizeof(char)); // allocate memory for each element
        sprintf(array[i], "%d", i+1); // use sprintf to convert int to string
    }
    return array;
}

float** makeSymmetric(float** mat, int n){
    float** arr = (float**)malloc(n * sizeof(float*));
    for(int i = 0; i < n; i++){
        arr[i] = (float*)malloc(n * sizeof(float));
        for(int j = 0; j < n; j++){
            if(mat[i][j] == 1 || mat[j][i] == 1){
                arr[i][j] = 1;
            } else {
                arr[i][j] = 0;
            }
        }
    }
    return arr;
}

void printMatrix(int N, float** mat){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.0f ", mat[i][j]);
        }
        printf("\n");
    }
}

void arrayX(int N, int* array){
    float edge = N / 4.0;
    int edgeCeil = ceil(edge);
    int stepRigth = 200;

    for(int i = 0; i < edgeCeil+1; i++){
        array[i] = 100 + 100*i + stepRigth;
    }

    for(int i = edgeCeil+1; i < edgeCeil*2+1; i++){
        array[i] = array[i-1];
    }

    for(int i = edgeCeil*2+1; i < edgeCeil*3+1; i++){
        array[i] = array[i-1]-100;
    }

    for(int i = edgeCeil*3+1; i < edgeCeil*4 && i < N; i++){
        array[i] = array[0];
    }
}

void arrayY(int N, int* array){
    float edge = N / 4.0;
    int edgeCeil = ceil(edge);

    for(int i = 0; i < edgeCeil+1; i++){
        array[i] = 100;
    }

    for(int i = edgeCeil+1; i < edgeCeil*2+1; i++){
        array[i] = array[i-1]+100;
    }

    for(int i = edgeCeil*2+1; i < edgeCeil*3+1; i++){
        array[i] = array[i-1];
    }

    for(int i = edgeCeil*3+1; i < edgeCeil*4 && i < N; i++){
        array[i] = array[i-1]-100;
    }
}


void drawUnDependenceGraph(HWND hWnd, HDC hdc, int n, char** nn, int nx[], int ny[], float** A){
    int edgeCeil = ceil(n / 4.0);//Number of vertex, that we can draw four time to get squer
    int dx = 16, dy = 16, dtx = 5;
    HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));
    SelectObject(hdc, KPen);

    printf("Undirected A matrix:\n");//Output our matrix of dependencies
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(A[j][i] == 1 || A[i][j] == 1){
                printf("1 ");
            } else printf("%.0f ", A[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < n; i++){//For ellipses
        for(int j = 0; j < n; j++){
            if(A[i][j] == 1){
                if(i == j){
                    int dir = (int) ceil((i+1)/(float) edgeCeil);
                    if(dir%2 == 0){
                        if(dir > edgeCeil){
                            Ellipse(hdc, nx[i]-40, ny[i]-20, nx[i], ny[i]+20);
                        } else{
                            Ellipse(hdc, nx[i]+40, ny[i]-20, nx[i], ny[i]+20);
                        }
                    } else{
                        if(dir >= edgeCeil){
                            Ellipse(hdc, nx[i]-20, ny[i]+40, nx[i]+20, ny[i]);
                        } else{
                            Ellipse(hdc, nx[i]-20, ny[i]-40, nx[i]+20, ny[i]);
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < n; i++){//For lines when circles are on the same row in X or Y
        for(int j = 0; j < n; j++){
            if(A[i][j] == 1 && abs(i-j) >=2 && abs(i-j) <= edgeCeil && (nx[i] == nx[j] || ny[i] == ny[j])){
                int dir = (int) ceil((i+1)/(float) edgeCeil);
                if(nx[i] == nx[j]){
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, nx[j]+35, ny[i]-(ny[i]-ny[j])/2);
                    MoveToEx(hdc, nx[j]+35, ny[i]-(ny[i]-ny[j])/2, NULL);
                    LineTo(hdc, nx[j], ny[j]);
                } else{
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-35);
                    MoveToEx(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-35, NULL);
                    LineTo(hdc, nx[j], ny[j]);
                }
            }
        }
    }

    for(int i = 0; i < n; i++){//For lines between vertex
        for(int j = 0; j < n; j++){
            if(A[i][j] == 1){
                if(abs(i-j) >=2 && abs(i-j) <= edgeCeil && (nx[i] == nx[j] || ny[i] == ny[j])){
                } else if(i == j){
                } else{
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, nx[j], ny[j]);
                }
            }
        }
    }

    HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));

    SelectObject(hdc, BPen);
    for(int i = 0;i < n; i++){
        Ellipse(hdc, nx[i]-dx,ny[i]-dy,nx[i]+dx,ny[i]+dy);
        TextOut(hdc, nx[i]-dtx,ny[i]-dy/2, nn[i],2);
    }
}

void drawGraph(HWND hWnd, HDC hdc, int N, int nx[], int ny[], char** nn, float** A){
    int edgeCeil = ceil(N / 4.0);//Number of vertex, that we can draw four time to get squer
    int dx = 16, dy = 16, dtx = 5;

    printf("%\nX coordinates: ");
    for(int i = 0; i < N; i++){//Output coordinates of vertexes for X
        printf("%d ", nx[i]);
    }
    printf("%\nY coordinates");
    for(int i = 0; i < N; i++){//Output coordinates of vertexes for Y
        printf("%d ", ny[i]);
    }
    printf("\n");

    printf("A:\n");//Output our matrix of dependencies
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.0f ", A[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < N; i++){//For ellipses
        for(int j = 0; j < N; j++){
            if(A[i][j] == 1){
                if(i == j){
                    int dir = (int) ceil((i+1)/(float) edgeCeil);
                    if(dir%2 == 0){
                        if(dir > edgeCeil){
                            Ellipse(hdc, nx[i]-40, ny[i]-20, nx[i], ny[i]+20);
                            drawArrow(nx[i]-35, ny[i]+50, nx[j], ny[j], dx, hdc);
                        } else{
                            Ellipse(hdc, nx[i]+40, ny[i]-20, nx[i], ny[i]+20);
                            drawArrow(nx[i]+35, ny[i]-50, nx[j], ny[j], dx, hdc);
                        }
                    } else{
                        if(dir >= edgeCeil){
                            Ellipse(hdc, nx[i]-20, ny[i]+40, nx[i]+20, ny[i]);
                            drawArrow(nx[i]+70, ny[i]+45, nx[j], ny[j], dx, hdc);
                        } else{
                            Ellipse(hdc, nx[i]-20, ny[i]-40, nx[i]+20, ny[i]);
                            drawArrow(nx[i]-70, ny[i]-45, nx[j], ny[j], dx, hdc);
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < N; i++){//For lines when circles are on the same row in X or Y
        for(int j = 0; j < N; j++){
            if(A[i][j] == 1 && abs(i-j) >=2 && abs(i-j) <= edgeCeil && (nx[i] == nx[j] || ny[i] == ny[j])){
                if(nx[i] == nx[j]){
                    if(i > j){
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, nx[j]+35, ny[i]-(ny[i]-ny[j])/2);
                        MoveToEx(hdc, nx[j]+35, ny[i]-(ny[i]-ny[j])/2, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        drawArrow(nx[j]+35, ny[i]-(ny[i]-ny[j])/2, nx[j], ny[j], dx, hdc);
                    } else{
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, nx[j]-35, ny[i]-(ny[i]-ny[j])/2);
                        MoveToEx(hdc, nx[j]-35, ny[i]-(ny[i]-ny[j])/2, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        drawArrow(nx[j]-35, ny[i]-(ny[i]-ny[j])/2, nx[j], ny[j], dx, hdc);
                    }
                } else{
                    if(i > j){
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]+35);
                        MoveToEx(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]+35, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        drawArrow(nx[j]+(nx[i]-nx[j])/2, ny[i]+35, nx[j], ny[j], dx, hdc);

                    } else{
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-35);
                        MoveToEx(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-35, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        drawArrow(nx[j]+(nx[i]-nx[j])/2, ny[i]-35, nx[j], ny[j], dx, hdc);
                    }
                }
            }
        }
    }

    int atall = 0;
    for(int i = 0; i < N; i++){//For lines between vertex
        for(int j = 0; j < N; j++){
            if(A[i][j] == 1){
                if(abs(i-j) >=2 && abs(i-j) <= edgeCeil && (nx[i] == nx[j] || ny[i] == ny[j])){
                    atall++;
                } else if(i == j){
                    atall++;
                } else{
                    atall++;
                    if(i > j && A[j][i] == 1){
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, (nx[i]+nx[j])/2+20, (ny[i]+ny[j])/2);
                        MoveToEx(hdc, (nx[i]+nx[j])/2+20, (ny[i]+ny[j])/2, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        drawArrow((nx[i]+nx[j])/2+20, (ny[i]+ny[j])/2, nx[j], ny[j], dx, hdc);

                    } else{
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        drawArrow(nx[i], ny[i], nx[j], ny[j], dx, hdc);
                    }
                }
            }
        }
    }

    printf("Number of all dependences: %d\n", atall);

    HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
    HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));

    SelectObject(hdc, BPen);
    for(int i = 0;i < N; i++){
        Ellipse(hdc, nx[i]-dx,ny[i]-dy,nx[i]+dx,ny[i]+dy);
        TextOut(hdc, nx[i]-dtx,ny[i]-dy/2, nn[i],2);
    }
}

int* powerOfUndirGraph(int N, float** mat){
    int* undirPower = malloc(N * sizeof(int));
    int count;

    for(int i = 0; i < N; i++){
        count = 0;
        for(int j = 0; j < N; j++){
            if(mat[i][j] == 1.0) count++;
        }
        undirPower[i] = count;
    }

    return undirPower;
}

int* outgoingDegrees(int N, float** mat) {
    int* outgoingDegrees = (int*) malloc(N * sizeof(int));
    int count = 0;

    for (int i = 0; i < N; i++) {
            count = 0;
        for (int j = 0; j < N; j++) {
            if (mat[i][j] == 1) count++;
        }
        outgoingDegrees[i] = count;
    }

    return outgoingDegrees;
}

int* incomingDegrees(int N, float** mat) {
    int* incomingDegrees = (int*) malloc(N * sizeof(int));

    for (int i = 0; i < N; i++) {
        incomingDegrees[i] = 0;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (mat[i][j] == 1) {
                incomingDegrees[j]++;
            }
        }
    }

    return incomingDegrees;
}

void mainFunc(int option, HWND hWnd, HDC hdc){
    const int N = 11;//Number of our vertex
    int nx[N], ny[N];
    arrayX(N, nx);
    arrayY(N, ny);

    char** nn = symbolArray(N);
    float** T = randm(N);
    float** A = mulmr(0.66, T, N);//Fill our matrix
    float** symA = makeSymmetric(A, N);
    int* undirPower = powerOfUndirGraph(N, symA);
    int* outgoingDeg = outgoingDegrees(N, A);
    int* incomingDeg = incomingDegrees(N, A);

    switch(option){
        case 1:
            drawGraph(hWnd, hdc, N, nx, ny, nn, A);
            break;
        case 2:
            drawUnDependenceGraph(hWnd, hdc, N, nn, nx, ny, A);
            break;
        case 3:

            break;
    }

    for(int i = 0; i < N; i++){
        free(T[i]);
        free(A[i]);
    }
    free(T);//To avoid problems with dynamic memory we free out matrix in the end of our programme
    free(A);
    free(nn);
    free(undirPower);
    free(outgoingDeg);
    free(incomingDeg);
}

void windowUpdate(HWND hWnd, HDC hdc, PAINTSTRUCT ps, int option){
    InvalidateRect(hWnd, NULL, TRUE);
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); // replace RGB(255, 255, 255) with desired background color
    UpdateWindow(hWnd);//Update our app window to make possible to draw new graph
    system("cls");//clear console
    hdc = BeginPaint(hWnd, &ps);
    FillRect(hdc, &ps.rcPaint, hBrush);
    mainFunc(option, hWnd, hdc);//Call main function that call needed functions
    EndPaint(hWnd, &ps);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow){
    WNDCLASS w;

    w.lpszClassName = ProgName;
    w.hInstance = hInstance;
    w.lpfnWndProc = WndProc;
    w.hCursor = LoadCursor(NULL, IDC_ARROW);
    w.hIcon = 0;
    w.lpszMenuName = 0;
    w.hbrBackground = WHITE_BRUSH;
    w.style = CS_HREDRAW|CS_VREDRAW;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;

    if(!RegisterClass(&w)) return 0;

    HWND hWnd;
    MSG lpMsg;

    hWnd = CreateWindow(ProgName, "Лабораторна робота 3. Виконав Д. М. Лесько", WS_OVERLAPPEDWINDOW, 100, 100, 1200, 700, (HWND)NULL, (HMENU)NULL, (HINSTANCE)hInstance, (HINSTANCE)NULL);
    ShowWindow(hWnd, nCmdShow);
    while(GetMessage(&lpMsg, hWnd, 0, 0)){
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }

    return(lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam){
    HDC hdc;
    PAINTSTRUCT ps;
    switch(messg){
        case WM_CREATE:

            buttonDrawDirect = CreateWindow("BUTTON",
                                  "Draw directed graph",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  20, 20, 150, 30,
                                  hWnd, (HMENU) 1, NULL, NULL);
            buttonDrawUnd = CreateWindow("BUTTON",
                                  "Draw undirected graph",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  20, 50, 150, 30,
                                  hWnd, (HMENU) 2, NULL, NULL);
            buttonPower = CreateWindow("BUTTON",
                                  "Power of graph",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  20, 80, 150, 30,
                                  hWnd, (HMENU) 3, NULL, NULL);
            break;
        case WM_COMMAND:

            switch(LOWORD(wParam)){
                case 1:
                    windowUpdate(hWnd, hdc, ps, 1);
                    break;
                case 2:
                    windowUpdate(hWnd, hdc, ps, 2);
                    break;
                case 3:
                    windowUpdate(hWnd, hdc, ps, 3);
                    break;
            }
            break;
        case WM_PAINT:
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, messg, wParam, lParam);
    }
    return 0;
}
