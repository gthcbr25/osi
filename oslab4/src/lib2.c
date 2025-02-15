#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int PrimeCount(int A, int B) {
    if (B < 2) return 0;
    bool *is_prime = (bool *)malloc((B + 1) * sizeof(bool));
    for (int i = 0; i <= B; i++) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= B; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= B; j += i) {
                is_prime[j] = false;
            }
        }
    }
    int count = 0;
    for (int i=A; i<=B; ++i){
        if (is_prime[i]) count++;
    }
    free(is_prime);
    return count;
}

int GCF(int a, int b){
    int minimal = 0;
    if (abs(a) < abs(b)){
        minimal = abs(a);
    }else{
        minimal = abs(b);
    }
    for (int i = minimal; i > 0; i--){
        if (a % i == 0 && b % i == 0){
            return i;
        }
    }
    return 1;
}