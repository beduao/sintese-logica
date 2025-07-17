#include "utils.h"

/*-----------IMPRESSÃO-----------*/
void imprimirImplicantes(implicante* lista) { 
    implicante* aux = lista;
    while(aux!=NULL){
        printf("%s\n",aux->expressao);
        aux = aux->proximo;
    }
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

void addListaImplicante(implicante** lista, implicante* novoImplicante){
    if(*lista==NULL){
        *lista = novoImplicante;
        return;
    }
    implicante* aux = *lista;
    while(aux->proximo!=NULL) aux = aux->proximo;
    aux->proximo = novoImplicante;
}

