#!
/usr/local/cuda/bin/nvcc -I/usr/local/cutil/include $1.cu -o $1 -arch=sm_20 -L/usr/local/cutil/lib -lcutil_x86_64

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/lib ./$1
