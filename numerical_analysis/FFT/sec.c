#include <stdio.h>
#include <complex.h>
#include <math.h>

#define MAX_LENGTH (1 << 10) -1
#define LENGTH pow(2, 3)

complex data[MAX_LENGTH];

void bfft(complex* x, int p, complex w);
void coplx_print(complex* x);

complex euler_formula(double x);
complex* fft(complex* p, complex* resp, int length);


int main(){

    data[0] = CMPLX(9, 0);
    data[1] = CMPLX(1, 0);

    // printf("%d", sizeof(data)/sizeof(complex));
    // complex x = euler_formula((2 * M_PI) / 3.0);
    // coplx_print(&x);

    complex test[2] = {};
    for(int i = 0; i < 2; i++){
        test[i] = data[i];
    }

    bfft(test, 1, euler_formula(M_PI_2));
}

// realize the fft with output in bit reverse
// p indicate there are pow(2, p) terms
complex* fft(complex* p, complex *resp, int length){
    
    if(length == 1){
        return p;
    }

    complex w = euler_formula((2 * M_PI) / length);
    complex pe[length / 2], po[length / 2];

    for(int i = 0; i < length / 2; i++){
        pe[i] = p[2 * i];
        po[i] = p[2 * i + 1];
    }

    complex *ye, *yo;
    ye = fft(pe, resp, length / 2);
    yo = fft(po, resp, length / 2);

    for(int i = 0; i < length / 2; i++){
        resp[i] = ye[i] + euler_formula((2 * M_PI * i) / length) * yo[i];
        resp[i + length / 2] = ye[i] - euler_formula((2 * M_PI * i) / length) * yo[i];
    }

    return resp;
}

void bfft(complex* x, int p, complex w){
    int length = pow(2, p);
    complex resp[2];
    printf("before the fft:\n");
    for(int i = 0; i < length; i++){
        coplx_print(&x[i]);
    }
    
    fft(x, resp, length);

    printf("after the fft:\n");
    for(int i = 0; i < length; i++){
        coplx_print(&resp[i]);
    }
}

void coplx_print(complex* x){
    printf("z = %lf + %lfi\n", creal(*x), cimag(*x));
}

complex euler_formula(double x){
    return CMPLX(cos(x), sin(x));
}