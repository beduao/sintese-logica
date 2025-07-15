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

typedef struct implicante {
    string expressao;
    string* termosCobertos;
} implicante;

//PROTÓTIPOS DE FUNÇÕES
void imprimirStrings(string* mintermos, uint32_t quantidade); 

uint32_t contarUns(string linha);

void agrupar(string* vetorMintermos, uint32_t qtdMintermos, grupo* vetorImplicantes, uint32_t* qtdGrupos);

void imprimirGrupos(grupo* vetorImplicantes, uint32_t qtdGrupos);

string compararStrings(string str1, string str2, uint32_t tamanho);

uint8_t existe(string elemento, implicante* vetorImplicantes, uint32_t tamanhoVetor);

void adicionarImplicante(implicante** vetor, uint32_t* qtd, string nova, string termo1, string termo2);

void compararGrupos(implicante** vetorImplicantes, uint32_t *qtdImplicantes, grupo* vetorGrupos, uint32_t qtdGrupos,  uint32_t qtdVariaveis);

void imprimirImplicantes(implicante* vetorImplicantes, uint32_t qtdImplicantes);

#endif