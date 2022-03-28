#define __COMMON__

#include<cstdlib>
//#include<unistd.h>
typedef unsigned char uchar;

const int h = 256;
const int w = 256;
const int cn = 1;
const int hw = h * w;
const int hwc = h * w * cn;
const int range_start = 0;
const int range_end = h;
const int beta = 20;
const float alpha = 0.7;
float result = 0;
int count = 141120;

uchar* usrc = (uchar *)aligned_alloc(32, hwc);
uchar* udst = (uchar *)aligned_alloc(32, hwc);
//uchar* udst1 = (uchar *)aligned_alloc(32, hwc);

float* src1 = (float *)aligned_alloc(32, hwc * sizeof(float));
float* src2 = (float *)aligned_alloc(32, hwc * sizeof(float));
float* weights1 = (float *)aligned_alloc(32, hw * sizeof(float));
float* weights2 = (float *)aligned_alloc(32, hw * sizeof(float));
float* weights = (float *)aligned_alloc(32, count * sizeof(float));
float* dst = (float *)aligned_alloc(32, hwc * sizeof(float));
//float* dst1 = (float *)aligned_alloc(32, hwc * sizeof(float));

#endif

typedef unsigned char uchar;
typedef struct Point2f {
    float x[141120];
    float y[141120];
}Point2f;

void init(uchar *a, int len, int step, uchar base);

void init(float *x, int len, float step, float base);

void init(float *weights1, float *weights2, int len);

void init(Point2f &points, int count);

void init(float *weights, int count);

#include "init.h"
#include <cmath>

void init(uchar *a, int len, int step, uchar base)
{
    for (int i = 0; i < len; i += 16)
    {
        for (int j = 0; j < 15; j++)
            a[i + j] = (uchar)(((i + j) * step + base) & 255);
        a[i + 15] = (uchar)((-(i * step + base)) & 255);
    }
}

void init(float *x, int len, float step, float base)
{
    for (int i = 0; i < len; i++)
    {
        x[i] = cos(step * i) + base;
    }
}
void init(float *weights1, float *weights2, int len) {
    for (int i = 0; i < len; i++)
    {
        weights1[i] = 0.6;
        weights2[i] = 0.4;
    }

}

void init(Point2f &points, int count) {
    for (int i = 0; i < count; ++i) {
        points.x[i] = i;
        points.y[i] = i + 1;
    }
}
void init(float *weights, int count) {
    for (int i = 0; i < count; ++i) {
        weights[i] = 0.4 + i / (i + 1);
    }
}

typedef unsigned char uchar;
void dummy(float *dst, float result) {}
void dummy(float *dst, float *dst1, float result) {}
void dummy(uchar *dst, uchar *dst1, float result) {}
void remap(short *XY) {}

~/remote/llvm/llvm-project/build-13/bin/clang++ -O0 -c init.cpp -o init.o --target=aarch64-linux-gnu -march=armv8.2-a+sve -static
~/remote/llvm/llvm-project/build-13/bin/clang++ -O0 -c dummy.cpp -o dummy.o --target=aarch64-linux-gnu -march=armv8.2-a+sve -static
~/remote/llvm/llvm-project/build-13/bin/clang++ -O3 compare.cpp dummy.o init.o -o compare.out -mllvm -scalable-vectorization=preferred -Rpass=loop -Rpass-analysis=loop --target=aarch64-linux-gnu -march=armv8.2-a+sve -static
~/remote/llvm/llvm-project/build-13/bin/clang++ -O3 compare.cpp dummy.o init.o -o compare_scalar.out -fno-tree-vectorize  --target=aarch64-linux-gnu -march=armv8.2-a+sve -static








