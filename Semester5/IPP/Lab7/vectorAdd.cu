#include <stdio.h> 
#include <stdlib.h> 
#include <cutil.h>

__global__ void vecAdd(float * in1, float * in2, float * out, int len) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  out[i] = in1[i] + in2[i];
}

int main(int argc, char * * argv) {
  int inputLength1, inputLength2, outputLength;
  float * hostInput1, * hostInput2, * hostOutput, * deviceInput1, * deviceInput2, * deviceOutput, * expectedOutput;

  FILE * infile1, * infile2, * outfile;
  unsigned int generic, gpu, copy, compute, blog = 1;

  // Import host input data
  CUT_SAFE_CALL(cutCreateTimer( & generic));
  cutStartTimer(generic);
  if ((infile1 = fopen("input0.raw", "r")) == NULL) {
    printf("Cannot open input0.raw.\n");
    exit(EXIT_FAILURE);
  }
  if ((infile2 = fopen("input1.raw", "r")) == NULL) {
    printf("Cannot open input1.raw.\n");
    exit(EXIT_FAILURE);
  }
  fscanf(infile1, "%i", & inputLength1);
  hostInput1 = (float * ) malloc(sizeof(float) * inputLength1);
  for (int i = 0; i < inputLength1; i++)
    fscanf(infile1, "%f", & hostInput1[i]);
  fscanf(infile2, "%i", & inputLength2);
  hostInput2 = (float * ) malloc(sizeof(float) * inputLength2);
  for (int i = 0; i < inputLength2; i++)
    fscanf(infile2, "%f", & hostInput2[i]);
  fclose(infile1);
  fclose(infile2);
  hostOutput = (float * ) malloc(sizeof(float) * inputLength1);
  cutStopTimer(generic);
  printf("Importing data and creating memory on host: %f ms\n", cutGetTimerValue(generic));

  if (blog) printf("*** The input length is %i\n", inputLength1);

  CUT_SAFE_CALL(cutCreateTimer( & gpu));
  cutStartTimer(gpu);

  int size = sizeof(float) * inputLength1;
  cudaMalloc((void * * ) & deviceInput1, size);
  cudaMalloc((void * * ) & deviceInput2, size);
  cudaMalloc((void * * ) & deviceOutput, size);

  cutStopTimer(gpu);
  printf("Allocating GPU memory: %f ms\n", cutGetTimerValue(gpu));

  cutDeleteTimer(gpu);
  CUT_SAFE_CALL(cutCreateTimer( & gpu));
  cutStartTimer(gpu);

  cudaMemcpy(deviceInput1, hostInput1, size, cudaMemcpyHostToDevice);
  cudaMemcpy(deviceInput2, hostInput2, size, cudaMemcpyHostToDevice);
  cudaMemcpy(deviceOutput, hostOutput, size, cudaMemcpyHostToDevice);

  cutStopTimer(gpu);
  printf("Copying input memory to the GPU: %f ms\n", cutGetTimerValue(gpu));

  dim3 gridDim(ceil(inputLength1 / 4), 1, 1);
  dim3 blockDim(inputLength1 / 4, 1, 1);

  if (blog) printf("*** Block dimension is %i\n", blockDim.x);
  if (blog) printf("*** Grid dimension is %i\n", gridDim.x);

  CUT_SAFE_CALL(cutCreateTimer( & compute));
  cutStartTimer(compute);

  vecAdd << < gridDim, blockDim >>> (deviceInput1, deviceInput2, deviceOutput, inputLength1);

  cudaDeviceSynchronize();

  cutStopTimer(compute);
  printf("Performing CUDA computation: %f ms\n", cutGetTimerValue(compute));

  CUT_SAFE_CALL(cutCreateTimer( & copy));
  cutStartTimer(copy);

  cudaMemcpy(hostOutput, deviceOutput, size, cudaMemcpyDeviceToHost);

  cutStopTimer(copy);
  printf("Copying output memory to the CPU: %f ms\n", cutGetTimerValue(copy));

  cutDeleteTimer(gpu);
  CUT_SAFE_CALL(cutCreateTimer( & gpu));
  cutStartTimer(gpu);

  cudaFree(deviceInput1);
  cudaFree(deviceInput2);
  cudaFree(deviceOutput);

  cutStopTimer(gpu);
  printf("Freeing GPU Memory: %f ms\n", cutGetTimerValue(gpu));

  if ((outfile = fopen("output.raw", "r")) == NULL) {
    printf("Cannot open output.raw.\n");
    exit(EXIT_FAILURE);
  }
  fscanf(outfile, "%i", & outputLength);
  expectedOutput = (float * ) malloc(sizeof(float) * outputLength);
  for (int i = 0; i < outputLength; i++)
    fscanf(outfile, "%f", & expectedOutput[i]);
  fclose(outfile);
  int test = 1;
  int count = 0;
  for (int i = 0; i < outputLength; i++) {
    if (abs(expectedOutput[i] - hostOutput[i]) > 0.005) {
      test = 0;
      break;
    }
  }
  if (test) printf("Results correct.\n");
  else printf("Results incorrect.%d\n", count);

  cutDeleteTimer(generic);
  cutDeleteTimer(gpu);
  cutDeleteTimer(copy);
  cutDeleteTimer(compute);

  free(hostInput1);
  free(hostInput2);
  free(hostOutput);
  free(expectedOutput);

  return 0;
}
