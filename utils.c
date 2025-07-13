#include "utils.h"

void imprimirMintermos(string* mintermos, uint32_t quantidade) {
    printf("Mintermos armazenados:\n");
    for (uint32_t i = 0; i < quantidade; i++) {
        printf("%s\n", mintermos[i]);
    }
}