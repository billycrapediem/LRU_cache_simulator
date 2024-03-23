#include <stdlib.h>
#include "simulator.h"
#include "stdio.h"
typedef struct {
    long long tag;
    int valid;
    long long last_used;
} CacheLine; // Represents a single line (or slot) within a cache set

typedef struct {
    CacheLine* lines;
    long long size;
} CacheSet;// Represents a set within the cache

typedef struct {
    CacheSet* sets;
    long long access_count;
    int miss_count;
    long long time;
    int B, S, W;
    long long CACHE_LINES;
    long long SETS;
} Cache;

Cache cache; // Represents the entire cache

long long getSetIndex(long long address) {
    return (address >> cache.B) & (cache.SETS - 1);
}

long long getTag(long long address) {
    return address >> (cache.B + cache.S);
}

long long findMRULineIndex(CacheSet *set) {
    long long max_time = 0;
    long long mru_index = -1;
    if(cache.CACHE_LINES > set->size){
        set->size ++;
        for(long long i = 0; i < cache.CACHE_LINES; i ++){
            if(!set->lines[i].valid){
                return i;
            }
        }
    }
    for (long long i = 0; i < cache.CACHE_LINES; i++) {
        if (set->lines[i].last_used > max_time ) {
            max_time = set->lines[i].last_used;
            mru_index = i;
        }
    }
    return mru_index;
}

void sim_start(int B, int S, int W) {
    cache.B = B;
    cache.S = S;
    cache.W = W;
    cache.CACHE_LINES = (1 << W);
    cache.SETS = (1 << S);

    cache.sets = (CacheSet*)malloc(cache.SETS * sizeof(CacheSet));
    for (long long int i = 0; i < cache.SETS; i++) {
        cache.sets[i].lines = (CacheLine*)malloc(cache.CACHE_LINES * sizeof(CacheLine));
        for (long long int j = 0; j < cache.CACHE_LINES; j++) {
            cache.sets[i].lines[j].valid = 0;
            cache.sets[i].lines[j].tag = 0;
            cache.sets[i].lines[j].last_used = 0;
        }
        cache.sets[i].size = 0;
    }
    cache.access_count = 0;
    cache.miss_count = 0;
    cache.time = 0;
}

void sim_access(long long acc) {
    long long address = acc;
    cache.access_count++;
    cache.time++;

    long long int setIndex = getSetIndex(address);
    long long tag = getTag(address);

    CacheSet *set = &cache.sets[setIndex];

    for (unsigned int i = 0; i < cache.CACHE_LINES; i++) {
        if (set->lines[i].valid && set->lines[i].tag == tag) {
            set->lines[i].last_used = cache.time;
            return; // Hit
        }
    }

    // Miss
    cache.miss_count++;
    long long replaceIndex = findMRULineIndex(set);
    set->lines[replaceIndex].valid = 1;
    set->lines[replaceIndex].tag = tag;
    set->lines[replaceIndex].last_used = cache.time;
}

int sim_finish(void) {
    int misses = cache.miss_count;
    cache.miss_count = 0;
    for (long long i = 0; i < cache.SETS; i++) {
        free(cache.sets[i].lines);
    }
    free(cache.sets);
    return misses;
}
