#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef char* string;

int main(int argc, char const *argv[])
{
    if(argc != 2) {
        printf("Erro ao processar o arquivo");
        return 1;
    }

    FILE* input = fopen(argv[1],"r");
    if (input == NULL) {
        printf("Erro ao abrir o arquivo selecionado");
        return 1;
    }

    char buffer[256];
    uint32_t numEntradas, numSaidas;

    while(fgets(buffer, sizeof(buffer), input)){ //lê cada linhda do .pla

        buffer[strcspn(buffer,"\n")] = 0; //remove o /n
        if(buffer[0]=='\0') continue; //ignora linhas vazias

        if(buffer[0] == '.') {
            if (buffer[1] == 'i') sscanf(buffer,".i %u",&numEntradas);
            if (buffer[1] == 'o') sscanf(buffer,".o %u",&numSaidas);
            if (buffer[1] == 'e') break;
        }
        else {
            string entrada = malloc(numEntradas*sizeof(char));
            string saida = malloc(numSaidas*sizeof(char)); 
            sscanf(buffer, "%s %s", entrada, saida);

            if(strcmp(saida,"0")==1){ 
                //ignora os mintermos que são 0
                //modificar para adicionar as entradas em um vetor
                printf("%s %s\n",entrada, saida);
            }
        }
    }    

    return 0;
}
