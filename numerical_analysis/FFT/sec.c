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
complex tem[MAX_LENGTH], tem_extra[MAX_LENGTH];
void rader_reverse(complex *cmplx_arr, int n);
int main(){

    // printf("%d", sizeof(data)/sizeof(complex));
    // complex x = euler_formula((2 * M_PI) / 3.0);
    // coplx_print(&x);

    complex test[4] = {};
    test[0] = CMPLX(1.0, 0);
    test[1] = CMPLX(2.0, 0);
    test[2] = CMPLX(3.0, 0);
    test[3] = CMPLX(2.0, 0);

    bfft(test, 2, euler_formula(M_PI_2));
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
    ye = fft(pe, tem_extra, length / 2);
    yo = fft(po, tem, length / 2);

    for(int i = 0; i < length / 2; i++){
        resp[i] = ye[i] + euler_formula((2 * M_PI * i) / length) * yo[i];
        resp[i + length / 2] = ye[i] - euler_formula((2 * M_PI * i) / length) * yo[i];
    }

    return resp;
}

void bfft(complex* x, int p, complex w){
    int length = pow(2, p);
    complex resp[length];
    printf("before the fft:\n");
    for(int i = 0; i < length; i++){
        coplx_print(&x[i]);
    }
    
    fft(x, resp, length);

    printf("after the fft:\n");
    rader_reverse(resp, 4);
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

void rader_reverse(complex *cmplx_arr, int n)
{
    int j, k;
    // n = 3, for example
    for (int i = 1, j = n / 2; i < n - 1; i++)
    {
        if (i < j)
        {
            // first switch the 001 and 100
            int temp = cmplx_arr[j];
            cmplx_arr[j] = cmplx_arr[i];
            cmplx_arr[i] = temp;
        }
        k = n / 2;
        // the calculate the next one on the base of 100
        while (k <= j)
        {
            j = j - k;
            k = k / 2;
        }
        j = j + k;
    }
}