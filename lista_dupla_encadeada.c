#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Contato {
    char nome[50];
    char email[50];
    char telefone[15];
} Contato;

typedef struct No {
    Contato contato;
    struct No *prox;
    struct No *ant;
} No;

typedef struct Lista {
    No *primeiro;
    No *ultimo;
    int tamanho;
} Lista;

// Protótipos das funções auxiliares
Lista* criaLista();
No* criaNo(Contato contato);
void insereContato(Lista *lista, Contato contato);
void insereContatoPorPosicao(Lista *lista, Contato contato, int posicao);
int posicaoInsercao(Lista *lista, char nome[]);
void removeContatoPorPosicao(Lista *lista, int posicao);
No* buscaContatoPorPosicao(Lista *lista, int posicao);
void listaContatos(Lista *lista);
void destroiLista(Lista *lista);
int digitaInteiro();
void lerContato(Contato *contato);
void menu(Lista *lista);
int testaIntervalo(Lista *lista, int posicao);
void mostraContato(No *no);

int main() {
    Lista *lista = criaLista();
    
    // Inicia o menu interativo
    menu(lista);

    // Destrói a lista
    destroiLista(lista);
    
    return 0;
}

Lista* criaLista() {
    Lista *lista = (Lista *)malloc(sizeof(Lista));
    lista->primeiro = NULL;
    lista->ultimo = NULL;
    lista->tamanho = 0;
    return lista;
}

No* criaNo(Contato contato) {
    No *novo = (No *)malloc(sizeof(No));
    novo->contato = contato;
    novo->prox = NULL;
    novo->ant = NULL;
    printf("\nContato criado com sucesso!\n");
    return novo;
}

//controle para não chamar funções que dependam da lista ter dados salvos
int verificaLista(Lista *lista) {
    if (lista->primeiro == NULL) {
        printf("\nNão há contatos na lista!\n");
        return 0;
    }
}

void insereContato(Lista *lista, Contato contato) {
    No *novo = criaNo(contato);
    if (lista->primeiro == NULL) {
        // se a lista está vazia, primeiro e último nós são o "novo" contato
        lista->primeiro = novo;
        lista->ultimo = novo;
    } else {
        // busca posição para inserir na ordem alfabética
        
        //atual recebe o nome do primeiro item da lista
        No *atual = lista->primeiro;
        
        //percorre a lista nó a nó comparando contato.nome com o 1º da lista = "atual"
        while (atual != NULL && strcmp(atual->contato.nome, contato.nome) < 0) {
            atual = atual->prox;
        }
        //atualiza posições na lista
        if (atual == lista->primeiro) {
            // Se iguais insere no início
            novo->prox = lista->primeiro;
            lista->primeiro->ant = novo;
            lista->primeiro = novo;
        } else if (atual == NULL) {
            // Se null (ou seja, identificado como maior que todos) insere no final
            lista->ultimo->prox = novo;
            novo->ant = lista->ultimo;
            lista->ultimo = novo;
        } else {
            // se não é o 1º nem o último, insere no meio atualizando anterior e próximo
            novo->prox = atual;
            novo->ant = atual->ant;
            atual->ant->prox = novo;
            atual->ant = novo;
        }
    }
    lista->tamanho++;
}

int testaIntervalo(Lista *lista, int posicao) {
    //testa intervalo 
    if (posicao < 1 || posicao > lista->tamanho + 1) {
        printf("\nPosição inválida.\n");
        return 0;
    }
}

void insereContatoPorPosicao(Lista *lista, Contato contato, int posicao) {
    int posicaoCerta = posicaoInsercao(lista, contato.nome);
    int resposta;
    
    //alerta sobre a posição informada
    if (posicao != posicaoCerta) {
        printf("\n*** A posição informada não é a correta, deseja continuar?\n");
        printf("Digite 0 para voltar ao menu, 1 para prosseguir: ");
        scanf(" %d", &resposta);
        
        if (resposta == 0) {
            return;
            
        //permite a inclusão em qualquer posição (não foi especificado no enunciado)    
        } else if (resposta == 1) {
            printf("\n Contato será criado na posição indicada...");
        } 
    }
    No *novo = criaNo(contato);
    
    if (posicao == 1) {
    // Inserir no início
        novo->prox = lista->primeiro;
        if (lista->primeiro != NULL) {
            lista->primeiro->ant = novo;
        }
        lista->primeiro = novo;
        if (lista->ultimo == NULL) {
            lista->ultimo = novo;
        }
    } else if (posicao == lista->tamanho + 1) {
        // Inserir no final
        novo->ant = lista->ultimo;
        if (lista->ultimo != NULL) {
            lista->ultimo->prox = novo;
        }
        lista->ultimo = novo;
        if (lista->primeiro == NULL) {
        lista->primeiro = novo;
        }
    } else {
        // Inserir no meio
        No *atual = lista->primeiro;
        for (int i = 1; i < posicao - 1; i++) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        novo->ant = atual;
        atual->prox->ant = novo;
        atual->prox = novo;
    }
    lista->tamanho++;
}

int posicaoInsercao(Lista *lista, char nome[]) {
    No *atual = lista->primeiro;
    int posicao = 1;
    
    //compara nome informado com os dados na lista
    while (atual != NULL && strcmp(atual->contato.nome, nome) < 0) {
        atual = atual->prox;
        posicao++;
    }
    return posicao;
}

void removeContatoPorPosicao(Lista *lista, int posicao) {

    No *atual = lista->primeiro;
    
    //Atualiza atual para apontar para o próximo nó na lista
    for (int i = 1; i < posicao; i++) {
        atual = atual->prox;
    }
    if (atual->ant != NULL) {
        //remove o nó atual da lista (nenhum aponta pra ele)
        atual->ant->prox = atual->prox;
    } else {
        lista->primeiro = atual->prox;
    }
    if (atual->prox != NULL) {
        //remove o nó atual da lista
        atual->prox->ant = atual->ant;
    } else {
        lista->ultimo = atual->ant;
    }
    free(atual);
    lista->tamanho--;
    printf("\nContato removido com sucesso!\n");
}

No* buscaContatoPorPosicao(Lista *lista, int posicao) {
    if (posicao < 1 || posicao > lista->tamanho) { 
        return NULL;
    }
    //localiza posição e atribui dados para "atual"
    No *atual = lista->primeiro;
    for (int i = 1; i < posicao; i++) {
        atual = atual->prox;
    }
    return atual;
}

void listaContatos(Lista *lista) {
    No *atual = lista->primeiro;
    int posicao = 1;
    while (atual != NULL) {
        printf("\nPosição: %d\n", posicao);
        printf("Nome: %s\n", atual->contato.nome);
        printf("Email: %s\n", atual->contato.email);
        printf("Telefone: %s\n", atual->contato.telefone);
        printf("\n           *** \n");
        printf("Endereço atual: %p\n", (void *)atual);
        printf("Endereço próximo: %p\n", (void *)atual->prox);
        printf("Endereço anterior: %p\n", (void *)atual->ant);
        printf("\n-------------------------------\n");
        atual = atual->prox;
        posicao++;
    }
}

void mostraContato(No *no) {
    printf("\nContato encontrado:\n");
    printf("Nome: %s\n", no->contato.nome);
    printf("Email: %s\n", no->contato.email);
    printf("Telefone: %s\n", no->contato.telefone);
}

void destroiLista(Lista *lista) {
    No *atual = lista->primeiro;
    while (atual != NULL) {
        No *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    free(lista);
}

void lerContato(Contato *contato) {
    printf("Digite o nome: ");
    fgets(contato->nome, sizeof(contato->nome), stdin);
    contato->nome[strcspn(contato->nome, "\n")] = '\0';
    printf("Digite o email: ");
    fgets(contato->email, sizeof(contato->email), stdin);
    contato->email[strcspn(contato->email, "\n")] = '\0';
    printf("Digite o telefone: ");
    fgets(contato->telefone, sizeof(contato->telefone), stdin);
    contato->telefone[strcspn(contato->telefone, "\n")] = '\0';
}

int digitaInteiro() {
    int valor;
    scanf("%d", &valor);
    while (getchar() != '\n'); // Limpar buffer
    return valor;
}

void menu(Lista *lista) {
    int opcao, posicao, confereLista;
    char nome[50];
    Contato contato;
    No *no;

    do {
        printf("\n|||| Menu:\n");
        printf("    1. Inserir contato\n");
        printf("    2. Inserir contato por posição\n");
        printf("    3. Encontrar posição de inserção \n");
        printf("    4. Remover contato por posição\n");
        printf("    5. Listar contatos\n");
        printf("    6. Buscar contato por posição \n");
        printf("    7. Sair\n");
        printf("\nEscolha uma opção: \n");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer

        switch (opcao) {
            case 1:
                lerContato(&contato);
                insereContato(lista, contato);
                break;
            case 2:
                confereLista = verificaLista(lista);
                if (confereLista == 0) {
                    printf("* Posição 1 é a única disponível.\n");
                    break;
                }
                
                printf("Digite a posição onde deseja inserir o contato: ");
                posicao = digitaInteiro();
                
                
                if(testaIntervalo(lista, posicao)) {
                    lerContato(&contato);
                    insereContatoPorPosicao(lista, contato, posicao);
                }
                break;
            case 3:
                confereLista = verificaLista(lista);
                if (confereLista == 0) {
                    printf("* Posição 1 é a única disponível.\n");
                    break;
                }
                printf("Digite o nome do contato para encontrar a posição de inserção: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                posicao = posicaoInsercao(lista, nome);
                printf("A posição de inserção para %s é: %d\n", nome, posicao);
                break;
            case 4:
                confereLista = verificaLista(lista);
                if (confereLista == 0) break;
                printf("Digite a posição do contato a ser removido: ");
                posicao = digitaInteiro();
                testaIntervalo(lista, posicao);
                removeContatoPorPosicao(lista, posicao);
                break;
            case 5:
                confereLista = verificaLista(lista);
                if (confereLista == 0) break;
                listaContatos(lista);
                break;
            case 6:
                confereLista = verificaLista(lista);
                if (confereLista == 0) break;
                printf("Digite a posição do contato a ser buscado: ");
                posicao = digitaInteiro();
                if (testaIntervalo(lista, posicao) == 0) break;
                no = buscaContatoPorPosicao(lista, posicao);
                mostraContato(no);
                break;
            case 7:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 7);
}

