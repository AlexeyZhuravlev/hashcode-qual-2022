#pragma once

#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <unordered_map>
#include <random>

using namespace std;

#ifndef SEED
#define SEED 1337
#endif

struct Context {
    using TSolution = int;
    TSolution Solution;

    std::mt19937_64 rng;

    Context() {
        rng.seed(SEED);
    }

    void Input() {
        
    }

    void Output() {
        
    }

    uint64_t GetScore() {
        return 0;
    }
    
};
