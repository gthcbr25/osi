// dynamic.c: Динамическая загрузка библиотек
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef int (*PrimeCountFunc)(int, int);
typedef int (*GcfFunc)(int, int);

int main() {
    void *lib1_handle;
    void *lib2_handle;
    PrimeCountFunc PrimeCount;
    GcfFunc GCF;
    char *error;

    lib1_handle = dlopen("./lib1.so", RTLD_LAZY);
    lib2_handle = dlopen("./lib2.so", RTLD_LAZY);
    if (!lib1_handle || !lib2_handle) {
        fprintf(stderr, "Error loading library: %s\n", dlerror());
        exit(1);
    }

    void *lib_handle;
    int var;
    scanf("%d", &var);
    if (var == 1) lib_handle = lib1_handle;
    else lib_handle = lib2_handle;

    PrimeCount = (PrimeCountFunc)dlsym(lib_handle, "PrimeCount");
    GCF = (GcfFunc)dlsym(lib_handle, "GCF");

    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "Error loading symbols: %s\n", error);
        exit(1);
    }

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

    dlclose(lib_handle);
    return 0;
}