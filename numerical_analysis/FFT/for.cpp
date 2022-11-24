#include <iostream>
#include <bits/stdc++.h>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <algorithm>

// define this to use M_PI and other parameters
// to use the "typedef std::vector<unsigned char> Integer"
// we create a new class Complex instead of using the default one
#define _USE_MATH_DEFINES
#include <math.h>

typedef std::vector<unsigned char> Integer;

class Complex
{
public:
    double real_;
    double imag_;

    Complex(void);
    Complex(double const &real);
    Complex(double const &real, double const &imag);
    Complex(Complex const &v);
    Complex operator+(Complex const &a) const;
    Complex operator-(Complex const &a) const;
    Complex operator*(Complex const &a) const;
    Complex operator/(int n) const;
    Complex &operator=(const Complex &a);
    double getReal();
    double getImage();
};

Complex::Complex(void)
{
    real_ = 0;
    imag_ = 0;
}

Complex::Complex(double const &real)
{
    real_ = real;
    imag_ = 0;
}

Complex::Complex(double const &real, double const &imag)
{
    real_ = real;
    imag_ = imag;
}

Complex::Complex(Complex const &v)
{
    real_ = v.real_;
    imag_ = v.imag_;
}

Complex Complex::operator+(Complex const &a) const
{
    Complex result(real_ + a.real_, imag_ + a.imag_);
    return result;
}

Complex Complex::operator-(Complex const &a) const
{
    Complex result(real_ - a.real_, imag_ - a.imag_);
    return result;
}

Complex Complex::operator*(Complex const &a) const
{
    Complex result(real_ * a.real_ - imag_ * a.imag_, imag_ * a.real_ + real_ * a.imag_);
    return result;
}

Complex Complex::operator/(int n) const
{
    Complex result(real_ / n, imag_ / n);
    return result;
}

Complex &Complex::operator=(const Complex &a)
{
    // if use the same one, we can prevent it from self-copying
    if (this == &a)
        return *this;
    real_ = a.real_;
    imag_ = a.imag_;
    return *this;
}

double Complex::getReal()
{
    return real_;
}

double Complex::getImage()
{
    return imag_;
}

void rader_reverse(Complex *cmplx_arr, int length)
{
    int j, k;
    // n = 3, for example
    for (int i = 1, j = length / 2; i < length - 1; i++)
    {
        if (i < j)
        {
            // first switch the 001 and 100
            Complex temp = cmplx_arr[j];
            cmplx_arr[j] = cmplx_arr[i];
            cmplx_arr[i] = temp;
        }
        k = length / 2;
        // the calculate the next one on the base of 100
        while (k <= j)
        {
            j = j - k;
            k = k / 2;
        }
        j = j + k;
    }
}

void fft(Complex *dist, Complex *src, int N)
{
    int n = log(N) / log(2);
    rader_reverse(dist, N);
    for (int s = 1, m = 1; s <= n; s++)
    { //进行迭代过程
        m *= 2;
        Complex temp, u, omiga, omigaM = Complex(cos(-2 * M_PI / m), sin(-2 * M_PI / m));
        for (int k = 0; k < N; k = k + m)
        {
            omiga = Complex(1);
            for (int j = 0; j <= m / 2 - 1; j++)
            { //蝶形运算
                temp = omiga * dist[k + j + m / 2];
                u = dist[k + j];
                dist[k + j] = u + temp;
                dist[k + j + m / 2] = u - temp;
                omiga = omiga * omigaM;
            }
        }
    }
}

void ifft(Complex *dist, Complex *src, int N)
{
    int n = 0;
    for (int i = 1; i < N; i *= 2)
    { //求N的二进制位数
        n++;
    }
    for (int i = 0; i <= N - 1; i++)
    { //进行位反转置换
        int a = i;
        int b = 0;
        for (int j = 0; j < n; j++)
        { //生成a的反转b
            b = (b << 1) + (a & 1);
            a >>= 1;
        }
        if (b > i)
        { //进行置换
            dist[b] = src[b] + src[i];
            dist[i] = dist[b] - src[i];
            dist[b] = dist[b] - dist[i];
        }
    }
    for (int s = 1, m = 1; s <= n; s++)
    { // iterative process
        m *= 2;
        Complex temp, u, omiga, omigaM = Complex(cos(2 * M_PI / m), sin(2 * M_PI / m));
        for (int k = 0; k < N; k = k + m)
        {
            omiga = Complex(1);
            for (int j = 0; j <= m / 2 - 1; j++)
            { // butterfly shape
                temp = omiga * dist[k + j + m / 2];
                u = dist[k + j];
                dist[k + j] = u + temp;
                dist[k + j + m / 2] = u - temp;
                omiga = omiga * omigaM;
            }
        }
    }
    for (int i = 0; i < N; i++)
    {
        dist[i] = dist[i] / N;
    }
}

void cmplx_print(Complex &complex)
{
    // std::cout<< complex.getReal() << "+" << complex.getImage() << std::endl;
    printf("%lf + %lfi\n", complex.getReal(), complex.getImage());
}

void int_print(Integer &integer){
    for(int i = 0; i < integer.size(); i++){
        printf("%c", integer.at(i));
    }
    printf("\n");
}

// hand computation
void hand_computation(Integer *res, Integer *multiplier_l, Integer *multiplier_r)
{
    int i, j, temp, t, tt, num = 0;
    int l_l = multiplier_l->size();
    int l_r = multiplier_r->size();

    for (i = 0; i < l_l; i++)
    {
        // temp denote the carry bit
        temp = 0;
        for (j = 0; j < l_r; j++)
        {
            t = (multiplier_r->at(l_r - j - 1) - '0') * (multiplier_l->at(l_l - i - 1) - '0');
            if (t == 0)
                continue;
            num = j + i;
            tt = res->at(num) + t;
            res->at(num) = tt % 10;
            temp = tt / 10;
            if (temp > 0)
            {
                res->at(++num) += temp;
            }
        }
        for (i = 0; i <= num / 2; i++)
        {
            res->at(i) += res->at(num - i);
            res->at(num - i) = res->at(i) - res->at(num - i);
            res->at(i) = res->at(i) - res->at(num - i);
        }
        for (i = 0; i < num; i++)
        {
            res->at(i) = '0' + res->at(i);
        }
    }
}

void hand_computation_2(Integer *res, Integer *multiplier_l, Integer *multiplier_r){
    
    int left_legth = multiplier_l->size();
    int right_length = multiplier_r->size();

    for(int i = 0; i < left_legth + right_length; i++){
        res->at(i) = res -> at(i) - '0';
    }

    for(int i = 0; i < left_legth; i++){
        int carry = 0;
        for(int j = 0; j < right_length; j++){
            int temp = res->at(i + j) + ((multiplier_l->at(i) - '0') * (multiplier_r->at(j) - '0')) + carry;
            res->at(i + j) = temp % 10;
            carry = temp / 10;
        }
        res->at(i + right_length) = carry;
    }

    for(int i = 0; i < left_legth + right_length; i++){
        res->at(i) = res -> at(i) + '0';
    }
            
    int index = res->size() - 1;
    for(;;index--){
        if(res->at(index) != '0'){
            break;
        }
        res->pop_back();
    }

    std::reverse(res->begin(), res->end());
}

void test_fft_ifft()
{
    const int N = 4;

    Complex *src = new Complex[N];
    double real = 0, image = 0;

    for (int i = 0; i < N; i++)
    {
        src[i] = *(new Complex((double)i));
        cmplx_print(src[i]);
    }

    fft(src, src, N);
    for (int i = 0; i < N; i++)
    {
        cmplx_print(src[i]);
    }

    ifft(src, src, N);
    for (int i = 0; i < N; i++)
    {
        cmplx_print(src[i]);
    }

    return;
}

void test_hand_computation(){
    Integer i_l, res;
    i_l.resize(40, '1');
    res.resize(100, '0');
    hand_computation_2(&res, &i_l, &i_l);
    int_print(i_l);
    int_print(res);
}

int main()
{
    test_hand_computation();
}
