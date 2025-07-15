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
        Cria um vetor de grupos para armazenar as saídas por número de 1s
        Atualizei para criar x grupos baseados em quantos mintermos tem, faz mais sentido assim. No pior caso, cada mintermo vei ter seu prórpio grupo.
        */
        grupo* grupos = calloc(qtdMintermos,sizeof(grupo)); //vetor com os grupos
        uint32_t qtdGrupos = 0; //vai ser passado por referência na função de agrupar, que vai atualizar toda vez que adicionar um novo grupo
        agrupar(mintermos,qtdMintermos,grupos,&qtdGrupos); //gera o vetor de grupos

        /*
        Cria um vetor para armazenar os implicantes iniciais da expressão
        A partir daqui, a saída dexa de ser um vetor de grupos, e passa a ser um vetor de implicantes (não são os implicantes primos ainda)
        */
        implicante* vetorImplicantes = malloc(sizeof(string)); //vetor de implicantes
        uint32_t qtdImplicantes = 0; //vai ser atualizado pela função compararGrupos a medida que ela cria um novo implicante
        compararGrupos(&vetorImplicantes,&qtdImplicantes,grupos,qtdGrupos,numEntradas); //gera o vetor de implicantes

        /*
        Seguindo a mesma lógica das outras funções, cria um vetor para armazenar os implicantes primos
        chama a função que executa os mesmos dois passos em loop, até chegar no final
        */
        //implicante* implicantesPrimos = NULL; //vetor com os implicants primos FINAIS
        //uint32_t qtdPrimos = 0;

        /* ----PROXIMOS PASSOS----
        começar a etapa da tabela
        */

        //BATERIA DE TESTES
        printf("Linhas com saida 1:\n");
        printf("-----------------\n");
        imprimirStrings(mintermos, qtdMintermos);
        printf("-----------------\n");

 
        imprimirGrupos(grupos,qtdGrupos);
        printf("-----------------\n");


        imprimirImplicantes(vetorImplicantes,qtdImplicantes);




        //LIBERAR MEMORIA ALOCADA
        for (uint32_t j = 0; j < qtdMintermos; j++) {
             free(mintermos[j]);
        }
        free(mintermos);

        fclose(input);

    return 0;
}
