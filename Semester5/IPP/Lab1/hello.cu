#include <stdio.h>

__global__ void hello() {
   printf("Hello world from block (%d,%d), thread (%d,%d,%d).\n",
           blockIdx.x, blockIdx.y, threadIdx.x, threadIdx.y, threadIdx.z);

   __syncthreads();
}

int main(void) {
   int devID;
   cudaDeviceProp p;

   cudaGetDevice(&devID);
   cudaGetDeviceProperties(&p, devID);
   printf("Running on device %d \"%s\" with capability %d.%d.\n",
           devID, p.name, p.major, p.minor);
   if (p.major < 2) {
      printf("Program incompatible with existing architecture; terminating.\n");
      return 1;
   }
   dim3 dimGrid(2,2);
   dim3 dimBlock(2,2,2);
   hello<<<dimGrid,dimBlock>>>();
   cudaDeviceSynchronize();
   return 0;
}

