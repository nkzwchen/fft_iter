#ifndef FFT_H
#define MAX 1
#define MIN 0
#define ERROR_TOLERANCE 1e-4
#include<vector>
#include<complex>
#include<CL/cl.h>
typedef std::complex<float> complex_t;

// tool.cpp
void generate_test_data(float* input_data, int squence_length);
char* read_cl_file(const char* file_name);
int check_correctness(const float* output, const float* ref_output, int squence_length);
void cl_gpu_init(cl_platform_id& platform, cl_device_id& device, cl_context& context, cl_queue& queue, cl_program& program, const char* program_source_path);

// cl_fft.cpp
void cl_fft(float* X, int squence_length);
void fftiteration(int Nr, int P, int T, complex_t* dataI, complex_t* dataO);
void fftalongdim(int Nr, int P, int T, int p, int t, complex_t* dataI);
void partial_reverse(int Nr, int P, int T, int p, int t, complex_t* dataI, complex_t* dataO);
void microfft(int Nr, int input_idx,  int strideI, complex_t* dataI);

// squential_fft.cpp
void squential_fft(int N, std::vector<int>& decompose_factor, complex_t* dataI, complex_t* dataO);
#endif