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
    uint32_t numEntradas, numSaidas, qtdMintermos = 0;
    string* mintermos = NULL; //vetor de string com as linhas em que a saida é 1

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
                
                if(strcmp(saida,"1")==0){ //quando a saida da linha é 1
                    string* temp = realloc(mintermos, sizeof(string) * (qtdMintermos + 1));
                    
                    if (temp == NULL) {//se houver falha na alocaçao temporaria
                        free(entrada); 
                        free(saida);    
                        fprintf(stderr, "Erro ao alocar memória.\n");
                        return 1;
                    } else { 
                        mintermos = temp;
                        mintermos[qtdMintermos] = entrada; 
                        qtdMintermos++;
                    
                    }
                } else free(entrada); //libera sempre que a saida for 0 (a entrada não vai ser utilizada)

                free(saida); //libera anyways
            }
        }    

        /*
        Cria um vetor de grupos para armazenar os implicantes. 
        Como o máximo de 1s que uma linha pode ter é quando todas as variáveis são 1, vou usar a quantidade de varíaveis para alocar o vetor.
        Não sei se é o ideal, já que pode ficar muito espaço sem utilizar, mas vou fazer assim de início.
        */
        grupo* implicantes = calloc(numEntradas+1,sizeof(grupo)); //implicantes é um vetor de grupos com n+1 espaços (n sendo o numero de variáveis)
        uint32_t qtdGrupos = 0; //vai ser passado por referência na função de agrupar, que vai atualizar toda vez que adicionar um novo grupo

        agrupar(mintermos,qtdMintermos,implicantes,&qtdGrupos);



        //bateria de testes
        imprimirMintermos(mintermos, qtdMintermos);
        printf("\n");
        imprimirGrupos(implicantes,qtdGrupos);


        //LIBERAR MEMORIA ALOCADA
        for (uint32_t j = 0; j < qtdMintermos; j++) {
             free(mintermos[j]);
        }
        free(mintermos);

        fclose(input);

    return 0;
}
