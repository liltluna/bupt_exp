#include <iostream>
#include <bits/stdc++.h>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <algorithm>

// define this to use M_PI and other parameters
#define _USE_MATH_DEFINES
#include <math.h>

// to use the "typedef std::vector<unsigned char> Integer"
// we create a new class Complex instead of using the default one
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
    { 
        // start the process of iteration
        m *= 2;
        Complex temp, u, omiga, omigaM = Complex(cos(-2 * M_PI / m), sin(-2 * M_PI / m));
        for (int k = 0; k < N; k = k + m)
        {
            omiga = Complex(1);
            for (int j = 0; j <= m / 2 - 1; j++)
            { // butterfly operation
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
    int n = log(N) / log(2);
    rader_reverse(dist, N);
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
    printf("%lf + %lfi\n", complex.getReal(), complex.getImage());
}

void int_print(Integer &integer){
    for(int i = 0; i < integer.size(); i++){
        printf("%c", integer.at(i));
    }
    printf("\n");
}

// this function to transform a vector<unsigned char> to something like vector<int>
void transform_integer(Integer *integer){
    for(int i = 0; i < integer->size(); i++){
        integer->at(i) = integer->at(i) > '9' ? integer->at(i) - '7' : integer->at(i) - '0';
    }
}

// this function eliminate the conversion of last function
void recover_integer(Integer *integer){
    for(int i = 0; i < integer->size(); i++){
        integer->at(i) = integer->at(i) > 9 ? integer->at(i) + '7' : integer->at(i) + '0';
    }
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
    int b = 16;

    int left_length = multiplier_l->size();
    int right_length = multiplier_r->size();

    // transform the vactors
    transform_integer(res);
    transform_integer(multiplier_l);
    if(multiplier_l != multiplier_r)
        transform_integer(multiplier_r);

    // simulate hand computation
    for(int i = 0; i < left_length; i++){
        int carry = 0;
        for(int j = 0; j < right_length; j++){
            int temp = res->at(i + j) + ((multiplier_l->at(i)) * (multiplier_r->at(j))) + carry;
            res->at(i + j) = temp % b;
            carry = temp / b;
        }
        res->at(i + right_length) = carry;
    }

    recover_integer(res);
    recover_integer(multiplier_l);
    if(multiplier_l != multiplier_r)
        recover_integer(multiplier_r);
    
    // delete the '0's
    int index = res->size() - 1;
    for( ; ;index--){
        if(res->at(index) != '0'){
            break;
        }
        res->pop_back();
    }

    std::reverse(res->begin(), res->end());
}

// realize the long integer multiplication
void long_integer_with_fft(Integer* res, Integer *integer_left, Integer *integer_right)
{
    int base = 16;
    int left_length, right_length, sum_length;

    left_length = integer_left->size();
    right_length = integer_right->size();
    sum_length = left_length + right_length;

    int N = pow(2, (int)(log(sum_length) / log(2)) + 1);

    Complex* left_extend_complex = new Complex[N];
    Complex* right_extend_complex = new Complex[N];
    Complex* result_complex = new Complex[N];

    transform_integer(res);
    transform_integer(integer_right);
    if(integer_right != integer_left)
        transform_integer(integer_left);

    for (int i = 0; i < left_length ;i++) {
        left_extend_complex[i] = Complex((double)(integer_left->at(left_length - i -1)));
    }

    for (int i = 0; i < right_length ;i++) {
        right_extend_complex[i] = Complex((double)(integer_right->at(right_length - i -1)));
    }

    fft(left_extend_complex, left_extend_complex, N);
    fft(right_extend_complex, right_extend_complex, N);

    for(int i = 0; i < N; i++) {
        result_complex[i] = left_extend_complex[i] * right_extend_complex[i];
    }

    ifft(result_complex, result_complex, N);

    int current_number = 0, round_up = 0;
    for(int i = 0; i < sum_length; i++) {
        current_number = (int)result_complex[i].getReal() + round_up;
        res->at(i) = current_number % base;
        round_up = current_number / base;
    }

    int index = res->size() - 1;
    for( ; ;index--){
        if(res->at(index) != 0){
            break;
        }
        res->pop_back();
    }

    // recover the integer
    recover_integer(res);
    recover_integer(integer_left);
    if(integer_right != integer_left)
        recover_integer(integer_left);

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

    i_l.resize(6, 'A');
    res.resize(100, '0');

    hand_computation_2(&res, &i_l, &i_l);
    int_print(i_l);
    int_print(res);
}

void test_long_integer_fft(){
    Integer i_l, res;

    i_l.resize(6, 'A');
    res.resize(100, '0');

    long_integer_with_fft(&res, &i_l, &i_l);
    int_print(i_l);
    int_print(res);
}

int main () 
{
    test_long_integer_fft();
    test_hand_computation();
    return 0;
}