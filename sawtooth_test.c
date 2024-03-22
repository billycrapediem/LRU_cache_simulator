#include "simulator_MRU.c"
#include <stdio.h>

void main(int reapeatTime){
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
    printf("result: %d", cnt);
}



