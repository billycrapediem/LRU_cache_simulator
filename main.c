#include <stdio.h>
#include "MRU/simulator.h"
void TestCyclicAccess(int reapeatTime){
    sim_start(6,3,11);
    long long finalVal = 1000000;
    long long traverseTime = reapeatTime * 2;
    for(long long j = 0; j < traverseTime; j ++){
        for(long long i = 0; i < finalVal; i ++){
            sim_access(i);
        }
    }
    int cnt = sim_finish();
    printf("cyclic test: %d\n", cnt);
}

void matrix_multiply(long long a, long long b, long long c, long long n, int B, int S, int W) {
    sim_start(B, S, W);
    for (long long i = 0; i < n; i++) {
        for (long long j = 0; j < n; j++) {
            for (long long k = 0; k < n; k++) {
                // Access A[i * n + k]
                sim_access(a + (i * n + k) *8); // Adjusted to sizeof(int64_t) assuming 64-bit integers
                // Access B[k * n + j]
                sim_access(b + (k * n + j) * 8);
                // Access C[i * n + j] twice
                sim_access(c + (i * n + j) * 8);
                sim_access(c + (i * n + j) * 8);
            }
        }

    }
    int result = sim_finish();
    printf(" actual: %d",result);
}

void TestSawtoothAccess(int reapeatTime){
    sim_start(6,3,11);
    long long finalVal = 1000000 * 4;
    long long traverseTime = reapeatTime;
    for(long long j = 0; j < traverseTime; j ++){
        for(long long i = 0; i < finalVal; i +=4){
            sim_access(i);
        }
        for(long long i = finalVal - 1; i >=0; i -=4){
            sim_access(i);
        }
    }
    int cnt = sim_finish();
    printf("sawtooth result: %d,", cnt);
}



int main(void) {
    //int n = 100;
    TestCyclicAccess(10);
    //TestSawtoothAccess(10);
    //matrix_multiply(0, 8 * n * n, 16 * n * n, n, 6, 3, 4);

}
