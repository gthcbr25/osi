#include <stdio.h>
#include <stdlib.h>

extern int PrimeCount(int A, int B);
extern int GCF(int A, int B);

int main() {
    int choice;
    printf("1: Count primes\n2: GCF\nChoose option: ");
    scanf("%d", &choice);

    int A, B;
    printf("Enter A and B: ");
    scanf("%d %d", &A, &B);

    if (choice == 1) {
        printf("Prime count: %d\n", PrimeCount(A, B));
    } else if (choice == 2) {
        printf("GCF: %d\n", GCF(A, B));
    }

    return 0;
}