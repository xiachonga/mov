#include <iostream>
#include <cstdio>
#include <sys/times.h>

const int width = 256;
const int height = 256;
const int cn = 3;
const int TG22 = 13573;
short dx[256][256];
short dy[256][256];
short _dx[256], _dy[256], _dx_n[256], _dy_n[256];
int _mag_a[256];
int _mag_n[256 *3];
int low, high;

//test4含有这两个宏，应该无法向量化
#define CANNY_PUSH(map, stack) *map = 2, stack.push_back(map)
#define CANNY_CHECK(m, high, map, stack) \
    if (m > high) \
        CANNY_PUSH(map, stack); \
    else \
        *map = 0
//计算Sobel横纵梯度的norm，即边缘响应的强度
//方法一
void test1() {
    clock_t start_t, end_t, clock_diff;
    start_t = clock();
    for (int i = 0;  i < height; ++i) {
        short *_dx = dx[i];
        short *_dy = dy[i];
        for (int j = 0; j < width * cn; ++j) {
            _mag_n[j] = int(_dx[j]) * _dx[j] + int(_dy[j])*_dy[j];
        }
    }
    end_t = clock();
    clock_diff = start_t - end_t;
    printf("test1\t %.2f\t\n", (double)clock_diff);
}
//方法二
void test2() {
    clock_t start_t, end_t, clock_diff;
    start_t = clock();
    for (int i = 0;  i < height; ++i) {
        short *_dx = dx[i];
        short *_dy = dy[i];
        for (int j = 0; j < width * cn; ++j) {
            _mag_n[j] = std::abs(int(_dx[j])) + std::abs(int(_dy[j]));
        }
    }
    end_t = clock();
    clock_diff = start_t - end_t;
    printf("test2\t %.2f\t\n", (double)clock_diff);
}
void test3() {
    clock_t start_t, end_t, clock_diff;
    start_t = clock();
    for (int i = 0; i < height; ++i) {
        for (int j = 0, jn = 0; j < width; ++j, jn +=cn) {
            int maxIdx = jn;
            for (int k = 1; k < cn; ++k) {
                if (_mag_n[jn + k] > _mag_n[maxIdx]) {
                    maxIdx = jn +k;
                }
            }
            _mag_n[j] = _mag_n[maxIdx];
            _dx_n[j] = dx[i][maxIdx];
            _dy_n[j] =dx[i][maxIdx];
        }
    }
    end_t = clock();
    clock_diff = start_t - end_t;
    printf("test2\t %.2f\t\n", (double)clock_diff);
}

/*
void test4() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int m = _mag_a[j];
            if (m > low) {
                short xs = _dx[j];
                short ys = _dy[j];
                int x = (int)std::abs(xs);
                int y = (int)std::abs(ys) << 15;
                int tg22x = x * TG22;
                if (y < tg22x) {
                    
                } else {

                }
            }
        }
    }
}*/
void init() {
    for (int i = 0; i < width; ++i) {
        for (int j =0; j < height; ++j) {
            dx[i][j] = 1;
            dy[i][j] = 2;
        }
    }
}
int main() {
    init();
    test1();
    test2();
    test3();
    return 0;
}
