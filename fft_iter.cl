#define COMPLEX_MUL_REAL(x, y) (x.even * y.0 - x.odd * y.1)
#define COMPLEX_MUL_IMAGE(x, y) (x.even * y.1 + x.odd * y.0)

__kernel void fft_iter(
    __global int* Nr, __global int* P, 
    __global int* T, __global float2* X,
    __global float2* Y,
    __global float2* S
){
    
    __private const int Nr = *Nr;
    __private const int P = *P;
    __private const int T = *T;

    __private const int BLOCK_X = get_local_size(1);
    __private const int BLOCK_Y = get_local_size(0);

    __private const int global_x = get_local_id(1);
    __private const int global_y = get_local_id(0);

    __private const int local_x = get_local_id(1);
    __private const int local_y = get_local_id(0);

    __private const int start_idx = (local_x / Nr) * Nr * BLOCK_Y;
    __private const int fft_idx = local_x % Nr;

    // Load data from global memory to local memory
    S[local_x * BLOCK_Y + local_y] = X[global_x * T + global_y];
    work_group_barrier(CLK_LOCAL_MEM_FENCE);

    __private float2 sum = float2(0.0f, 0.0f);
    
    // Butterfly Transformation for fft squence with BLOCK_Y stride
    for(int _it = 0; _it < Nr; _it++){
        __private float2 cof = (float2)(cospi(2 * _it * fft_idx / Nr), -sinpi(2 * _it * fft_idx / Nr));

        sum.even += (COMPLEX_MUL_REAL(S[start_idx + _it * BLOCK_Y]， cof));
        sum.odd += (COMPLEX_MUL_REAL(S[start_idx + _it * BLOCK_Y]， cof));
    } 

    // Y[n, p, t] = X[p, n, t];   
    Y[(global_x % Nr) * P * T + (global_x / Nr) * T + global_y] = sum;
     
}