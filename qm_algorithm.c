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
    fila* implicantesIniciais = NULL; //fila de implicantes com as linhas em que a saida é 1 (alterado para para o tempo de inserção ser o(1)


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
                                &(novoTermo->qtdTermosCobertos)); //passagem por referência, pois aumenta a quantidade de termos cobertos em um

                    add_na_fila(&(implicantesIniciais),novoTermo); //adiciona o novo implicante alocado na fila

                } else free(entrada); //libera sempre que a saida for 0 (a entrada não vai ser utilizada)

                free(saida); //libera anyways
            }
        }

        /*----------------------FIM DA CRIAÇÃO DOS IMPLICANTES INICIAIS----------------------*/

        printf("----------\nFila original:\n");
        imprimirFila(implicantesIniciais); //imprime a fila inicial antes dela ser esvaziada na criação de grupos


        /*----------------------AGRUPAMENTO POR NÚMERO DE UNS----------------------*/
        
        grupo* vetorGrupos = NULL; //vetor de grupos
        uint32_t qtdGrupos = 0; //números de grupos criados

        //percorre todo a fila de implicantes
        while (implicantesIniciais->inicio != NULL) {
            //vai percorrendo a fila e movendo os implicantes para os grupos
            implicante* atual = implicantesIniciais->inicio; //percorre a fila pelo início
            implicantesIniciais->inicio = atual->proximo; //o pirmeiro a entrar é o primeiro a sair

            atual->proximo = NULL; // limpar vínculo anterior

            uint32_t uns = contarUns(atual->expressao);

            bool grupoEncontrado = false;

            for (uint32_t i = 0; i < qtdGrupos; i++) {
                if (vetorGrupos[i].qtdUns == uns) {
                    add_na_fila(&(vetorGrupos[i].filaImplicantes), atual);
                    grupoEncontrado = true;
                    break;
                }
            }

            if (!grupoEncontrado) {
                vetorGrupos = realloc(vetorGrupos, sizeof(grupo) * (qtdGrupos + 1));
                vetorGrupos[qtdGrupos].qtdUns = uns;
                vetorGrupos[qtdGrupos].filaImplicantes = NULL;
                add_na_fila(&(vetorGrupos[qtdGrupos].filaImplicantes), atual);
                qtdGrupos++;
            }
        }

        printf("-----------\n\n");
        imprimirGrupos(vetorGrupos,qtdGrupos);

        /* ----------------------PROXIMOS PASSOS----------------------
            - Implementar a lógica de agrupamento + comparação
            - Fazer as comparações consectuivas nos implicantes iniciais
            - Gerar os implicantes primos
            - Começar a etapa da tabela
        */

       /*----------------------LIBERAÇÃO DE MEMÓRIA----------------------*/


        fclose(input);

    return 0;
}
