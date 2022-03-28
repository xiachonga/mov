#include<iostream>
#include<chrono>
#include<cmath>
#include"common.h"
#include"init.h"

using namespace std;

#define ptr(i) i * w *cn

template <typename ST>
void compare(ST *src1, ST *src2, ST *dst, int width)
{
    // width = 1080 * 1920 * 3
    for (int x = 0; x < width; x++)
    {
        dst[x] = - (src1[x] >= src2[x]);
    }
}
template<typename T>
void blendLinear(const T *src1, const T *src2, const T *weighgts1, const T *weights2, T *dst, const int range_start, const int range_end, const int src_width, const int src_height, const int cn) {
    int width = src_width * cn;
    for (int y = range_start; y < range_end; ++y) {
        const float * const weights1_row = weights1 + src_width * y;
        const float * const weights2_row = weights2 + src_width * y;
        const T * const src1_row = src1 + y * width;
        const T * const src2_row = src2 + y * width;
        T * const dst_row = dst + y * src_width;
        for (int x = 0; x < width; ++x) {
            int x1 = x / cn;
            float w1 = weights1_row[x1], w2 = weights2_row[x1];
            float den = (w1 + w2 + 1e-5f);
            float num = (src1_row[x] * w1 + src2_row[x] * w2);
            dst_row[x] = (T)(num / den);
            //dst_row[x] = (T)(num);
        }
    }
}

void sobel(uchar *image, uchar *gradY, uchar *gradX, int height, int width) {
    double Y, X;
    for (int i = 1; i < height - 1; ++i) {
        for (int j = 1; j < width - 1; ++j) {
          Y = (double)((image + ptr(i - 1))[j - 1] + 2 * (image + ptr(i - 1))[j] + (image + ptr(i - 1))[j + 1] - (image + ptr(i + 1))[j - 1] - 2 * (image + ptr(i + 1))[j] - (image + ptr(i + 1))[j + 1]);
          X = (double)((image + ptr(i - 1))[j + 1] + 2 * (image + ptr(i))[j + 1] + (image + ptr(i + 1))[j + 1] - (image + ptr(i - 1))[j - 1] - 2 * (image + ptr(i))[j - 1] - (image + ptr(i + 1))[j - 1]);
          (gradY + ptr(i))[j] = (uchar)Y;
          (gradX + ptr(i))[j] = (uchar)X; 
        }
    }   

}
extern void dummy(float *dst, float result);

extern void dummy(uchar *dst, uchar *dst1, float result);

int main() {
    
    Point2f points;
    //init(usrc, hwc, 2, 11);    
    init(src1, hwc, 1, 0.03);
    init(src2, hwc, 2, 0.05);
    init(weights1, weights2, hw);
    init(points, count);
    init(weights, count);
  //  usleep(1);
   // float x = 3.0;
   // result = (5.1 + x) * 2;   
    auto start1 = std::chrono::system_clock::now();
    blendLinear<float>(src1, src2, weights1, weights2, dst, range_start, range_end, w, h, cn);
    auto end1 = std::chrono::system_clock::now();
    cout << "blend:" <<std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1).count() <<endl; 
    auto start2 = std::chrono::system_clock::now();
    compare<float>(src1, src2, dst, hwc);
    auto end2 = std::chrono::system_clock::now();
    cout << "compare:" <<std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2).count() <<endl;
    dummy(dst, result);
    return 0;
}
