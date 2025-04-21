#pragma once
#include <chrono>

using clck = std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::seconds;

int min(int a, int b, int c) {
    if (a <= b && a <= c) return a;
    if (b <= a && b <= c) return b;
    return c;
}

int max(int a, int b, int c) {
    if (a >= b && a >= c) return a;
    if (b >= a && b >= c) return b;
    return c;
}

bool is_prime(int p) {
    if (p <= 1) return false;
    for (int k = 2; k*k <= p; ++k) {
        if (p % k == 0) return false;
    }
    return true;
}

int bin_pow(int a, int power, int p) {
    int res = 1;
    int pow = a;
    while (power > 0) {
        if (power & 1) {
            res *= pow;
            res %= p;
        }
        pow *= pow;
        pow %= p;
        power <<= 1;
    }
    return res;
}

int inverse(int a, int p) {
    return bin_pow(a, p-2, p);
}
