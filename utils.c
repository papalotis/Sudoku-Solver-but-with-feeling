#include "utils.h"

void array_print(int* a, int size) {
    printf("Array %p: [", a);

    for (int i = 0; i < size; i++) {
        printf("%d", a[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

float getTime() {
    return (float)clock()/CLOCKS_PER_SEC;
}