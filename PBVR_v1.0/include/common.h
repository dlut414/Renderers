/*
LICENCE
*/
//common.h
//file to save common parameters
#ifndef COMMON_H
#define COMMON_H

#include "typedef.h"

///dimention
#define DIM 3

///debug
#define DEBUG
//#define DEBUG_

///omp configuration
#define CPU_OMP         1
#define OMP_THREADS     8
#define STATIC_CHUNK    500
#define STATIC_CHUNK_S  100
#define DYNAMIC_CHUNK   100
#define DYNAMIC_CHUNK_S 50

///gpu configuration
#define GPU_CUDA        1
#define GPU_CUDA_DEP    0
#define RENDER          1
#define TIMER           1
#define GRID_DIM_X      4
#define BLOCK_DIM_X_S   512
#define BLOCK_DIM_X_L   1024
#define USE_SHARED_MEM  0

#define A(i,j,ld) A[i*ld+j]

///rendering
#define REPEAT_LEVEL    100

namespace mytype
{

static const char LOG_NAME[256] = "./out/LOG.txt"; // place for output

static const Real PI = 3.14159265358979f;
static const Real OVERPI = 0.31830988618379f;
static const Real3 G = {0.0f , 0.0f , -9.18f};

static const Real EPS = 1e-10;
static const Real RMAX = 1e100; // maximum of residual

static const Real EPS_BY_EPS = EPS * EPS;

}
#endif //COMMON_H
