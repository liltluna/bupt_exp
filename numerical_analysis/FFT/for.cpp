#include <iostream>
#include <bits/stdc++.h>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <ctime>
#include <algorithm>

// define this to use M_PI and other parameters
#define _USE_MATH_DEFINES
#include <math.h>

#define MAX_RES_LENGTH 2048

// to use the "typedef std::vector<unsigned char> Integer"
// we create a new class Complex instead of using the default one
typedef std::vector<unsigned char> Integer;

clock_t start, end;
int counter = 0;

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
    Complex operator/(Complex const &a) const;
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

Complex Complex::operator/(Complex const &a) const
{
    double areal = real_ * a.real_ + imag_ * a.imag_;
    double breal = imag_ * a.real_ - real_ * a.imag_;
    double n = a.real_ * a.real_ + a.imag_ * a.imag_;
    Complex result(areal / n, breal / n);
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

void int_print(Integer &integer)
{
    for (int i = 0; i < integer.size(); i++)
    {
        printf("%c", integer.at(i));
    }
    printf("\n");
}

void int_print_with_name(Integer &integer, const char* string){
    printf("%s = ", string);
    for (int i = 0; i < integer.size(); i++)
    {
        printf("%c", integer.at(i));
    }
    printf("\n");
}

// this function to transform a vector<unsigned char> to something like vector<int>
void transform_integer(Integer *integer)
{
    for (int i = 0; i < integer->size(); i++)
    {
        integer->at(i) = integer->at(i) > '9' ? integer->at(i) - '7' : integer->at(i) - '0';
    }
}

// this function eliminate the conversion of last function
void recover_integer(Integer *integer)
{
    for (int i = 0; i < integer->size(); i++)
    {
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

void hand_computation_2(Integer *res, Integer *multiplier_l, Integer *multiplier_r, int base_)
{
    int base = base_;

    int left_length = multiplier_l->size();
    int right_length = multiplier_r->size();

    // transform the vactors
    transform_integer(res);
    transform_integer(multiplier_l);
    if (multiplier_l != multiplier_r)
        transform_integer(multiplier_r);

    std::reverse(multiplier_l->begin(), multiplier_l->end());
    std::reverse(multiplier_r->begin(), multiplier_r->end());

    // start timer 
    // --------------------------------------------------
    // start = clock();
    // --------------------------------------------------

    // simulate hand computation
    for (int i = 0; i < left_length; i++)
    {
        int carry = 0;
        for (int j = 0; j < right_length; j++)
        {
            int temp = res->at(i + j) + ((multiplier_l->at(i)) * (multiplier_r->at(j))) + carry;
            res->at(i + j) = temp % base;
            carry = temp / base;
        }
        res->at(i + right_length) = carry;
    }

    // end timer 
    // --------------------------------------------------
    // end = clock();
    // printf("had,%d,%lf\n", counter, (double)(end-start)/CLOCKS_PER_SEC);
    // --------------------------------------------------

    recover_integer(res);
    recover_integer(multiplier_l);
    if (multiplier_l != multiplier_r)
        recover_integer(multiplier_r);

    // delete the '0's
    int index = res->size() - 1;
    for (;; index--)
    {
        if (res->at(index) != '0')
        {
            break;
        }
        res->pop_back();
    }

    std::reverse(res->begin(), res->end());
    std::reverse(multiplier_l->begin(), multiplier_l->end());
    std::reverse(multiplier_r->begin(), multiplier_r->end());
}

// realize the long integer multiplication
void long_integer_with_fft(Integer *res, Integer *integer_left, Integer *integer_right, int base_)
{
    int base = base_;
    int left_length, right_length, sum_length;

    left_length = integer_left->size();
    right_length = integer_right->size();
    sum_length = left_length + right_length;

    int N = pow(2, (int)(log(sum_length) / log(2)) + 1);

    Complex *left_extend_complex = new Complex[N];
    Complex *right_extend_complex = new Complex[N];
    Complex *result_complex = new Complex[N];

    transform_integer(res);
    transform_integer(integer_right);
    if (integer_right != integer_left)
        transform_integer(integer_left);

    for (int i = 0; i < left_length; i++)
    {
        left_extend_complex[i] = Complex((double)(integer_left->at(left_length - i - 1)));
    }

    for (int i = 0; i < right_length; i++)
    {
        right_extend_complex[i] = Complex((double)(integer_right->at(right_length - i - 1)));
    }
    // start timer 
    // --------------------------------------------------
    // start = clock();
    // --------------------------------------------------

    fft(left_extend_complex, left_extend_complex, N);
    fft(right_extend_complex, right_extend_complex, N);

    for (int i = 0; i < N; i++)
    {
        result_complex[i] = left_extend_complex[i] * right_extend_complex[i];
    }

    ifft(result_complex, result_complex, N);

    int current_number = 0, round_up = 0;
    for (int i = 0; i < sum_length; i++)
    {
        current_number = (int)result_complex[i].getReal() + round_up;
        res->at(i) = current_number % base;
        round_up = current_number / base;
    }

    int index = res->size() - 1;
    for (;; index--)
    {
        if (res->at(index) != 0)
        {
            break;
        }
        res->pop_back();
    }

    // end timer 
    // --------------------------------------------------
    // end = clock();
    // printf("fft,%d,%lf\n", counter, (double)(end-start)/CLOCKS_PER_SEC);
    // --------------------------------------------------


    // recover the integer
    recover_integer(res);
    recover_integer(integer_left);
    if (integer_right != integer_left)
        recover_integer(integer_right);

    std::reverse(res->begin(), res->end());
}

// forth problem
void multiply(Integer *rst, Integer const &a, Integer const &b)
{
    Integer tem_a = a;
    Integer tem_b = b;
    rst->resize(MAX_RES_LENGTH, '0');

    int base = 10;

    for(int i = 0; i < tem_a.size(); i++){
        if(tem_a.at(i) > '9')
        {
            base = 16;
            break;
        }
    }

    // printf("Performing fft long integer multiplication\n");
    long_integer_with_fft(rst, &tem_a, &tem_b, base);
    // int_print_with_name(tem_a, "a");
    // int_print_with_name(tem_b, "b");
    // int_print_with_name(*rst, "result");
}

// fifth problem
void multiply_check(Integer *rst, Integer const &a, Integer const &b)
{
    Integer tem_a = a;
    Integer tem_b = b;
    rst->resize(MAX_RES_LENGTH, '0');

    int base = 10;

    for(int i = 0; i < tem_a.size(); i++){
        if(tem_a.at(i) > '9')
        {    
            base = 16;
            break;
        }
    }
    // printf("Performing hand way integer multiplication\n");
    hand_computation_2(rst, &tem_a, &tem_b, base);
    // int_print_with_name(tem_a, "a");
    // int_print_with_name(tem_b, "b");
    // int_print_with_name(*rst, "result");
}

// sixth problem
void long_integer_cal_check(){
    Integer a, b, c, d;
    Integer res_1, res_2, res_3, res_4;
    int base;

    for (int i = 1; i < 16; i++)
    {
        if(i < 10)
            b.push_back(10 - i);
        a.push_back(i);
    }

    c = a;
    d = b;

    std::reverse(c.begin(), c.end());
    std::reverse(d.begin(), d.end());

    recover_integer(&a);
    recover_integer(&b);
    recover_integer(&c);
    recover_integer(&d);

    
   for(int i = 0; i < 1000; i++){
        a.push_back('0');
        c.push_back('0');
        if(i < 800){
            b.push_back('0');
            d.push_back('0');
        }
    }

    multiply(&res_1, a, c);
    multiply_check(&res_2, a, c);


    multiply(&res_3, b, d);
    multiply_check(&res_4, b, d);
}

// seventh problem 
void time_comparing_test(){
    Integer fir_integer, sec_integer;
    fir_integer.resize(1, 'A');
    sec_integer.resize(1, 'B');

    for(int i = 0; i < 200; i++){
        Integer res;
        multiply(&res, fir_integer, sec_integer);
        Integer res_h;
        multiply_check(&res_h, fir_integer, sec_integer);
        fir_integer.push_back('A');
        sec_integer.push_back('B');
        ++counter;
    }
}

// eigth problem 
void accurency_test(){
    Integer fir_integer, sec_integer;
    fir_integer.resize(1, '1');
    sec_integer.resize(1, '1');

    for(int i = 0; i < 100; i++){
        Integer res;
        multiply(&res, fir_integer, sec_integer);
        Integer res_h;
        multiply_check(&res_h, fir_integer, sec_integer);
        if(res.size() != res_h.size()){
            printf("%d\n", counter);
            return;
        }
        for(int i = 0; i < res.size(); i++){
            if(res.at(i) != res_h.at(i)){
                printf("%d\n", counter);
                return;
            }
        }
        fir_integer.push_back('1');
        sec_integer.push_back('1');
        ++counter;
    }
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

void test_hand_computation()
{
    Integer i_l, res;

    i_l.resize(100, 'A');
    res.resize(300, '0');

    hand_computation_2(&res, &i_l, &i_l, 16);
    int_print(i_l);
    int_print(res);
}

void test_long_integer_fft()
{
    Integer i_l, res;

    i_l.resize(100, 'A');
    res.resize(300, '0');

    long_integer_with_fft(&res, &i_l, &i_l, 16);
    int_print(i_l);
    int_print(res);
}

void test_class_complex(){
    printf("Creating some complex using constructors...\n");
    Complex *a = new Complex(2.0, 4.0);
    Complex *b = new Complex(1.0, 3.0);
    Complex *c = new Complex();

    printf("result:a, b, c\n");
    cmplx_print(*a);
    cmplx_print(*b);
    cmplx_print(*c);

    printf("c = a + b\n");
    (*c) = (*a) + (*b);
    cmplx_print(*c);

    printf("c = a - b\n");
    (*c) = (*a) - (*b);
    cmplx_print(*c);

    printf("c = a * b\n");
    (*c) = (*a) * (*b);
    cmplx_print(*c);

    printf("c = a / b\n");
    (*c) = (*a) / (*b);
    cmplx_print(*c);

}

int main()
{
    // Integer a, b, res, res_;
    // a.push_back('A');
    // b.push_back('A');
    // multiply(&res, a, b);
    // multiply_check(&res_, a, b);
    // test_class_complex();
    // long_integer_cal_check();
    // test_hand_computation();
    // time_comparing_test();
    accurency_test();
}