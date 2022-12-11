#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<vector>
#include "fft.h"
using namespace std;
// #define PRINT_CALL_STACK

void squential_fft(int N, vector<int>& decompose_factor, complex_t* dataI, complex_t* dataO){
    int P = 1;
    int T = N;
    for(int r = 0; r < decompose_factor.size(); r++){
        int Nr = decompose_factor[r];
        T /= Nr;
        fftiteration(Nr, P, T, dataI, dataO);
        P *= Nr;
        for(int i = 0; i < N; i++)
            dataI[i] = dataO[i];
    }
    
}

void fftiteration(int Nr, int P, int T, complex_t* dataI, complex_t* dataO){
    #ifdef PRINT_CALL_STACK
    printf("fftiteration(Nr: %d, P: %d, T: %d)\n", Nr, P, T);
    #endif
    for(int p = 0; p < P; p++){
        for(int t = 0; t < T; t++){
            fftalongdim(Nr, P, T, p, t, dataI);
            partial_reverse(Nr, P, T, p, t, dataI, dataO);
        }
    }
}

void fftalongdim(int Nr, int P, int T, int p, int t, complex_t* dataI){
    int input_idx = p * Nr * T + 0 * T + t;
    int strideI = T;
    microfft(Nr, input_idx, strideI, dataI);
    if (T != 1){
        const float theta1 = 2 * M_PI / (Nr * T);
        for(int j = 0; j < Nr; j++){
            complex_t wp =  complex_t(cos((j * t * theta1)), -sin((j * t * theta1)));
            dataI[input_idx + j * strideI] *= wp;
        }            
    }
}

void partial_reverse(int Nr, int P, int T, int p, int t, complex_t* dataI, complex_t* dataO){
    int input_idx = p * Nr * T + 0 * T + t;
    int strideI = T;
    int output_idx = 0 * P * T + p * T + t;
    int strideO = P * T;
    for(int k = 0; k < Nr; k++){
        dataO[output_idx + k * strideO] = dataI[input_idx + k * strideI];
    }

}

void microfft(int Nr, int input_idx,  int strideI, complex_t* dataI){
    const float theta0 = 2 * M_PI / Nr;
    complex_t* v = new complex_t[Nr];
    for(int k = 0; k < Nr; k++){
        complex_t sum = complex_t(0, 0);
        for(int j = 0; j < Nr; j++){
            complex_t wp = complex_t(cos(j * k * theta0) , -sin(j * k * theta0));
            sum += (dataI[input_idx + j * strideI] * wp);
        }
        v[k] = sum;
    }
    for(int j = 0; j < Nr; j++){
        dataI[input_idx + j * strideI] = v[j];
    }
    delete[] v;
}