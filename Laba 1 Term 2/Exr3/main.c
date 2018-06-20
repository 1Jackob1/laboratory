#include <stdio.h>
#include <malloc.h>

double** NewMatrix(int size){
    char* buf = (char *)(malloc(size*sizeof(double*) + size*(size+1)*sizeof(double)));
    if(buf == NULL)
        return NULL;
    double** a=(double**)buf;
    double *tmp=(double*)(buf+size*sizeof(double*));
    for (int i = 0; i < size; ++i) {
        a[i] = tmp + i*(size+1);
    }

    return a;
}

void DellMatrix(double** a){
    free(a);
    a=NULL;
}

void swap(double **a, double **b){
    double **tmp = a;
    **a = **b;
    **b = **tmp;
}
int main() {
    int n;
    printf("Input matrix size: ");
    scanf("%d", &n);
    double *check = (double*)(malloc(n * sizeof(double)));//bug was here -- was sizeof(double *) -> sizeof(double).
    double **A=NewMatrix(n);
    double **B=NewMatrix(n);
    if(check==NULL || A==NULL || B==NULL){
       printf("No memory, resize matrix pls!");
        return 0;
    }



    for (int i = 0; i < n; i++){
        for (int j = 0; j <= n; ++j) {
            scanf("%lf", &A[i][j]);
            B[i][j]=A[i][j];
        }
        check[i]=0;
    }

    //strait run
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i; j < n - 1; ++j) {
            if(A[j+1][i] == 0){
                int _j=j+1;
                while(A[_j][i]==0)
                {
                    if(_j>=n-1){
                        printf("No solution!\n");
                        return 0;
                    }
                    _j++;
                }
                swap(A+j,A+_j);
            }
            double buf = -A[j+1][i] / A[i][i];
            for (int k = 0; k <= n; ++k) {
                A[j + 1][k] += buf * A[i][k];
            }
        }
    }
    //----------------------

    //back stroke
    for (int i = n-1; 0 < i ; i--) {
        for (int j = i; 0 < j ; j--) {
            double buf = -A[j-1][i]/A[i][i];
            for (int k = j; k <=n ; k++) {
                A[j-1][k] += buf * A[i][k];
            }
        }
    }
    //----------------------

    //reductions
    for (int i = 0; i < n ; i++)
        for (int j = n; 0 <= j ; j--)
            A[i][j]/=A[i][i];
    //----------------------

    //checking
    for (int i = 0; i < n ; i++) {
        for (int j = 0; j < n; j++)
            check[i] += B[i][j] * A[j][n];
        check[i]-=B[i][n];
    }
    //----------------------

    //output
    for (int i = 0; i < n; i++){
        printf("%lf ? %e", A[i][n], check[i]);
        printf("\n");
    }
    //----------------------

    //Free
    DellMatrix(A);
    DellMatrix(B);
    free(check);
    check=NULL;


/*
      3
      3 4 7 61
      3 3 2 35
      4 4 2 44

    4
    5 2 4 7 62
    5 5 4 5 73
    6 4 7 5 80
    2 4 6 4 52
*/


    return 0;
}

