//
// Created by Bryan Zhang on 3/21/24.
//

#ifndef CACHE_SIMULATOR_MRUSPLAY_H
#define CACHE_SIMULATOR_MRUSPLAY_H
typedef struct{
    struct Node* prev;
    struct Node* next;
    long long tag;
} Node;

typedef struct LRUCache {
    Node* head;
    Node* tail;
    long long capacity;
    long long size;
} LRUCache;

typedef struct{
    LRUCache** sets;
    int B;
    int S;
    int W;
    long long indexBit;
} cache;
LRUCache* createCache(long long capacity);

void splay_sim_start(int B, int S, int W);
int splay_sim_access(long long acc);
int splay_sim_finish(void);
#endif //CACHE_SIMULATOR_MRUSPLAY_H
