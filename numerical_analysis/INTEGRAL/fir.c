// Based on the provided functions.
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <math.h>
#include <time.h>

#define SEGMENT_NUMBER 16
#define ROMBERG_LINE_NUMBER 12

clock_t start,end;

double gauss_ch1(double (*f)(double), int n);
double gauss_ch2(double (*f)(double), int n);
double gauss_leg_9(double (*f)(double));
double comp_gauss_leg(double (*f)(double), double A, double B);
double comp_trep(double (*f)(double), double a, double b);
double romberg(double(*f)(double), double a, double b);

// target function
double f_1(double x){
    return x * exp(x);
}

double f_2(double x){
    return 0.0;
}

int main()
{
    double result = 0.0;

    printf("1---gauss_ch1:\nresult: ");
    start = clock();
    result = gauss_ch1(f_1, SEGMENT_NUMBER);
    end = clock();
    printf("%f\ntime: %f\n\n",result, (double)(end-start) / CLK_TCK);

    printf("2---gauss_ch2:\nresult: ");
    start = clock();
    result = gauss_ch2(f_1, SEGMENT_NUMBER);
    end = clock();
    printf("%f\ntime: %f\n\n",result, (double)(end-start) / CLK_TCK);

    printf("3---gauss_leg_9:\nresult: ");
    start = clock();
    result = gauss_leg_9(f_1);
    end = clock();
    printf("%f\ntime: %f\n\n",result, (double)(end-start) / CLK_TCK);

    printf("4---comp_gauss_leg:\nresult: ");
    start = clock();
    result = comp_gauss_leg(f_1, -1, 1);
    end = clock();
    printf("%f\ntime: %f\n\n",result, (double)(end-start) / CLK_TCK);

    printf("5---comp_trep:\nresult: ");
    start = clock();
    result = comp_trep(f_1, -1, 1);
    end = clock();
    printf("%f\ntime: %f\n\n",result, (double)(end-start) / CLK_TCK);

    printf("6---romberg:\nresult: ");
    start = clock();
    result = romberg(f_1, -1, 1);
    end = clock();
    printf("%f\ntime: %f\n\n",result, (double)(end-start) / CLK_TCK);

    return 0;
}

// first
double gauss_ch1(double (*f)(double), int n)
{
    double integral_sum = 0.0;
    double start = M_PI / (2 * n);
    double step = M_PI / n;

    for (int k = 1; k < n + 1; k++)
    {
        integral_sum += f(cos(start + step * (k - 1)));
    }

    return integral_sum * (M_PI / n);
}

// second
double gauss_ch2(double (*f)(double), int n)
{
    double integral_sum = 0.0;
    double start = M_PI / (n + 1);
    double step = start;

    for (int k = 1; k < n + 1; k++)
    {
        integral_sum += f(cos(start + step * (k - 1))) * pow(sin(start + step * (k - 1)), 2);
    }

    return integral_sum * (M_PI / (n + 1));
}

// third
double gauss_leg_9(double (*f)(double))
{
    double abscissa[9] = {0.0000000000000000, -0.8360311073266358, 0.8360311073266358, -0.9681602395076261, 0.9681602395076261, -0.3242534234038089, 0.3242534234038089, -0.6133714327005904, 0.6133714327005904};
    double weight[9] = {0.3302393550012598, 0.1806481606948574, 0.1806481606948574, 0.0812743883615744, 0.0812743883615744, 0.3123470770400029, 0.3123470770400029, 0.2606106964029354, 0.2606106964029354};
    double integral_sum = 0.0;

    for (int i = 0; i < 9; i++)
    {
        integral_sum += weight[i] * f(abscissa[i]);
    }

    return integral_sum;
}

// forth
double comp_gauss_leg(double (*f)(double), double A, double B)
{
    double intergral_sum = 0.0;
    double start = A;
    double step = (B - A) / SEGMENT_NUMBER;

    for (int i = 0; i < SEGMENT_NUMBER; i++)
    {
        intergral_sum += (step / 2) * (f((-step) / (2 * sqrt(3)) + (2 * start + (2 * i + 1) * step) / 2) + f((step) / (2 * sqrt(3)) + (2 * start + (2 * i + 1) * step) / 2));
    }

    return intergral_sum;
}

// fifth
double comp_trep(double (*f)(double), double a, double b)
{
    double start = a;
    double step = (b - a) / SEGMENT_NUMBER;
    double intergral_sum = 0.0;

    for (int i = 1; i < SEGMENT_NUMBER; i++)
    {
        intergral_sum += f(start + step * i);
    }

    intergral_sum = 2 * intergral_sum + f(a) + f(b);

    return (intergral_sum * step) / 2;
}

// sixth
double romberg(double(*f)(double), double a, double b)
{
    double step_line_i = 0.0;
    double romberg_table[ROMBERG_LINE_NUMBER + 1][ROMBERG_LINE_NUMBER + 1];
    for(int i = 0; i < ROMBERG_LINE_NUMBER + 1; i++)
    {
        for(int j = 0; j < ROMBERG_LINE_NUMBER + 1; j++)
        {
            romberg_table[i][j] = 0;
        }
    }

    romberg_table[1][1] = (b - a) * (f(a) + f(b)) / 2;
    for(int i = 2; i < ROMBERG_LINE_NUMBER +1; i++)
    {
        double subtotal = 0;
        step_line_i = (b - a) / pow(2, i);

        for(int j = 1; j < pow(2, j - 2) + 1; i++)
        {
            subtotal += f(a + (2 * i - 1) * step_line_i);
        }

        romberg_table[i][1] = romberg_table[i - 1][1] / 2 + step_line_i * subtotal;

        for(int j = 2; j < i + 1; j++)
        {
            romberg_table[i][j] = (pow(4, j - 1) * romberg_table[i][j - 1] - romberg_table[i - 1][j - 1]) / (pow(4, j - 1) - 1);
        }
    }

    return romberg_table[ROMBERG_LINE_NUMBER][ROMBERG_LINE_NUMBER];

}