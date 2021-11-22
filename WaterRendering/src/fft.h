#ifndef FFT_H
#define FFT_H

#include <math.h>
#include "complex.h"

#ifndef  M_PI
#define M_PI 3.1415926	//3.14159265358979323846
#endif // ! M_PI

class cFFT {
  private:
	unsigned int N, which;
	unsigned int log_2_N;
	float pi2;
	unsigned int *reversed;
	complex **T;
	complex *c[2];
  protected:
  public:
	cFFT(unsigned int N);
	~cFFT();
	unsigned int reverse(unsigned int i);
	complex t(unsigned int x, unsigned int N);
	void fft(complex* input, complex* output, int stride, int offset);
};

#endif