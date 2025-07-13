#include "utils.h"

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
    uint32_t numEntradas, numSaidas, i = 0;
    string* mintermos = NULL;

    while(fgets(buffer, sizeof(buffer), input)){ //lê cada linhda do .pla

        buffer[strcspn(buffer,"\n")] = 0; //remove o /n
        if(buffer[0]=='\0') continue; //ignora linhas vazias

        if(buffer[0] == '.') { //adicionar .type e .ilb / .ob (se precisar)
            if (buffer[1] == 'i') sscanf(buffer,".i %u",&numEntradas);
            if (buffer[1] == 'o') sscanf(buffer,".o %u",&numSaidas);
            if (buffer[1] == 'e') break;
        }
        else {
                string entrada = malloc((numEntradas + 1)*sizeof(char));
                string saida = malloc((numSaidas + 1)*sizeof(char)); 
                sscanf(buffer, "%s %s", entrada, saida);
                
                if(strcmp(saida,"1")==0){
                    string* temp = realloc(mintermos, sizeof(string) * (i + 1));
                    if (temp != NULL) {
                        mintermos = temp;
                        mintermos[i] = entrada; 
                        i++;
                    } else {
                        free(entrada); 
                        free(saida);    
                        fprintf(stderr, "Erro ao alocar memória.\n");
                        return 1;
                    }
                } else free(entrada); //libera sempre que a saida for 0 (a entrada não vai ser utilizada)

                free(saida); //libera anyways
            }
        }    

        imprimirMintermos(mintermos, i); //teste para vez se tá armazenando legal

        //LIBERAR MEMORIA ALOCADA
        for (uint32_t j = 0; j < i; j++) {
             free(mintermos[j]);
        }
        free(mintermos);

        fclose(input);
    return 0;
}
