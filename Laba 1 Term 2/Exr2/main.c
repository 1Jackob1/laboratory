#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double CalcFanc(double x){
    return x*x;
}

double trapezeSquareMethod(double (*p)(double x), double* a, double* b,long long int n){
    double h = (*b-*a) / n / 2;
    double res=0;
    for (int i = 1; i <= n ; ++i) {
        res+=h*(p(*a+(i-1)*h)+p(*a+i*h));
    }
    return res;
}

int main() {
    double a;
    double b;
    double e = 0.000001;
    double tcurr, tprev;
    long long int n = 100;
    long long int k = 10;
    int m = 31;
    scanf("%lf%lf",&a,&b);
    tcurr = trapezeSquareMethod(CalcFanc,&a,&b, pow(2,k) * n);
    tprev = trapezeSquareMethod(CalcFanc,&a,&b, pow(2,k-1) * n);
    while(k<=m  && ((tcurr-tprev) - e > 0)){
        tprev=tcurr;
        k++;
        tcurr=trapezeSquareMethod(CalcFanc,&a,&b, pow(2,k) * n);
    }
    printf("Square equals: %lf",tcurr);


    return 0;
}