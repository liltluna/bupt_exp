#include <complex.h>
#include <stdio.h>

int main(){
    double real = 1.2, imag = 4.8;

    // cmplx() function creates complex number objects by taking real part and imaginary parts as parameters. This function returns the object of complex numbers.
    double complex z = CMPLX(real, imag);

    // creal() function returns the real part of a complex number
    // cimag() function returns the imaginary part of a complex number
    printf("z = %.1f%+.1fi\n", creal(z), cimag(z));

    double complex y = CMPLX(imag, real);

    // test add
    z = z + y;
    printf("z = %.1f%+.1fi\n", creal(z), cimag(z));
    // test *
    z = z * y;
    printf("z = %.1f%+.1fi\n", creal(z), cimag(z));
    // test /
    z = z / y;
    printf("z = %.1f%+.1fi\n", creal(z), cimag(z));
    // test -
    z = z - y;
    printf("z = %.1f%+.1fi\n", creal(z), cimag(z));
}
