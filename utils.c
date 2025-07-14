#include "utils.h"

void imprimirMintermos(string* mintermos, uint32_t quantidade) { //função auxiliar para testes
    printf("Mintermos armazenados:\n");
    for (uint32_t i = 0; i < quantidade; i++) {
        printf("%s\n", mintermos[i]);
    }
}

uint32_t contarUns(string linha) { // conta quantos 1s tem em uma linha 
    uint32_t contagem = 0;
    while(*linha!='\0'){
        if (*linha=='1') {
            contagem++;
        }
        linha++;
    }
    return contagem;
}

void agrupar(char** vetorMintermos, uint32_t qtdMintermos, grupo* vetorImplicantes, uint32_t* qtdGrupos) {

    for (uint32_t i = 0; i < qtdMintermos; i++) { //percorre todo o vetor de mintermos

        uint32_t numUns = contarUns(vetorMintermos[i]);
        int grupoEncontrado = 0;

        // buscar grupo existente com mesmo numero de 1s
        for (uint32_t j = 0; j < *qtdGrupos; j++) { //percorre todo o vetor dos grupos

            //se achar um grupo que já existe
            if (vetorImplicantes[j].qtdUns == numUns) { 
                // adiciona a string ao grupo existente
                uint32_t index = vetorImplicantes[j].stringsArmazenadas; //espaço livre no vetor do grupo selecionado
                vetorImplicantes[j].vetorStrs[index] = strdup(vetorMintermos[i]);

                vetorImplicantes[j].stringsArmazenadas++;
                grupoEncontrado = 1;
                break;
            }
        }

        // se não achou, cria um novo grupo
        if (!grupoEncontrado) {
            grupo novoGrupo;
            novoGrupo.qtdUns = numUns;
            novoGrupo.stringsArmazenadas = 1;

            novoGrupo.vetorStrs = malloc(sizeof(char*) * qtdMintermos); // alocação máxima possível, para o pior caso em que todos os minteor tem a mesma quantidade de uns
            novoGrupo.vetorStrs[0] = strdup(vetorMintermos[i]);

            vetorImplicantes[*qtdGrupos] = novoGrupo;
            (*qtdGrupos)++;
        }
    }
}


void imprimirGrupos(grupo* vetorImplicantes, uint32_t qtdGrupos) {
    for (uint32_t i = 0; i < qtdGrupos; i++) {
        printf("Grupo com %u uns:\n", vetorImplicantes[i].qtdUns);
        for (uint32_t j = 0; j < vetorImplicantes[i].stringsArmazenadas; j++) {
            printf("-%s\n", vetorImplicantes[i].vetorStrs[j]);
        }
    }
}