#ifndef UTILS_H
#define UTILS_H

//BIBLIOTECAS UTILIZADAS
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//ESTRUTURAS UTILIZADAS
typedef char* string;

typedef struct grupo {
    string* vetorStrs;
    uint32_t stringsArmazenadas;
    uint32_t qtdUns;
} grupo;

//PROTÓTIPOS DE FUNÇÕES
void imprimirMintermos(string* mintermos, uint32_t quantidade); 

uint32_t contarUns(string linha);

void agrupar(char** vetorMintermos, uint32_t qtdMintermos, grupo* vetorImplicantes, uint32_t* qtdGrupos);

void imprimirGrupos(grupo* vetorImplicantes, uint32_t qtdGrupos);

#endif