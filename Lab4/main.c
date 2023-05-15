#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include<conio.h>
#include <stdbool.h>
#define RADIUS 35


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Declare our buttons
HWND buttonDrawDirect, buttonDrawUnd, buttonPower, buttonRegCheck, buttonFindIsolated, buttonDraw2, buttonWays, buttonReachable, buttonStronglyConnectedMat,
buttonComponent, buttonCondensat;

char ProgName[] = "Лабораторна робота 4";


void freeMatrix(float** matrix, int N){
    if (matrix != NULL) {
        for (int i = 0; i < N; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
}

void freeMatrixInt(int** matrix, int N){
    if (matrix != NULL) {
        for (int i = 0; i < N; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
}

void arrow(float fi, int px, int py, HDC hdc){//draw arrow
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

void drawArrow(int fromx, int fromy, int tox, int toy, int r, HDC hdc){//count angle and call function for drawing arrow
    float angleRad = atan2((toy-fromy), (tox-fromx));
    float px = tox - r*cos(angleRad);
    float py = toy - r*sin(angleRad);
    arrow(angleRad, px, py, hdc);
}

float** randm(int N) {//generate random matrix with value from 0 to 2.0
    srand(2113);
    float** arr = (float**)malloc(N * sizeof(float*));
    for(int i = 0; i < N; i++){
        arr[i] = (float*)malloc(N * sizeof(float));
        for(int j = 0; j < N; j++){
            arr[i][j] = ((float)rand() / (float)RAND_MAX) * 2.0;
        }
    }

    return arr;
}

float** mulmr(float c, float** mat, int n) {//round matrix from randm() to 0 1 matrix
    float** res = (float**)malloc(n * sizeof(float*));
    for(int i = 0; i < n; i++){
        res[i] = (float*)malloc(n * sizeof(float));
        for(int j = 0; j < n; j++){
            res[i][j] = floor(mat[i][j] * c);
        }
    }

    return res;
}

char** symbolArray(int N){//return symbol char array of pointer with elements from 1 to N
    char** array = malloc(N * sizeof(char*));
    for(int i = 0; i < N; i++) {
        array[i] = malloc(3 * sizeof(char)); // allocate memory for each element
        sprintf(array[i], "%d", i+1); // use sprintf to convert int to string
    }
    return array;
}

float** makeSymmetric(float** mat, int N){//receive 0 1 matrix A and make it symmetric for 1 (undirected graph)
    float** arr = (float**)malloc(N * sizeof(float*));
    for(int i = 0; i < N; i++){
        arr[i] = (float*)malloc(N * sizeof(float));
        for(int j = 0; j < N; j++){
            if(mat[i][j] == 1 || mat[j][i] == 1){
                arr[i][j] = 1;
            } else {
                arr[i][j] = 0;
            }
        }
    }
    return arr;
}

void printMatrix(int N, float** mat){//Print received matrix
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.0f ", mat[i][j]);
        }
        printf("\n");
    }
}

void printMatrixInt(int N, int** mat){//Print received matrix
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

void printIntArray(int N, int* array){//Print received int array
    for(int i = 0; i < N; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

int checkForRegularity(int N, int* array){//return 1(true) or 0(false) value about received array of vertex power. If graph is regular- return 1
    for(int i = 0; i < N; i++){
        if(array[0] == array[i]) continue;
        else return 0;
    }
    return 1;
}

//depend on received flag from checkForRegularity() printf information about regularity of graph in console. Also printf power of graph
void regularityPrint(int flag, int* array){
    if(flag == 0){
        printf("NO\n");
    } else if(flag == 1){
        printf("YES\nPower of regularity is: %d\n", array[1]);
    }
}

void arrayX(int N, int* array){//count X coordinates for graph
    float edge = N / 4.0;
    int edgeCeil = ceil(edge);
    int stepRigth = 400;//step to go away from left border of window, to let space for buttons

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

void arrayY(int N, int* array){//count Y coordinates for graph
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

//from received matrix of dependencies, coordinates, names and number of vertex draw undirected graph
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
            if(A[i][j] == 1 && ((abs(i-j) >=2 && abs(i-j) <= edgeCeil) || abs(i-j) >= 3*edgeCeil) && (nx[i] == nx[j] || ny[i] == ny[j])){
                if(nx[i] == nx[j]){
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, nx[j]+RADIUS, ny[i]-(ny[i]-ny[j])/2);
                    MoveToEx(hdc, nx[j]+RADIUS, ny[i]-(ny[i]-ny[j])/2, NULL);
                    LineTo(hdc, nx[j], ny[j]);
                } else{
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-RADIUS);
                    MoveToEx(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-RADIUS, NULL);
                    LineTo(hdc, nx[j], ny[j]);
                }
            }
        }
    }

    for(int i = 0; i < n; i++){//For lines between vertex
        for(int j = 0; j < n; j++){
            if(A[i][j] == 1){
                if(!(((abs(i-j) >=2 && abs(i-j) <= edgeCeil) || abs(i-j) >= 3*edgeCeil) && (nx[i] == nx[j] || ny[i] == ny[j])) && i != j){
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

//from received matrix of dependencies, coordinates, names and number of vertex draw directed graph
void drawGraph(HWND hWnd, HDC hdc, int N, int nx[], int ny[], char** nn, float** A){
    int edgeCeil = ceil(N / 4.0);//Number of vertex, that we can draw four time to get squer
    int dx = 16, dy = 16, dtx = 5;

    printf("Adjacency matrix:\n");//Output our matrix of dependencies
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
                            drawArrow(nx[i]-RADIUS, ny[i]+50, nx[j], ny[j], dx, hdc);
                        } else{
                            Ellipse(hdc, nx[i]+40, ny[i]-20, nx[i], ny[i]+20);
                            drawArrow(nx[i]+RADIUS, ny[i]-50, nx[j], ny[j], dx, hdc);
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
            if(A[i][j] == 1 && ((abs(i-j) >=2 && abs(i-j) <= edgeCeil) || abs(i-j) >= 3*edgeCeil) && (nx[i] == nx[j] || ny[i] == ny[j])){
                if(nx[i] == nx[j]){
                    if(i > j){
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, nx[j]+RADIUS, ny[i]-(ny[i]-ny[j])/2);
                        MoveToEx(hdc, nx[j]+RADIUS, ny[i]-(ny[i]-ny[j])/2, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        drawArrow(nx[j]+RADIUS, ny[i]-(ny[i]-ny[j])/2, nx[j], ny[j], dx, hdc);
                    } else{
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, nx[j]-RADIUS, ny[i]-(ny[i]-ny[j])/2);
                        MoveToEx(hdc, nx[j]-RADIUS, ny[i]-(ny[i]-ny[j])/2, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        drawArrow(nx[j]-RADIUS, ny[i]-(ny[i]-ny[j])/2, nx[j], ny[j], dx, hdc);
                    }
                } else{
                    if(i > j){
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]+RADIUS);
                        MoveToEx(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]+RADIUS, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        drawArrow(nx[j]+(nx[i]-nx[j])/2, ny[i]+RADIUS, nx[j], ny[j], dx, hdc);

                    } else{
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-RADIUS);
                        MoveToEx(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-RADIUS, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        drawArrow(nx[j]+(nx[i]-nx[j])/2, ny[i]-RADIUS, nx[j], ny[j], dx, hdc);
                    }
                }
            }
        }
    }

    int atall = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(A[i][j] == 1){
                atall++;
                if(i == j){
                } else if(((abs(i-j) >=2 && abs(i-j) <= edgeCeil) || abs(i-j) >= 3*edgeCeil) && (nx[i] == nx[j] || ny[i] == ny[j])){
                } else{
                    if(i > j && A[j][i] == 1){
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, (nx[i]+nx[j])/2, (ny[i]+ny[j])/2+20);
                        MoveToEx(hdc, (nx[i]+nx[j])/2, (ny[i]+ny[j])/2+20, NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        drawArrow((nx[i]+nx[j])/2, (ny[i]+ny[j])/2+20, nx[j], ny[j], dx, hdc);
                    } else if(ny[i] == ny[j]){
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

//function to circle isolated and pendant vertexes by red and yellow color accordingly
void drawIsolatedPendantVertexes(HWND hWnd, HDC hdc, int N, int nx[], int ny[], char** nn, int* isPen){
    HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HPEN yellowPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
    int dx = 16, dy = 16, dtx = 5;

    for(int i = 0; i < N; i++){
        if(isPen[i] == 0){
            SelectObject(hdc, redPen);
            Ellipse(hdc, nx[i]-dx,ny[i]-dy,nx[i]+dx,ny[i]+dy);
            TextOut(hdc, nx[i]-dtx,ny[i]-dy/2, nn[i],2);
        } else if(isPen[i] == 1){
            SelectObject(hdc, yellowPen);
            Ellipse(hdc, nx[i]-dx,ny[i]-dy,nx[i]+dx,ny[i]+dy);
            TextOut(hdc, nx[i]-dtx,ny[i]-dy/2, nn[i],2);
        }
    }
}

int* powerOfUndirGraph(int N, float** mat){//return array of powers of undirected graph vertexes
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

int* outgoingDegrees(int N, float** mat) {//return array of half power of outgoing to vertex dependencies
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

int* incomingDegrees(int N, float** mat) {//return array of half power of incoming to vertex dependencies
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

int* UndirIsolatedPendant(int N, int* array){//Function, that receive array of graph`s vertexes power and return array with mark which vertex is it
    int* UndirIsPen = (int*) malloc(N * sizeof(int));

    //if vertex is isolated, their number in array would be 0, if has only one dependencies (leaf vertex)- 1, more- 2
    for(int i = 0; i < N; i++){
        if(array[i] == 0){
            UndirIsPen[i] = 0;
        } else if(array[i] == 1){
            UndirIsPen[i] = 1;
        } else UndirIsPen[i] = 2;
    }

    return UndirIsPen;
}

void printIsolatedPendant(int N, int* array){//Function that output numbers of isolated and pendant vertexes

    printf("\nIsolated vertexes:\n");
    for(int i = 0; i < N; i++){
        if(array[i] == 0){
            printf("%d  ", (i+1));
        }
    }
    printf("\n\nPendant vertexes:\n");
    for(int i = 0; i < N; i++){
        if(array[i] == 1){
            printf("%d  ", (i+1));
        }
    }
}

float** multiplyMatrices(float** mat1, float** mat2, int N) {
    float** result = (float**)malloc(N * sizeof(float*));

    for (int i = 0; i < N; i++) {
        result[i] = (float*)malloc(N * sizeof(float));
        for (int j = 0; j < N; j++) {
            result[i][j] = 0;
            for (int k = 0; k < N; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    return result;
}

float** powerMatrix(float** matrix, int N, int power) {
    float** result = (float**)malloc(N * sizeof(float*));

    for (int i = 0; i < N; i++) {
        result[i] = (float*)malloc(N * sizeof(float));
        for (int j = 0; j < N; j++) {
            result[i][j] = matrix[i][j];
        }
    }

    for (int i = 1; i < power; i++) {
        float** temp = multiplyMatrices(result, matrix, N);
        for (int j = 0; j < N; j++) {
            free(result[j]);
        }
        free(result);
        result = temp;
    }

    return result;
}

void makeBinaryMatrix(float** mat, int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(mat[i][j] >= 1) mat[i][j] = 1;
        }
    }
}

float** transposeMatrix(float** matrix, int rows, int columns) {
    float** result = (float**)malloc(columns * sizeof(float*));
    for (int i = 0; i < columns; i++) {
        result[i] = (float*)malloc(rows * sizeof(float));
        for (int j = 0; j < rows; j++) {
            result[i][j] = matrix[j][i];
        }
    }
    return result;
}

float** multiplyMatricesStraight(float** mat1, float** mat2, int N) {
    float** result = (float**)malloc(N * sizeof(float*));

    for (int i = 0; i < N; i++) {
        result[i] = (float*)malloc(N * sizeof(float));
        for (int j = 0; j < N; j++) {
            result[i][j] = mat1[i][j] * mat2[i][j];
        }
    }

    return result;
}

void makeBinaryMatrixInt(int** mat, int N){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(mat[i][j] >= 1) mat[i][j] = 1;
        }
    }
}

void findPathsOfLengthTwo(float** adjacencyMatrix, int N) {

    printf("Paths of length two:\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (adjacencyMatrix[i][j] == 1) {
                for (int k = 0; k < N; k++) {
                    if (adjacencyMatrix[j][k] == 1) {
                        printf("%d -> %d -> %d\n", (i+1), (j+1), (k+1));
                    }
                }
            }
        }
    }
}

void findPathsOfLengthThree(float** adjacencyMatrix, int N) {

    printf("Paths of length three:\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (adjacencyMatrix[i][j] == 1 && adjacencyMatrix[j][k] == 1) {
                    for (int m = 0; m < N; m++) {
                        if (adjacencyMatrix[k][m] == 1) {
                            printf("%d -> %d -> %d -> %d\n", (i+1), (j+1), (k+1), (m+1));
                        }
                    }
                }
            }
        }
    }
}

float** findReachabilityMatrix(float** adjacencyMatrix, int N) {
    float** reachabilityMatrix = (float**)malloc(N * sizeof(float*));

    for (int i = 0; i < N; i++) {
        reachabilityMatrix[i] = (float*)malloc(N * sizeof(float));
        for (int j = 0; j < N; j++) {
            if (adjacencyMatrix[i][j] > 0 || i == j)
                reachabilityMatrix[i][j] = 1;
            else
                reachabilityMatrix[i][j] = 0;
        }
    }

    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                reachabilityMatrix[i][j] = reachabilityMatrix[i][j] || (reachabilityMatrix[i][k] && reachabilityMatrix[k][j]);
            }
        }
    }

    return reachabilityMatrix;
}

int** findStronglyConnectedMatrix(float** adjacencyMatrix, int N) {
    float** trans = transposeMatrix(adjacencyMatrix, N, N);
    float** result = multiplyMatricesStraight(adjacencyMatrix, trans, N);

    freeMatrix(trans, N);

    return result;
}

void dfs(int v, float** adjacencyMatrix, int* visited, int N) {
    visited[v] = 1; // Позначаємо вершину як відвідану
    printf("%d ", v + 1); // Виводимо номер вершини

    for (int i = 0; i < N; i++) {
        if (adjacencyMatrix[v][i] != 0 && !visited[i]) {
            dfs(i, adjacencyMatrix, visited, N); // Рекурсивний виклик для сусідньої вершини
        }
    }
}

void printConnectedComponents(float** adjacencyMatrix, int N) {
    int* visited = (int*)calloc(N, sizeof(int)); // Масив відвіданих вершин

    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            printf("Component: ");
            dfs(i, adjacencyMatrix, visited, N); // Запускаємо DFS для невідвіданої вершини
            printf("\n");
        }
    }

    free(visited);
}

void dfs2(float** adjacencyMatrix, int N, int vertex, int* visited, int* componentLabels, int* componentIndices, int component) {
    visited[vertex] = 1;
    componentLabels[vertex] = component;

    for (int i = 0; i < N; i++) {
        if (adjacencyMatrix[vertex][i] != 0 && !visited[i]) {
            dfs2(adjacencyMatrix, N, i, visited, componentLabels, componentIndices, component);
        }
    }
}

float** getCondensedGraph(float** adjacencyMatrix, int N, int* componentCount) {
    int* visited = (int*)calloc(N, sizeof(int));
    int* componentLabels = (int*)malloc(N * sizeof(int));
    int* componentIndices = (int*)calloc(N, sizeof(int));
    int component = 0;

    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            component++;
            dfs2(adjacencyMatrix, N, i, visited, componentLabels, componentIndices, component);
        }
    }

    int numComponents = component;
    float** condensedGraph = (float**)malloc(numComponents * sizeof(float*));
    for (int i = 0; i < numComponents; i++) {
        condensedGraph[i] = (float*)calloc(numComponents, sizeof(float));
    }

    for (int i = 0; i < N; i++) {
        int componentLabel = componentLabels[i];
        for (int j = 0; j < N; j++) {
            if (adjacencyMatrix[i][j] != 0 && componentLabel != componentLabels[j]) {
                condensedGraph[componentLabel - 1][componentLabels[j] - 1] = 1.0;
            }
        }
    }

    free(visited);
    free(componentLabels);
    free(componentIndices);

    *componentCount = numComponents;
    return condensedGraph;
}


//main function from which we call all needed function onclick of buttons. Also this function response all of calculations and let hem in argument of functions
void mainFunc(int option, HWND hWnd, HDC hdc){
    const int N = 11;//Number of our vertex
    int nx[N], ny[N];
    int flag;
    int components;

    arrayX(N, nx);
    arrayY(N, ny);

    char** nn = symbolArray(N);
    float** T = randm(N);
    float** A = mulmr(0.66, T, N);//Fill our matrix
    float** A2 = mulmr(0.71, T, N);
    float** A2Power2 = powerMatrix(A2, N, 2);
    float** A2Power3 = powerMatrix(A2, N, 3);
    float** symA = makeSymmetric(A, N);
    float** symA2 = makeSymmetric(A2, N);
    float** reachabilityMatrix = findReachabilityMatrix(A2, N);
    float** strongConMat = findStronglyConnectedMatrix(reachabilityMatrix, N);
    float** condensate = getCondensedGraph(A2, N, &components);
    int* undirPower = powerOfUndirGraph(N, symA);
    int* outgoingDeg = outgoingDegrees(N, A);
    int* incomingDeg = incomingDegrees(N, A);
    int* IsolatedPendant = UndirIsolatedPendant(N, undirPower);

    switch(option){
        case 1:
            drawGraph(hWnd, hdc, N, nx, ny, nn, A);
            break;
        case 2:
            drawUnDependenceGraph(hWnd, hdc, N, nn, nx, ny, A);
            break;
        case 3:
            printf("Directed graph: \n");
            printMatrix(N, A);
            printf("Half degree of incoming according to above matrix:\n");
            printIntArray(N, incomingDeg);
            printf("Half degree of outcoming according to above matrix:\n");
            printIntArray(N, outgoingDeg);
            printf("\n");
            printf("Undirected graph:\n");
            printMatrix(N, symA);
            printf("Power of vertexes in undirected graph according to above matrix:\n");
            printIntArray(N, undirPower);
            break;
        case 4:
            printf("Is undirected graph is regular: ");
            flag = checkForRegularity(N, undirPower);
            regularityPrint(flag, undirPower);
            printf("Is directed graph is regular for incoming: ");
            flag = checkForRegularity(N, incomingDeg);
            regularityPrint(flag, incomingDeg);
            printf("Is directed graph is regular for outcoming: ");
            flag = checkForRegularity(N, outgoingDeg);
            regularityPrint(flag, outgoingDeg);
            break;
        case 5:
            drawUnDependenceGraph(hWnd, hdc, N, nn, nx, ny, A);
            drawIsolatedPendantVertexes(hWnd, hdc, N, nx, ny, nn, IsolatedPendant);
            printIsolatedPendant(N, IsolatedPendant);
            break;
        case 6:
            drawGraph(hWnd, hdc, N, nx, ny, nn, A2);
            free(undirPower);
            free(outgoingDeg);
            free(incomingDeg);
            undirPower = powerOfUndirGraph(N, symA2);
            outgoingDeg = outgoingDegrees(N, A2);
            incomingDeg = incomingDegrees(N, A2);
            printf("Half degree of incoming according to above matrix:\n");
            printIntArray(N, incomingDeg);
            printf("Half degree of outcoming according to above matrix:\n");
            printIntArray(N, outgoingDeg);
            printf("\n");
            printf("Undirected graph:\n");
            printMatrix(N, symA2);
            printf("Power of vertexes in undirected graph according to above matrix:\n");
            printIntArray(N, undirPower);
            break;
        case 7:
            printf("(A2)^2:\n");
            printMatrix(N, A2Power2);
            printf("(A2)^3:\n");
            printMatrix(N, A2Power3);
            drawGraph(hWnd, hdc, N, nx, ny, nn, A2);
            findPathsOfLengthTwo(A2, N);
            findPathsOfLengthThree(A2, N);
            break;
        case 8:
            printf("Reachability matrix:\n");
            printMatrix(N, reachabilityMatrix);
            drawGraph(hWnd, hdc, N, nx, ny, nn, A2);
            break;
        case 9:
            printf("Strongly connected matrix:\n");
            printMatrix(N, strongConMat);
            drawGraph(hWnd, hdc, N, nx, ny, nn, A2);
            break;
        case 10:
            drawGraph(hWnd, hdc, N, nx, ny, nn, A2);
            printf("\n");
            printConnectedComponents(A2, N);
            break;
        case 11:
            printConnectedComponents(A2, N);
            drawGraph(hWnd, hdc, components, nx, ny, nn, condensate);
            break;
        default:
            break;
    }

    free(nn);
    free(outgoingDeg);
    free(incomingDeg);
    free(undirPower);
    free(IsolatedPendant);
    freeMatrix(T, N);
    freeMatrix(A, N);
    freeMatrix(A2, N);
    freeMatrix(A2Power2, N);
    freeMatrix(A2Power3, N);
    freeMatrix(symA, N);
    freeMatrix(symA2, N);
    freeMatrix(reachabilityMatrix, N);
    freeMatrix(strongConMat, N);
    freeMatrix(condensate, components);
}

//function that refresh console and our app window from previous action. Also after cleaning call mainFunc() with options
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
    int b;
    while((b = GetMessage(&lpMsg, hWnd, 0, 0))!= 0) {
        if(b == -1)	{
            return lpMsg.wParam;
        }
        else {
            TranslateMessage(&lpMsg);
            DispatchMessage(&lpMsg);
        }
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
                                  "Power graph",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  20, 80, 150, 30,
                                  hWnd, (HMENU) 3, NULL, NULL);
            buttonRegCheck = CreateWindow("BUTTON",
                                  "Check for regular graph",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  20, 110, 180, 30,
                                  hWnd, (HMENU) 4, NULL, NULL);
            buttonFindIsolated = CreateWindow("BUTTON",
                                  "Show isolated and pendant",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  20, 140, 200, 30,
                                  hWnd, (HMENU) 5, NULL, NULL);
            buttonDraw2 = CreateWindow("BUTTON",
                                  "Draw A2 and show power",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  20, 170, 200, 30,
                                  hWnd, (HMENU) 6, NULL, NULL);
            buttonWays = CreateWindow("BUTTON",
                                  "Show all 2 and 3 steps ways",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  20, 200, 200, 30,
                                  hWnd, (HMENU) 7, NULL, NULL);
            buttonReachable = CreateWindow("BUTTON",
                                  "A2 matrix of reachability",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  20, 230, 200, 30,
                                  hWnd, (HMENU) 8, NULL, NULL);
            buttonStronglyConnectedMat = CreateWindow("BUTTON",
                                  "Strongly connected matrix",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  20, 260, 200, 30,
                                  hWnd, (HMENU) 9, NULL, NULL);
            buttonComponent = CreateWindow("BUTTON",
                                  "Components of str. connection",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  20, 290, 200, 30,
                                  hWnd, (HMENU) 10, NULL, NULL);
            buttonCondensat = CreateWindow("BUTTON",
                                  "Graph of Condensate",
                                  WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  20, 320, 200, 30,
                                  hWnd, (HMENU) 11, NULL, NULL);
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
                case 4:
                    windowUpdate(hWnd, hdc, ps, 4);
                    break;
                case 5:
                    windowUpdate(hWnd, hdc, ps, 5);
                    break;
                case 6:
                    windowUpdate(hWnd, hdc, ps, 6);
                    break;
                case 7:
                    windowUpdate(hWnd, hdc, ps, 7);
                    break;
                case 8:
                    windowUpdate(hWnd, hdc, ps, 8);
                    break;
                case 9:
                    windowUpdate(hWnd, hdc, ps, 9);
                    break;
                case 10:
                    windowUpdate(hWnd, hdc, ps, 10);
                    break;
                case 11:
                    windowUpdate(hWnd, hdc, ps, 11);
                    break;
            }
            break;
        case WM_PAINT:
            UpdateWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, messg, wParam, lParam);
    }

    return 0;
}
