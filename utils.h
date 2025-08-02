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

//novo struct para organizar
typedef struct fila {
    implicante* inicio;
    implicante* fim;
} fila;

typedef struct grupo {
    fila* filaImplicantes;
    uint32_t qtdUns;
} grupo;


/*----------------------PROTÓTIPOS DE FUNÇÕES----------------------*/

/*-----------IMPRESSÃO-----------*/

void imprimirFila(fila* f);

void imprimirGrupos(grupo* vetorGrupos, uint32_t tamanho);

void imprimirTabela(uint8_t** tabela, fila* primos, fila* mintermos, uint32_t qtdPrimos, uint32_t qtdMintermos);


/*-----------LÓGICA-----------*/

//conta quantos 1s tem em uma expressão (oara fazer os agrupamentos por número de 1s)
uint32_t contarUns(string linha);


//compara duas expressões e retorna NULL se elas diferem em mais de 1 bit
//e retorna a nova expressão se elas diferem em apenas 1 bit
string compararStrings(string str1, string str2, uint32_t tamanho);


//verifica se uma string existe em uma lista de implicantes
bool existe(implicante* lista, string elemento);

//verifica se uma string existe em vetor de termos cobertos
bool existe_no_vetor(string* vetor, uint32_t tamanhoVetor, string elemento);

/*-----------INSERÇÃO-----------*/

//adiciona um elemento ao vetor de termos cobertos de um implicante (por enquanto ela só é utilizada pra isso na main)
void addVetorStr(string** vetor, string novaString, uint32_t* tamanhoVetor);

//adiciona um implicante ao final de uma fila de implicantes
void add_na_fila(fila** implicantes, implicante* novoImplicante);

/*-----------CRIAÇÃO-----------*/

implicante* copiar_implicante(implicante* original);



void liberar_implicante(implicante* imp);

#endif
