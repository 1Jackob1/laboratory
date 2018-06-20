#include <stdio.h>
#include <math.h>

int GetStat(double a, double b, double c, double* x1, double* x2){

    if(a==b && c==b && c==0)//return -1
        return -1;
    else if(a==b && b==0 && c!=0){
        return 0;
    }
    else{
        double d=b*b-a*c*4;
        if(d<0){//return -2
            d*=-1;
            *x1=-b/2/a;
            *x2=sqrt(d)/2;
            return -2;
        }
        else if(d==0){
            *x1=-b/2;
            return 1;
        }
        else{
            *x1=-b/2+sqrt(d);
            *x2=-b/2-sqrt(d);
            return 2;
        }
    }

}

int main() {
    double a,b,c, x1, x2;
    scanf("%lf%lf%lf",&a,&b,&c);
    switch(GetStat(a,b,c,&x1,&x2)){
        case -2:
            printf("%lf-%lfi",x1,x2);
            break;
        case -1:
            printf("There are INF roots");
            break;
        case 0:
            printf("There aren't any roots");
            break;
        case 1:
            printf("%lf",x1);
            break;
        case 2:
            printf("%lf \n %lf",x1,x2);
            break;
    }

    return 0;
}