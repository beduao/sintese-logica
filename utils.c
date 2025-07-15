#include "utils.h"

void imprimirStrings(string* mintermos, uint32_t quantidade) { //função auxiliar para testes
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

void agrupar(string* vetorMintermos, uint32_t qtdMintermos, grupo* vetorGrupos, uint32_t* qtdGrupos) {

    for (uint32_t i = 0; i < qtdMintermos; i++) { //percorre todo o vetor de mintermos

        uint32_t numUns = contarUns(vetorMintermos[i]);
        int grupoEncontrado = 0;

        // buscar grupo existente com mesmo numero de 1s
        for (uint32_t j = 0; j < *qtdGrupos; j++) { //percorre todo o vetor dos grupos

            //se achar um grupo que já existe
            if (vetorGrupos[j].qtdUns == numUns) { 
                // adiciona a string ao grupo existente
                uint32_t index = vetorGrupos[j].stringsArmazenadas; //espaço livre no vetor do grupo selecionado
                vetorGrupos[j].vetorStrs[index] = strdup(vetorMintermos[i]);

                vetorGrupos[j].stringsArmazenadas++;
                grupoEncontrado = 1;
                break;
            }
        }

        // se não achou, cria um novo grupo
        if (!grupoEncontrado) {
            grupo novoGrupo;
            novoGrupo.qtdUns = numUns;
            novoGrupo.stringsArmazenadas = 1;

            novoGrupo.vetorStrs = malloc(sizeof(string) * qtdMintermos); // alocação máxima possível, para o pior caso em que todos os mintermos tem a mesma quantidade de uns
            novoGrupo.vetorStrs[0] = strdup(vetorMintermos[i]);

            vetorGrupos[*qtdGrupos] = novoGrupo;
            (*qtdGrupos)++;
        }
    }
}

void imprimirGrupos(grupo* vetorGrupos, uint32_t qtdGrupos) {
    for (uint32_t i = 0; i < qtdGrupos; i++) {
        printf("Grupo com %u uns:\n", vetorGrupos[i].qtdUns);
        for (uint32_t j = 0; j < vetorGrupos[i].stringsArmazenadas; j++) {
            printf("-%s\n", vetorGrupos[i].vetorStrs[j]);
        }
    }
}

string compararStrings(string str1, string str2, uint32_t tamanho) { //o tamanho das strings é sempre a quantiadde de variáveis 
    uint8_t diff = 0;
    string novaString = NULL;
    for (uint32_t i = 0; i < tamanho; i++){
        if(str1[i]!=str2[i]) diff++;
    }
    if (diff==1){ //se difere em apenas um bit
        novaString = malloc((tamanho+1)*sizeof(char));
        for (uint32_t j = 0; j < tamanho; j++){
            if(str1[j]==str2[j]){
                novaString[j] = str1[j];
            }
            else novaString[j] = '-';
        }
        novaString[tamanho] = '\0';
    } 
    return novaString;
}

uint8_t existe(string elemento, implicante* vetorImplicantes, uint32_t tamanhoVetor){ //verifica se uma string existe em um vetor de implicantes
    for(uint32_t i = 0; i < tamanhoVetor; i++){
        if(strcmp(vetorImplicantes[i].expressao,elemento)==0){
            return 1; //já existe
        }
    }
    return 0; //não existe
}

void adicionarImplicante(implicante** vetor, uint32_t* qtd, string nova, string termo1, string termo2) { //adiciona um novo implicante em um vetor de implicantes
    implicante* temp = realloc(*vetor, sizeof(implicante) * (*qtd + 1));
    if (temp == NULL) {
        fprintf(stderr, "Erro ao alocar memória ao adicionar implicante.\n");
        exit(1);
    }

    *vetor = temp;
    (*vetor)[*qtd].expressao = strdup(nova);

    if(strcmp(termo1,termo2)==0){ 
        (*vetor)[*qtd].termosCobertos = NULL;
        }   
    else {
        (*vetor)[*qtd].termosCobertos = malloc(sizeof(string)*2);

        (*vetor)[*qtd].termosCobertos[0] = strdup(termo1);
        (*vetor)[*qtd].termosCobertos[1] = strdup(termo2);
        
    }
    (*qtd)++;
}


void compararGrupos(implicante** vetorImplicantes, uint32_t* qtdImplicantes, grupo* vetorGrupos, uint32_t qtdGrupos, uint32_t qtdVariaveis) {
    //recebe um vetor de implicantes (por referência) vazio e adiciona nele os implicantes a medida que faz comparações entre os grupos

    //exceções!
    if (qtdGrupos == 0) return;
    if (qtdGrupos == 1) { //se há apenas um grupo, todos os seus termos são implicantes
        for (uint32_t i = 0; i < vetorGrupos[0].stringsArmazenadas; i++) {
            string termo = vetorGrupos[0].vetorStrs[i];
            adicionarImplicante(vetorImplicantes, qtdImplicantes, termo, termo, termo);
        }
        return; //encerra a função
    }

    //inicia de fato o loop
    for (uint32_t i = 0; i < qtdGrupos - 1; i++) {

        uint32_t tamAtual = vetorGrupos[i].stringsArmazenadas; //tamanho do vetor de strings do grupo atual(i)
        uint32_t tamProximo = vetorGrupos[i + 1].stringsArmazenadas; //tamanho do vetor de strings do grupo subsequente(i+1)

        /* 
        cria vetores com os mesmo tamanhos dos vetores de strings de cada grupos, com a informação binaria se eles foram combinados ou não
        exemplo: [1,0,0,1,1,0,...,tamA]
        inicialmente todos são 0, vai mudando pra 1 conforme as combinações são feitas
        */
        uint8_t* combinadosA = calloc(tamAtual, sizeof(uint8_t));
        uint8_t* combinadosB = calloc(tamProximo, sizeof(uint8_t));
        if (combinadosA == NULL || combinadosB == NULL) {
            fprintf(stderr, "Erro ao alocar memória para vetores de combinação.\n");
            exit(1);
        }

        for (uint32_t j = 0; j < tamAtual; j++) {

            for (uint32_t k = 0; k < tamProximo; k++) {

                string comparacao = compararStrings(vetorGrupos[i].vetorStrs[j], vetorGrupos[i + 1].vetorStrs[k], qtdVariaveis);

                if (comparacao != NULL) {
                    if (!existe(comparacao, *vetorImplicantes, *qtdImplicantes)) {
                        adicionarImplicante(vetorImplicantes, qtdImplicantes, comparacao, vetorGrupos[i].vetorStrs[j], vetorGrupos[i + 1].vetorStrs[k]);
                        combinadosA[j] = 1;
                        combinadosB[k] = 1;
                    }
                    // CORREÇÃO: libera a memória de 'comparacao' após o uso indepentende se já existe o implicante ou não
                    free(comparacao);
                }
            }
        }

        /*
        DETALHE: AINDA TÁ DENTRO DO PRIMEIRO FOR! 
        ou seja, ainda NÃO atualizou os grupos autal e proximo
        adiciona os termos do grupo ATUAL que não foram combinados
        Nota: se um termo não foi adicionado, o seu grupo coberto é ele mesmo, portanto, na struct ficará setado como NULL
        */
        for (uint32_t j = 0; j < tamAtual; j++) { //percorre a lista com a informação se foi combinado ou não

            if (combinadosA[j] == 0) {  //se achar um elemento que não foi combinado, adiciona ele
                string termo = vetorGrupos[i].vetorStrs[j];
                //CORREÇÂO: não precisa do strdup, a função de adicionar já tem
                adicionarImplicante(vetorImplicantes, qtdImplicantes, termo, termo, termo);
                //como não foi feita nenhuma combinação com esse termo, o único termo coberto é ele mesmo
                //por isso passa ele mesmo duas vezes, pra função adicionarImplicante setar o campo termosCobertos como NULL
            }
        }

        /*
        se for o último par de grupos, adicionar os não combinados também do grupo final, que é o proximo 
        DETALHE: o que está nesse if só é executa uma vez no loop
        que é quando o par de grupos corresponde ao par (penúltimo, último)
        */
        if (i == qtdGrupos - 2) {
            for (uint32_t k = 0; k < tamProximo; k++) { //percorre a lista com a informação se foi combinado ou não

                if (combinadosB[k] == 0) {
                    string termo = vetorGrupos[i + 1].vetorStrs[k];
                    //CORREÇÂO: não precisa do strdup, a função de adicionar já tem
                    adicionarImplicante(vetorImplicantes, qtdImplicantes, termo, termo, termo);
                     //como não foi feita nenhuma combinação com esse termo, o único termo coberto é ele mesmo
                    //por isso passa ele mesmo duas vezes, pra função adicionarImplicante setar o campo termosCobertos como NULL
                }
            }
        }

        free(combinadosA);
        free(combinadosB);
    }
}

void imprimirImplicantes(implicante* vetorImplicantes, uint32_t qtdImplicantes) {
    printf("--- Tabela de Implicantes ---\n");
    
    // Verifica se há algo para imprimir
    if (vetorImplicantes == NULL || qtdImplicantes == 0) {
        printf("Nenhum implicante para exibir.\n");
        return;
    }

    // Itera sobre cada implicante no vetor
    for (uint32_t i = 0; i < qtdImplicantes; i++) {
        // Imprime a expressão do implicante (ex: "10-")
        printf("%s - [", vetorImplicantes[i].expressao);

        // Verifica se o implicante foi resultado de uma combinação
        if (vetorImplicantes[i].termosCobertos != NULL) {
            // Se sim, imprime os dois termos que ele cobre
            printf("%s, %s", vetorImplicantes[i].termosCobertos[0], vetorImplicantes[i].termosCobertos[1]);
        } else {
            // Se não, ele é um implicante primo que não se combinou.
            // Nesse caso, ele cobre a si mesmo.
            printf("%s", vetorImplicantes[i].expressao);
        }

        // Fecha o colchete e pula para a próxima linha
        printf("]\n");
    }
    printf("---------------------------\n");
}