//
// Created by Bryan Zhang on 3/18/24.
//

#ifndef CACHE_SIMULATOR_SIMULATOR_H
#define CACHE_SIMULATOR_SIMULATOR_H
typedef struct{
    struct Node* prev;
    struct Node* next;
    long long pos;
} Node;
typedef struct{
    long long tag;
    int valid;
    Node* pos;
} cacheLine;

typedef struct LRUCache {
    Node* head;
    Node* tail;
} LRUCache;

typedef  struct {
    cacheLine** lines;
    LRUCache * LRU;
} cacheSet;

typedef struct{
    cacheSet** sets;
    int B;
    int S;
    int W;
    long long indexBit;
} cache;
LRUCache* createCache(void);

void sim_start(int B, int S, int W);
void sim_access(long long acc);
int sim_finish(void);
#endif //CACHE_SIMULATOR_SIMULATOR_H
