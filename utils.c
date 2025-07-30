#include "utils.h"

/*-----------IMPRESSÃO-----------*/
void imprimirFila(fila* f) {
    implicante* aux = f->inicio;
    while (aux != NULL) {
        printf("%s\n", aux->expressao);
        aux = aux->proximo;
    }
}

void imprimirGrupos(grupo* vetorGrupos, uint32_t tamanho){
    for(uint32_t i = 0; i < tamanho; i++){
        printf("Grupo %u: (numero de uns = %u)\n", i, vetorGrupos[i].qtdUns);
        imprimirFila(vetorGrupos[i].filaImplicantes);
        printf("\n");
    }
}

void imprimirTabela(uint8_t** tabela, fila* primos, fila* mintermos, uint32_t qtdPrimos, uint32_t qtdMintermos) {
    printf("\nTabela de Cobertura (Implicantes Primos x Implicantes Originais):\n");

    // cabeçalho das colunas
    printf("            "); // espaço para o nome do implicante
    implicante* m = mintermos->inicio;
    while (m != NULL) {
        printf("%s ", m->expressao);
        m = m->proximo;
    }
    printf("\n");

    // corpo da tabela
    implicante* p = primos->inicio;
    for (uint32_t i = 0; i < qtdPrimos; i++) {
        printf("%-10s ", p->expressao);
        for (uint32_t j = 0; j < qtdMintermos; j++) {
            printf("   %d", tabela[i][j]);
        }
        printf("\n");
        p = p->proximo;
    }

    printf("\n");
}


/*-----------LÓGICAS-----------*/
uint32_t contarUns(string linha) {
    uint32_t contagem = 0;
    while(*linha!='\0'){
        if (*linha=='1') {
            contagem++;
        }
        linha++;
    }
    return contagem;
}

string compararStrings(string str1, string str2, uint32_t tamanho) {
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

bool existe(implicante* lista, string elemento){
    implicante* aux = lista;
    while(aux!=NULL){
        if(strcmp(aux->expressao,elemento)==0) return true;
        aux = aux->proximo;
    }
    return false; //não existe
}

/*-----------INSERÇÃO-----------*/
void addVetorStr(string** vetor, string novaString, uint32_t* tamanhoVetor){
    uint32_t qtd = *tamanhoVetor;
    string* temp = realloc((*vetor),sizeof(string)*(qtd+1));
    if(temp==NULL){
        perror("Erro fatal ao alocar no vetor de termos cobertos. Programa finalizado\n");
        exit(1);
    }
    else {
        (*vetor) = temp;
        (*vetor)[qtd] = strdup(novaString);
        (*tamanhoVetor)++;
    }
}

//alterado para tempo o(1)
void add_na_fila(fila** implicantes, implicante* novoImplicante) {
    if (*implicantes == NULL) {
        *implicantes = malloc(sizeof(fila));
        (*implicantes)->inicio = NULL;
        (*implicantes)->fim = NULL;
    }

    novoImplicante->proximo = NULL;

    if ((*implicantes)->fim == NULL) {
        (*implicantes)->inicio = novoImplicante;
        (*implicantes)->fim = novoImplicante;
    } else {
        (*implicantes)->fim->proximo = novoImplicante;
        (*implicantes)->fim = novoImplicante;
    }
}

/*-----------CRIAÇÃO-----------*/

implicante* copiar_implicante(implicante* original) {
    implicante* novo = malloc(sizeof(implicante));
    if (!novo) {
        perror("Erro ao copiar implicante");
        exit(1);
    }

    novo->expressao = strdup(original->expressao);
    novo->combinado = original->combinado;
    novo->qtdTermosCobertos = original->qtdTermosCobertos;
    novo->proximo = NULL;

    if (original->qtdTermosCobertos > 0) {
        novo->termosCobertos = malloc(sizeof(string) * original->qtdTermosCobertos);
        for (uint32_t i = 0; i < original->qtdTermosCobertos; i++) {
            novo->termosCobertos[i] = strdup(original->termosCobertos[i]);
        }
    } else {
        novo->termosCobertos = NULL;
    }

    return novo;
}