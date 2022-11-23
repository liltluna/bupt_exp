#include <complex.h>
#include <stdio.h>
#include <math.h>

#define LENGTH 1 << 1

void rader_reverse(complex *cmplx_arr, int n);
void fft(complex *x, int n);
void cmplx_print(complex *x);
complex euler_formula(double x);

int main()
{
    // x is sample and X is result
    complex x[LENGTH], X[LENGTH];

    // produce the test sample
    // for (int i = 0; i < LENGTH; ++i)
    // {
    //     x[i] = CMPLX((double)i, 0.0);
    //     // copy to X, making preparation for FFT
    //     X[i] = x[i];
    // }

    X[0] = CMPLX(9.0, 0);
    X[1] = CMPLX(1.0, 0);

    fft(X, LENGTH);

    // print the result
    for (int i = 0; i < LENGTH; i++)
        cmplx_print(&X[i]);
    // to test the rader algorithm
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

void fft(complex* x, int n){
    int i, j, k, l;
    i = j = k = l = 0;
    complex up, down, product;
    rader_reverse(x, n);

    // w is the Butterfly shape coefficient
    complex w[n];
    for(int i = 0; i < n; i++){
        // w[i] = exp(CMPLX(0, (-2 * M_PI * i / n)));
        w[i] = euler_formula(-2 * M_PI * i / n);
    }

    for(int i = 0; i < log(n) / log(2); i++){
        l = 1 << i;
        for(j = 0; j < n; j += 2 * l){
            for(k = 0; k < l; k++){
                product = x[j + k + l] * w[n * k / 2 / l];
                up = x[j + k] + product;
                down = x[j + k] - product;
                x[j + k] = up;
                x[j + k + l] = down;
            }
        }
    }
}

// format print the complex x
void cmplx_print(complex *x)
{
    printf("z = %lf + %lfi\n", creal(*x), cimag(*x));
}

void rader_test()
{
    complex x[LENGTH], X[LENGTH];

    for (int i = 0; i < LENGTH; ++i)
    {
        x[i] = CMPLX((double)i, 0.0);
        X[i] = x[i];
    }

    printf("before rader\n");
    for (int i = 0; i < LENGTH; i++)
        cmplx_print(&X[i]);

    rader_reverse(X, LENGTH);

    for (int i = 0; i < LENGTH; i++)
        cmplx_print(&X[i]);
}

complex euler_formula(double x){
    return CMPLX(cos(x), sin(x));
}