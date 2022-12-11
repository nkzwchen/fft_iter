CLTEST := test

CC = g++
CLFFT_INCLUDE_DIR = /home/wenchen/j2vFFT/src/include/
CLFFT_LD_DIR = /home/wenchen/j2vFFT/cbuild/library/

CFLAGS = -g -O2 -std=c++11 -I$(CLFFT_INCLUDE_DIR) -DCL_TARGET_OPENCL_VERSION=120
LDFLAGS = -lOpenCL -L$(CLFFT_LD_DIR) -lclFFT

CLTEST_OBJS = squential_fft.o cl_fft.o tool.o test.o

$(CLTEST): $(CLTEST_OBJS)
	$(CC) -o $@ $(CLTEST_OBJS) $(LDFLAGS)

$(CLTEST_OBJS) : %.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CLTEST_OBJS) $(CLTEST)