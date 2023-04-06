#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

char ProgName[] = "Лабораторна робота 3";

void arrow(float fi, int px, int py, HDC hdc){
    fi = 3.1416 * (180.0 - fi) / 180.0;
    int lx, ly, rx, ry;
    lx = px + 15 * cos(fi + 0.3);
    rx = px + 15 * cos(fi - 0.3);
    ly = py + 15 * sin(fi + 0.3);
    ry = py + 15 * sin(fi - 0.3);
    MoveToEx(hdc, px, py, NULL);
    LineTo(hdc, px, py);
    LineTo(hdc, rx, ry);
    MoveToEx(hdc, px, py, NULL);
    LineTo(hdc, lx, ly);
    return 0;
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
            res[i][j] = roundf(mat[i][j] * c);
        }
    }
    return res;
}

void symbolArray(int n, char* arr){
    for(int i = 0; i < n; i++){
        arr[i] = (i+1) + '0';
    }
}

void arrayX(int N, int* array){
    float edge = N / 4.0;
    printf("%d\n", edgeCeil);
    for(int i = 0; i < edgeCeil+1; i++){
        array[i] = 100 + 100*i;
    }

    for(int i = edgeCeil; i < edgeCeil*2; i++){
        array[i] = array[i-1];
    }

    for(int i = edgeCeil*2; i < edgeCeil*3; i++){
        array[i] = array[i-1]-100;
    }

    for(int i = edgeCeil*3; i < edgeCeil*4-2; i++){
        if(i == N-1) break;
        array[i] = array[i-1];
    }
}


void drawGraph(HWND hWnd, HDC hdc)
{
    const int N = 11;//Number of our vertex
    char *nn[11] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"};
    //int nx[11] = {100, 200, 300, 400, 400, 400, 400, 300, 200, 100, 100};
    int nx[N];
    arrayX(N, nx);
    for(int i = 0; i < N; i++){
        printf("%d ", nx[i]);
    }
    printf("\n");
    int ny[11] = {100, 100, 100, 100, 200, 300, 400, 400, 400, 400, 250};
    int dx = 16, dy = 16, dtx = 5;
    printf("Size of array: %d\n", sizeof(nn)/sizeof(nn[0]));

    float** T = randm(N);
    float** A = mulmr(0.715, T, N);
    printf("T:\n");
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.2f ", T[i][j]);
        }
        printf("\n");
    }

    printf("A:\n");
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.0f ", A[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(A[i][j] == 1.0){
                if(2 <= abs(i - j) && abs(i - j) <= 3){
                    if(nx[i] == nx[j] || ny[i] == ny[j]){
                        Arc(hdc, nx[i], ny[i]-40, nx[j], ny[j]+40, nx[j], ny[j], nx[i], ny[i]);
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        arrow(-45.0,nx[j]-dx*0.5,ny[j]-dy*0.8, hdc);
                    }
                }else if(i == j){
                Ellipse(hdc, nx[i]-15-20, ny[i]+15-20, nx[i]+5, ny[i]+35);
                }
            }else{
                MoveToEx(hdc, nx[i], ny[i], NULL);
                LineTo(hdc, nx[j], ny[j]);
                arrow(acos(nx[j]/ny[j]),nx[j]-dx,ny[j], hdc);
            }
        }
    }
    /*
    MoveToEx(hdc, nx[0], ny[0], NULL);
    LineTo(hdc, nx[2], ny[2]);
    arrow(0,nx[2]-dx,ny[2], hdc);
    Arc(hdc, nx[0], ny[0]-40, nx[1], ny[1]+40, nx[1], ny[1], nx[0], ny[0]);
    arrow(-85.0,nx[1]-dx*0.5,ny[1]-dy*0.8, hdc);
    */

    HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
    HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));
    /*
    SelectObject(hdc, KPen);
    MoveToEx(hdc, nx[0], ny[0], NULL);
    LineTo(hdc, nx[1], ny[1]);
    arrow(0,nx[1]-dx,ny[1], hdc);
    Arc(hdc, nx[0], ny[0]-40, nx[2], ny[2]+40, nx[2], ny[2], nx[0], ny[0]);
    arrow(-45.0,nx[2]-dx*0.5,ny[2]-dy*0.8, hdc);
    */
    SelectObject(hdc, BPen);
    for(int i = 0;i < N; i++){
        Ellipse(hdc, nx[i]-dx,ny[i]-dy,nx[i]+dx,ny[i]+dy);
        TextOut(hdc, nx[i]-dtx,ny[i]-dy/2, nn[i],2);
    }
    for(int i = 0; i < N; i++){
        free(T[i]);
        free(A[i]);
    }
    free(T);
    free(A);
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

    hWnd = CreateWindow(ProgName, "Лабораторна робота 3. Виконав Д. М. Лесько", WS_OVERLAPPEDWINDOW, 100, 100, 600, 500, (HWND)NULL, (HMENU)NULL, (HINSTANCE)hInstance, (HINSTANCE)NULL);
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
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            drawGraph(hWnd, hdc);
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, messg, wParam, lParam);
    }
    return 0;
}
