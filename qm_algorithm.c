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

    /*----------------------LEITURA DOS IMPLICANTES INICIAIS----------------------*/ 

    char buffer[256];
    uint32_t numEntradas, numSaidas;
    fila* implicantesIniciais = NULL; //fila de implicantes com as linhas em que a saida é 1 (alterado para para o tempo de inserção ser o(1)
    uint32_t qtdImplicantes = 0; //tamanho da fila de implicantes

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
                    qtdImplicantes++;

                } else free(entrada); //libera sempre que a saida for 0 (a entrada não vai ser utilizada)

                free(saida); //libera anyways
            }
        }

        printf("------------------\nImplicantes originais:\n");
        imprimirFila(implicantesIniciais);    
        printf("------------------\n");

        /*----------------------FIM DA LEITURA DOS IMPLICANTES INICIAIS----------------------*/


        /*----------------------AGRUPAMENTO POR NÚMERO DE UNS----------------------*/
        
        grupo* vetorGrupos = NULL; //vetor de grupos
        uint32_t qtdGrupos = 0; //números de grupos criados

        //percorre todo a fila de implicantes
        implicante* atual = implicantesIniciais->inicio;
        while (atual != NULL) {
            implicante* copia = copiar_implicante(atual);
            uint32_t uns = contarUns(copia->expressao);

            bool grupoEncontrado = false;
            for (uint32_t i = 0; i < qtdGrupos; i++) {
                if (vetorGrupos[i].qtdUns == uns) {
                    add_na_fila(&(vetorGrupos[i].filaImplicantes), copia);
                    grupoEncontrado = true;
                    break;
                }
            }

            if (!grupoEncontrado) {
                vetorGrupos = realloc(vetorGrupos, sizeof(grupo) * (qtdGrupos + 1));
                vetorGrupos[qtdGrupos].qtdUns = uns;
                vetorGrupos[qtdGrupos].filaImplicantes = NULL;
                add_na_fila(&(vetorGrupos[qtdGrupos].filaImplicantes), copia);
                qtdGrupos++;
            }

            atual = atual->proximo;
        }

        imprimirGrupos(vetorGrupos,qtdGrupos);

        /*----------------------FIM DO AGRUPAMENTO INICIAL----------------------*/

        
        /*----------------------LOOP DE COMBINAÇÃO E REAGRUPAMENTO----------------------*/

        fila* primos = NULL; // onde serão guradados os implicantes primos
        uint32_t qtdPrimos = 0;
        bool houveCombinacao;

        do { //loop infinito que só para quando não são feitas mais combinações entre os grupos

            houveCombinacao = false;

            grupo* novoVetorGrupos = NULL;
            uint32_t novaQtdGrupos = 0;

            for (uint32_t i = 0; i < qtdGrupos - 1; i++) { //percorre todos o grupos iniciais

                fila* filaA = vetorGrupos[i].filaImplicantes; 
                fila* filaB = vetorGrupos[i + 1].filaImplicantes; 

                implicante* impA = filaA->inicio;
                while (impA != NULL) { //percorre todo o primeiro grupo

                    implicante* impB = filaB->inicio;
                    while (impB != NULL) { //percorre todo o segundo grupo

                        string novaExpressao = compararStrings(impA->expressao, impB->expressao, numEntradas);

                        if (novaExpressao != NULL) {
                            houveCombinacao = true;

                            //cria novo implicante
                            implicante* novo = malloc(sizeof(implicante));
                            novo->expressao = novaExpressao;
                            novo->combinado = false;
                            novo->proximo = NULL;
                            novo->qtdTermosCobertos = 0;
                            novo->termosCobertos = NULL;

                            //junta os termos cobertos dos implicantes combinados
                            //DETALHE, não diciona termos duplicados
                            for (uint32_t t = 0; t < impA->qtdTermosCobertos; t++) {
                                addVetorStr(&novo->termosCobertos, impA->termosCobertos[t], &novo->qtdTermosCobertos);
                            } //primeiro adiciona os termos do primeiro implicante

                            for (uint32_t t = 0; t < impB->qtdTermosCobertos; t++) {
                                // CORRIGIDO: usa a nova função para buscar no vetor de strings
                                if (!existe_no_vetor(novo->termosCobertos, novo->qtdTermosCobertos, impB->termosCobertos[t])) {
                                    addVetorStr(&novo->termosCobertos, impB->termosCobertos[t], &novo->qtdTermosCobertos);
                                }
                            } //depois do segundo implicante

                            //seta os implicantes como combinados
                            impA->combinado = true;
                            impB->combinado = true;


                            //REAGRUPA O NOVO TERMO COMBINADO
                            // descobrir número de 1s (desconsiderando os '-')
                            uint32_t uns = contarUns(novo->expressao);

                            bool grupoExiste = false;
                            for (uint32_t g = 0; g < novaQtdGrupos; g++) {
                                if (novoVetorGrupos[g].qtdUns == uns) {
                                    grupoExiste = true; // Encontrou o grupo correto
                                    
                                    // VERIFICA SE O IMPLICANTE JÁ EXISTE NA FILA DESTE GRUPO
                                    if (!existe(novoVetorGrupos[g].filaImplicantes->inicio, novo->expressao)) {
                                        add_na_fila(&(novoVetorGrupos[g].filaImplicantes), novo);
                                    } else {
                                        // e já existe, é uma combinação duplicado.
                                        liberar_implicante(novo);
                                    }
                                    break;
                                }
                            }

                            if (!grupoExiste) {
                                novoVetorGrupos = realloc(novoVetorGrupos, sizeof(grupo) * (novaQtdGrupos + 1));
                                novoVetorGrupos[novaQtdGrupos].qtdUns = uns;
                                novoVetorGrupos[novaQtdGrupos].filaImplicantes = NULL;
                                add_na_fila(&(novoVetorGrupos[novaQtdGrupos].filaImplicantes), novo);
                                novaQtdGrupos++;
                            }
                        }

                        //atualiza o ponteiro auxiliar do segundo grupo
                        impB = impB->proximo;
                    }
                    //atualiza o ponteiro auxiliar do primeiro grupo
                    impA = impA->proximo;
                }
            }

            // Adiciona todos os implicantes não combinados aos primos
            for (uint32_t i = 0; i < qtdGrupos; i++) {
                implicante* aux = vetorGrupos[i].filaImplicantes->inicio;

                while (aux != NULL) {
                    implicante* proximo = aux->proximo;
                    if (!aux->combinado) {
                        add_na_fila(&primos, aux);
                        qtdPrimos++;
                    } else {
                        // se foi combinado, libera a memória agora
                        free(aux->expressao);
                        for (uint32_t t = 0; t < aux->qtdTermosCobertos; t++) {
                            free(aux->termosCobertos[t]);
                        }
                        free(aux->termosCobertos);
                        free(aux);
                    }
                    aux = proximo;
                }
                free(vetorGrupos[i].filaImplicantes);
            }

            free(vetorGrupos);
            vetorGrupos = novoVetorGrupos;
            qtdGrupos = novaQtdGrupos;

        } while (houveCombinacao);

        printf("------------------\nImplicantes Primos:\n");
        imprimirFila(primos);    
        printf("------------------\n");

        /*----------------------FIM DO LOOP DE REAGRUPAMENTO----------------------*/
        

        /*----------------------CRIAÇÃO DA MATRIZ----------------------*/

        //cria a tabela binária PRIMOS (linhas) X IMPLICANTES ORIGINAIS (colunas)
        uint8_t** tabela = malloc(qtdPrimos * sizeof(uint8_t*));
        for (uint32_t i = 0; i < qtdPrimos; i++) {
            tabela[i] = calloc(qtdImplicantes, sizeof(uint8_t)); // tudo começa como 0
        }

        implicante* linha = primos->inicio;
        for (uint32_t i = 0; i < qtdPrimos; i++) { //percorre todo os implicantes primos

            implicante* coluna = implicantesIniciais->inicio;
            for (uint32_t j = 0; j < qtdImplicantes; j++) { //percorre todos os implicantes originais

                //percorre os termos cobertos do implicante primo e vê se ele cobre o implicante original
                for (uint32_t k = 0; k < linha->qtdTermosCobertos; k++) {
                    if (strcmp(linha->termosCobertos[k], coluna->expressao) == 0) {
                        tabela[i][j] = 1; //se cobrir, coloca 1 na tabela
                        break;
                    }
                }
                coluna = coluna->proximo;
            }
            linha = linha->proximo;
        }

        imprimirTabela(tabela, primos, implicantesIniciais, qtdPrimos, qtdImplicantes);

        /*----------------------FIM DA CRIAÇÃO DA MATRIZ----------------------*/

        
        /*----------------------SELECIONAR OS IMPLICANTES ESSENCIAIS----------------------*/

        bool* termosOriginais = calloc(qtdImplicantes, sizeof(bool)); //seria referente ao cabçalho das linhas 
        //vetor binário que contém a informação dos implicantes iniciais, indica se um implicante já foi coberto ou não
        

        bool* primosSelecionados = calloc(qtdPrimos, sizeof(bool)); //seria referente ao cabçalho das colunas
        //vetor binário que contém a informação dos implicantes primos, indica se um implicante escolhido como essencial

        for (uint32_t j = 0; j < qtdImplicantes; j++) { //percorre os termos origianis e conta quantos implicantes primos os cobrem

            uint32_t count = 0; //quantidade de termos que cobrem a expressão original
            uint32_t ultimo = -1; //guarda o índice do último implicante que cobre esse mintermo
            for (uint32_t i = 0; i < qtdPrimos; i++) { //percore todas as linhas (i) daquela coluna (j)
                if (tabela[i][j] == 1) {
                    count++;
                    ultimo = i;
                }
        }

        if (count == 1) { //se apenas um implicante primo cobre aquele termo, ele é essencial
            primosSelecionados[ultimo] = true;// marca esse implicante primo como essencial
            for (uint32_t k = 0; k < qtdImplicantes; k++) { //daí, percorre a tabela na coluna desse implicante primo, linha a linha
                if (tabela[ultimo][k] == 1) { //tabela[ultimo][k] é a coluna do implicante primo primos[ultimo]
                    termosOriginais[k] = true; //seta todos os termos que esse primo cobria como cobertos no vetor binário das expressões originais 
                }
            }
        }

        //implementar a cobertura para primos NÃO ESSENCIAIS (que cobre mais que um termo original)
        //enquanto existirem termos originais não cobertos, continuar o processo

        /*----------------------IMPLICANTES ESSENCIAIS SELECIONADOS----------------------*/




        fclose(input);

    return 0;
}
