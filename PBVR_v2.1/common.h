/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Published under CC BY-NC
*/
//common.h
//file to save common parameters
#ifndef COMMON_H
#define COMMON_H

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

///rendering
#define REPEAT_LEVEL    121

namespace Common
{

static const char LOG_NAME[256] = "./out/LOG.txt"; // place for output

static const float PI = 3.14159265358979f;
static const float OVERPI = 0.31830988618379f;
static const float EPS = 1e-6;

}
#endif //COMMON_H
