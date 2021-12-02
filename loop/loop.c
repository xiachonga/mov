#include <stdio.h>
#include <arm_acle.h>

int LEN = 32000;
uint64_t VL = 8;

void init (int *a, int *b) {
    for (int i = 0; i < LEN; ++i) {
        a[i] = i;
        b[i] = i;
    }
}

void check (int *a, int *b) {
    for (int i = 0; i < LEN; ++i) {
        if (a[i] != 2 * i) {
           printf("!!!False!!!\n");
           return;
        }
    }
    printf("!!!True!!!\n");
}

int main() {
    int a[LEN], b[LEN];
    init(a, b);
    __arm_wsr64("1:2:3:4:5", VL);
    uint64_t temp = __arm_rsr64("1:2:3:4:5");
    if (temp != 8) {
       printf("The value of the vector length register was not successfully set!\n");
       return 0;
    }
    for (int i = 0; i < LEN; ++i) {
        a[i] += b[i];
    }
    check(a, b);
    return 0;
}
