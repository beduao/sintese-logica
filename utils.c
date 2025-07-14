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

uint8_t existe(string elemento, string* vetorStrings, uint32_t tamanhoVetor){ //verifica se uma string existe em um vetor de strings
    for(uint32_t i = 0; i < tamanhoVetor; i++){
        if(strcmp(vetorStrings[i],elemento)==0){
            return 1; //já existe
        }
    }
    return 0; //não existe
}

void adicionarString(string** vetor, uint32_t* qtd, string nova) { //adiciona uma nova string em um vetor de strings
    string* temp = realloc(*vetor, sizeof(string) * (*qtd + 1));
    if (temp == NULL) {
        fprintf(stderr, "Erro ao alocar memória ao adicionar string.\n");
        free(nova);  // evita vazamento de memória
        return;
    }

    *vetor = temp;
    (*vetor)[*qtd] = nova;
    (*qtd)++;
}


void compararGrupos(string** vetorImplicantes, uint32_t* qtdImplicantes, grupo* vetorGrupos, uint32_t qtdGrupos, uint32_t qtdVariaveis) {
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

        for (uint32_t j = 0; j < tamAtual; j++) {
            for (uint32_t k = 0; k < tamProximo; k++) {

                string comparacao = compararStrings(vetorGrupos[i].vetorStrs[j], vetorGrupos[i + 1].vetorStrs[k], qtdVariaveis);

                if (comparacao != NULL) {
                    
                    // verifica se já existe
                    if (existe(comparacao, *vetorImplicantes, *qtdImplicantes)) {
                        free(comparacao);
                        continue;
                    }

                    // adiciona no vetor de implicantes
                    adicionarString(vetorImplicantes, qtdImplicantes, comparacao);
                    
                    // marca as combinações como feitas
                    combinadosA[j] = 1;
                    combinadosB[k] = 1;
                }
            }
        }

        /*
        DETALHE: AINDA TÁ DENTRO DO PRIMEIRO FOR! 
        ou seja, ainda NÃO atualizou os grupos autal e proximo
        aiciona os termos do grupo ATUAL que não foram combinados
        */
        for (uint32_t j = 0; j < tamAtual; j++) { //percorre a lista com a informação se foi combinado ou não

            if (combinadosA[j] == 0) {  //se achar um elemento que não foi combinado, adiciona ele
                string termo = strdup(vetorGrupos[i].vetorStrs[j]);
                if (termo == NULL) {
                    fprintf(stderr, "Erro ao duplicar string.\n");
                    continue;
                }
                adicionarString(vetorImplicantes, qtdImplicantes, termo);
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
                    string termo = strdup(vetorGrupos[i + 1].vetorStrs[k]);
                    if (termo == NULL) {
                        fprintf(stderr, "Erro ao duplicar string.\n");
                        continue;
                    }
                    adicionarString(vetorImplicantes, qtdImplicantes, termo);
                }
            }
        }

        free(combinadosA);
        free(combinadosB);
    }
}
