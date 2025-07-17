#ifndef UTILS_H
#define UTILS_H

/*----------------------BIBLIOTECAS UTILIZADAS----------------------*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*----------------------ESTRUTURAS UTILIZADAS----------------------*/

typedef char* string;

typedef struct implicante {
    string expressao;
    bool combinado;
    string* termosCobertos; 
    uint32_t qtdTermosCobertos;
    struct implicante* proximo;
} implicante;

typedef struct grupo {
    implicante* listaImplicantes;
    uint32_t qtdUns;
} grupo;


/*----------------------PROTÓTIPOS DE FUNÇÕES----------------------*/

/*-----------IMPRESSÃO-----------*/
void imprimirImplicantes(implicante* lista);

/*-----------LÓGICA-----------*/
uint32_t contarUns(string linha);
//conta quantos 1s tem em uma expressão (oara fazer os agrupamentos por número de 1s)

string compararStrings(string str1, string str2, uint32_t tamanho);
//compara duas expressões e retorna NULL se elas diferem em mais de 1 bit
//e retorna a nova expressão se elas diferem em apenas 1 bit

bool existe(implicante* lista, string elemento);
//verifica se uma string existe em uma lista de implicantes

/*-----------INSERÇÃO-----------*/
void addVetorStr(string** vetor, string novaString, uint32_t* tamanhoVetor);
//adiciona um elemento ao vetor de termos cobertos de um implicante (por enquanto ela só é utilizada pra isso na main)

void addListaImplicante(implicante** lista, implicante* novoImplicante);
//adiciona um implicante a uma lista de implicantes

#endif