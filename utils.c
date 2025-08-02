#include "utils.h"

/*-----------IMPRESSÃO-----------*/
void imprimirFila(fila* f) {
    printf("---------------------------\n");
    implicante* aux = f->inicio;
    while (aux != NULL) {
        printf("%s\n", aux->expressao);
        aux = aux->proximo;
    }
    printf("---------------------------\n");
}

void imprimirGrupos(grupo* vetorGrupos, uint32_t tamanho){
    printf("\n|------------Grupos formados:------------|\n\n");
    for(uint32_t i = 0; i < tamanho; i++){
        printf("Grupo %u: (numero de uns = %u)\n", i, vetorGrupos[i].qtdUns);
        imprimirFila(vetorGrupos[i].filaImplicantes);
        printf("\n");
    }
    printf("\n");
}

void imprimirTabela(uint8_t** tabela, fila* primos, fila* mintermos, uint32_t qtdPrimos, uint32_t qtdMintermos) {
    printf("\n-----------------------------------------------------------------------");
    printf("\nImplicantes Originais x Implicantes Primos:\n");

    // cabeçalho das colunas
    printf("            "); // espaço para o nome do implicante
    implicante* p = primos->inicio;
    while (p != NULL) {
        printf("%-10s ", p->expressao);
        p = p->proximo;
    }
    printf("\n");

    // corpo da tabela
    implicante* m = mintermos->inicio;
    for (uint32_t i = 0; i < qtdMintermos; i++) {
        printf("%-10s ", m->expressao);
        for (uint32_t j = 0; j < qtdPrimos; j++) {
            printf("   %d      ", tabela[j][i]);
        }
        printf("\n");
        m = m->proximo;
    }
    printf("-----------------------------------------------------------------------\n");

}

void printarFinal(fila* primos, uint32_t qtdPrimos, bool* implicante_selecionado){
    implicante* p = primos->inicio;

    uint32_t primeiro_implicante = true;

    for (uint32_t i = 0; i < qtdPrimos; i++) {
        if (implicante_selecionado[i]) {
            
            if (!primeiro_implicante) printf(" + ");
            
            string exp = p->expressao;
            uint32_t x = 0;

            while(exp[x] != '\0'){
                if(exp[x] == '1') {
                    printf("v%u", x);
                }
                else if(exp[x] == '0') {
                    printf("~v%u", x);
                }
                x++;
            }
            
            primeiro_implicante = false;
        }
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

bool existe_no_vetor(string* vetor, uint32_t tamanhoVetor, string elemento) {
    for (uint32_t i = 0; i < tamanhoVetor; i++) {
        if (strcmp(vetor[i], elemento) == 0) {
            return true;
        }
    }
    return false;
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

/*-----------LIBERAÇÃO-----------*/

void liberar_implicante(implicante* imp) {
    if (imp == NULL) return;

    free(imp->expressao);
    for (uint32_t i = 0; i < imp->qtdTermosCobertos; i++) {
        free(imp->termosCobertos[i]);
    }
    free(imp->termosCobertos);
    free(imp);
}