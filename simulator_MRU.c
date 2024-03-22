#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint64_t tag;
    bool valid;
    uint64_t last_used;
} CacheLine; // Represents a single line (or slot) within a cache set

typedef struct {
    CacheLine* lines;
} CacheSet;// Represents a set within the cache

typedef struct {
    CacheSet* sets;
    uint64_t access_count;
    uint64_t miss_count;
    uint64_t time;
    int B, S, W;
    unsigned int CACHE_LINES;
    unsigned int SETS;
} Cache;

Cache cache; // Represents the entire cache

unsigned int getSetIndex(uint64_t address) {
    return (address >> cache.B) & (cache.SETS - 1);
}

uint64_t getTag(uint64_t address) {
    return address >> (cache.B + cache.S);
}

int findMRULineIndex(CacheSet *set) {
    uint64_t max_time = 0;
    int mru_index = 0;
    for (unsigned int i = 0; i < cache.CACHE_LINES; i++) {
        if (set->lines[i].last_used > max_time) {
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
    for (unsigned int i = 0; i < cache.SETS; i++) {
        cache.sets[i].lines = (CacheLine*)malloc(cache.CACHE_LINES * sizeof(CacheLine));
        for (unsigned int j = 0; j < cache.CACHE_LINES; j++) {
            cache.sets[i].lines[j].valid = false;
            cache.sets[i].lines[j].tag = 0;
            cache.sets[i].lines[j].last_used = 0;
        }
    }
    cache.access_count = 0;
    cache.miss_count = 0;
    cache.time = 0;
}

void sim_access(long long acc) {
    uint64_t address = (uint64_t)acc;
    cache.access_count++;
    cache.time++;

    unsigned int setIndex = getSetIndex(address);
    uint64_t tag = getTag(address);

    CacheSet *set = &cache.sets[setIndex];

    for (unsigned int i = 0; i < cache.CACHE_LINES; i++) {
        if (set->lines[i].valid && set->lines[i].tag == tag) {
            set->lines[i].last_used = cache.time;
            return; // Hit
        }
    }

    // Miss
    cache.miss_count++;
    int replaceIndex = findMRULineIndex(set);
    set->lines[replaceIndex].valid = true;
    set->lines[replaceIndex].tag = tag;
    set->lines[replaceIndex].last_used = cache.time;
}

int sim_finish(void) {
    int misses = cache.miss_count;
    for (unsigned int i = 0; i < cache.SETS; i++) {
        free(cache.sets[i].lines);
    }
    free(cache.sets);
    return misses;
}
