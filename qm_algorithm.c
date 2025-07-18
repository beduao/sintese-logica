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

    /* ----------------------CRIAÇÃO DOS IMPLICANTES INICIAIS----------------------
        Separa as linhas em que as saídas são 1 em uma lista inicial
        Para cada nova saída 1, adiciona um novo Implicante, onde:
        - EXPRESSAO é a linha das entradas que fazem a saída.
        - COMBINADO diz se essa expressao resulta de uma combinação de outras duas expressões ou não.
            Como são os implicantes inciais, ele é inicializado como false!
        - TERMOS COBERTOS é um vetor com os termos que a expressão do implicante cobre.
            Como são os implicantes inciais, o termo cobertos é a prórpia expressão do implicante!
        - QTD TERMOS COBERTOS é o tamanho desse vetor.
        - PROXIMO é um ponteiro para o próximo implicante da lista.
        ---------------------------------------------------------------------------
    */

    char buffer[256];
    uint32_t numEntradas, numSaidas;
    implicanteorg* implicantesIniciais = NULL; //lista de implicantes com as linhas em que a saida é 1 (alterado para para o tempo o(1)


    while(fgets(buffer, sizeof(buffer), input)){ //lê cada linhda do .pla

        buffer[strcspn(buffer,"\n")] = 0; //remove o '/n'
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
                    implicante* novoTermo = malloc(sizeof(implicante));
                    if(novoTermo==NULL){
                        perror("Erro fatal ao alocar implicantes iniciais. Programa finalizado\n");
                        return 1;
                    }

                    novoTermo->combinado = false;
                    novoTermo->proximo = NULL;
                    novoTermo->termosCobertos = NULL;
                    novoTermo->qtdTermosCobertos = 0;

                    novoTermo->expressao = strdup(entrada);
                    addVetorStr(&(novoTermo->termosCobertos), //passagem por referência, pois modifica o vetor do implicante
                                novoTermo->expressao,
                                &(novoTermo->qtdTermosCobertos)); //passagem por referência, pois aumenta a quantidade de termos em um
                    addListaImplicante(&(implicantesIniciais),novoTermo);

                } else free(entrada); //libera sempre que a saida for 0 (a entrada não vai ser utilizada)

                free(saida); //libera anyways
            }
        }

        /*----------------------FIM DA CRIAÇÃO DOS IMPLICANTES INICIAIS----------------------*/


        /* ----------------------PROXIMOS PASSOS----------------------
            - Implementar a lógica de agrupamento + comparação
            - Fazer as comparações consectuivas nos implicantes iniciais
            - Gerar os implicantes primos
            - Começar a etapa da tabela
        */

        /*----------------------BATERIA DE TESTES----------------------*/
        imprimirImplicantes(implicantesIniciais);


       /*----------------------LIBERAÇÃO DE MEMÓRIA----------------------*/


        fclose(input);

    return 0;
}
