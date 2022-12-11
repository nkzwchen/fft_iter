#include "fft.h"
#include <cstdlib>
#include <cstring>
#include <vector>

#define PRINT_INPUT
#define PRINT_OUTPUT
void test_case(int squence_length, std::vector<int>& decompose_factor, int test_case_num){
    // allocate memory
    float* ref_x = (float*)malloc(2 * squence_length * sizeof(float));
    float* x = (float*)malloc(2 * squence_length * sizeof(float));
    float* ref_output = (float*)malloc(2 * squence_length * sizeof(float));
    float* output = (float*)malloc(2 * squence_length * sizeof(float));

    for(int c = 0; c < test_case_num; c++){
        // generate test data
        generate_test_data(x, squence_length);
        #ifdef PRINT_INPUT
            printf("input data is\n");
            for(int i = 0; i < squence_length; i++){
                printf("%f + %fi, ", x[2 * i], x[2 * i + 1]);        
            }
            printf("\n");
        #endif
        memcpy(ref_x, x, 2 * squence_length * sizeof(float));

        // parallel fft iteration
        cl_fft_iteration(x, output, squence_length);

        //squential fft iteration
        fftiteration(NR, PBLOCK, TBLOCK, (complex_t*)ref_x, (complex_t*)ref_output);

        output = (float*)output;
        ref_out = (float*)ref_output;
        #ifdef PRINT_OUTPUT
            printf("result is: \n");
            for(int i = 0; i < squence_length; i++)
                printf("%d : (%f, %f)\n", i, output[2 * i], output[ 2 * i + 1]);
            printf("correct result is: \n");
            for(int i = 0; i < squence_length; i++)
                printf("%d : (%f, %f)\n", i, ref_x[2 * i], ref_x[ 2 * i + 1]);
        #endif
            int correct_num = check_correctness(output, ref_x, squence_length);
            printf("test case %d correct rate: %d / %d\n", c, correct_num, squence_length);
    }
    free(x);
    free(ref_x);
    free(ref_output)
    free(output);

}
int main(int argc, char** argv){
    if (argc < 3){
        printf("input param error\n");
        return -1;
    }
    std::vector<int> decompose_factor({PBLOCK, NR, BLOCK});
    int squence_length = 1;
    for(int f = 0; f < decompose_factor.size(); f++){
        squence_length *= decompose_factor[f];
    }
    int test_num = atoi(argv[argc - 1]);
    test_case(squence_length, decompose_factor, test_num);
    return 0;
}