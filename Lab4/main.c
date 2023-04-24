#include <stdio.h>
#include <stdlib.h>


float** randm(int n) {
    srand(2113);
    float** arr = (float**)malloc(n * sizeof(float*));
    for(int i = 0; i < n; i++){
        arr[i] = (float*)malloc(n * sizeof(float));
        for(int j = 0; j < n; j++){
            arr[i][j] = ((float)rand() / (float)RAND_MAX) * 2.0;
        }
    }

    printf("T:\n");//Output our start random matrix with value from 0 to 2.0
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%.2f ", arr[i][j]);
        }
        printf("\n");
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

    printf("A:\n");//Output our matrix of dependencies
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%.0f ", res[i][j]);
        }
        printf("\n");
    }

    return res;
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

    printf("symmetric A:\n");//Output our start random matrix with value from 0 to 2.0
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%.0f ", arr[i][j]);
        }
        printf("\n");
    }

    return arr;
}

int main()
{
    const int N = 11;
    float** T = randm(N);
    float** A = mulmr(0.66, T, N);
    float** symA = makeSymmetric(A, N);


    for(int i = 0; i < N; i++){
        free(T[i]);
        free(A[i]);
        free(symA[i]);
    }
    free(T);//To avoid problems with dynamic memory we free out matrix in the end of our programme
    free(A);
    free(symA);
    return 0;
}

