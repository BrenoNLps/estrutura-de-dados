#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define TAMBASE 200

typedef struct NO{
    int info;
    int altura;
    struct pessoa *pessoa;
    struct NO *esq;
    struct NO *dir;
}*ArvAVL;

typedef struct pessoa {
    char CPF[12];
    char nome[TAMBASE];
    char sobrenome[TAMBASE];
} pessoa;

/*typedef struct grafo {
    int arestas;
    int vertices[TAMBASE][TAMBASE];
}grafo;*/

pessoa* cadastrar(){
	
	pessoa *novaPessoa;
	novaPessoa = (pessoa*)malloc(sizeof(pessoa));
	

    printf("\nDigite o CPF: ");
    scanf("%s", &novaPessoa->CPF);
    fflush(stdin);

    printf("\nDigite o Nome: ");
    gets(novaPessoa->nome);
    fflush(stdin);


    printf("\nDigite o Sobrenome: ");
    gets(novaPessoa->sobrenome);
    fflush(stdin);
   
    return novaPessoa;
}



ArvAVL* cria_ArvAVL(){
    ArvAVL* raiz;
    raiz = (ArvAVL*) malloc(sizeof(ArvAVL));
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}
///////////////////////////////////////////////////////////////////////
int altura_NO(struct NO* no){
    if(no == NULL)
        return -1;
    else
    return no->altura;
}

int fatorBalanceamento_NO(struct NO* no){
    return labs(altura_NO(no->esq) - altura_NO(no->dir));
}

int maior(int x, int y){
    if(x > y)
        return x;
    else
        return y;
}
void RotacaoLL(ArvAVL *A){//LL
    printf("RotacaoLL\n");
    struct NO *B;
    B = (*A)->esq;
    (*A)->esq = B->dir;
    B->dir = *A;
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->esq),(*A)->altura) + 1;
    *A = B;
}

void RotacaoRR(ArvAVL *A){//RR
    printf("RotacaoRR\n");
    struct NO *B;
    B = (*A)->dir;
    (*A)->dir = B->esq;
    B->esq = (*A);
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->dir),(*A)->altura) + 1;
    (*A) = B;
}
void RotacaoLR(ArvAVL *A){//LR
    RotacaoRR(&(*A)->esq);
    RotacaoLL(A);
}

void RotacaoRL(ArvAVL *A){//RL
    RotacaoLL(&(*A)->dir);
    RotacaoRR(A);
}


int insere_ArvAVL(ArvAVL *raiz,pessoa *pessoaNova,int valor){
    int res;
    if(*raiz == NULL){//Árvore vazia ou nó folha
        struct NO *novo;
        novo = (struct NO*)malloc(sizeof(struct NO));
        if(novo == NULL)
            return 0;

        novo->info = valor;
        novo->altura = 0;
        novo->pessoa = pessoaNova;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
        return 1;
    }

    struct NO *atual = *raiz;
    if(valor < atual->info){
        if((res = insere_ArvAVL(&(atual->esq),pessoaNova,valor)) == 1){
            if(fatorBalanceamento_NO(atual) >= 2){
                if(valor < (*raiz)->esq->info ){
                    RotacaoLL(raiz);
                }else{
                    RotacaoLR(raiz);
                }
            }
        }
    }else{
        if(valor > atual->info){
            if((res = insere_ArvAVL(&(atual->dir),pessoaNova,valor)) == 1){
                if(fatorBalanceamento_NO(atual) >= 2){
                    if((*raiz)->dir->info < valor){
                        RotacaoRR(raiz);
                    }else{
                        RotacaoRL(raiz);
                    }
                }
            }
        }else{
            printf("Valor duplicado!!\n");
            return 0;
        }
    }

    atual->altura = maior(altura_NO(atual->esq),altura_NO(atual->dir)) + 1;

    return res;
}


void emOrdem_ArvAVL2(ArvAVL *raiz){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        emOrdem_ArvAVL2(&((*raiz)->esq));
        //printf("%d\n",(*raiz)->info);
        printf("__________________________________\n");
        printf("ID: %d\n",(*raiz)->info);
        printf("CPF: %s\n",(*raiz)->pessoa->CPF);
        printf("NOME: %s\n",(*raiz)->pessoa->nome);
        printf("SOBRENOME: %s\n",(*raiz)->pessoa->sobrenome);

        emOrdem_ArvAVL2(&((*raiz)->dir));
    }
}


///////////////////////////////////////////////////////////////////////////////////

int menu(){
    int escolha;
    printf("__________________________________\n\n        PAINEL DE OPCOES\n__________________________________\nDigite o numero da opcao desejada:\n\n");
    printf("1-Cadastrar pessoa \n2-Imprimir todas as pessoas em ordem de ID \n3-Adicionar novo amigo \n4-Imprimir todos os amigos  \n5-Imprimir amigos de amigos   \n0-sair\n\n>> ");
        
    scanf("%i", &escolha);
    printf("__________________________________\n");
    return escolha;
}

int cria_Adiciona(ArvAVL *avl,int numeroPessoas, int grafo[TAMBASE][TAMBASE]){
    pessoa* pessoaCriada;//Cria pessoa
    pessoaCriada= cadastrar();//pega os dados
    numeroPessoas++;//aumenta o numero de pessoas cadastradas
    insere_ArvAVL(avl,pessoaCriada,numeroPessoas);//insere na arvore
    
    for(int x=0;x<numeroPessoas;x++){//percorre numero de pessoas cadastradas
        grafo[x][numeroPessoas]=0;//adiciona 0 para o final do grafo de todas as pessoas(desbloquea espaço para nova pessoa cadastrada e seta como não amigo)  
    }
    for(int y=0;y<=numeroPessoas;y++){//faz o mesmo com a nova pessoa cadastrada so que com as pessoas ja existentes
        grafo[numeroPessoas][y]=0;
    }

    return numeroPessoas;  
}

void adicionarAmigo(int numeroPessoas,int grafo[TAMBASE][TAMBASE]){
    int idPessoa;
    int idAmigo;

    printf("Digite o ID da pessoa que ira adicionar um amigo:\n");
    scanf("%d", &idPessoa);

    if(idPessoa <= numeroPessoas && idPessoa>=0 ){//se o id for valido
        do{
            printf("Digite o ID do novo amigo(ou digite um numero maior que 200 para sair):\n");
            scanf("%d", &idAmigo);

            if(idAmigo<200){//se nao for para sair do laço
                if(idAmigo<=numeroPessoas && idAmigo!= idPessoa && idAmigo>=0){//se for um id valido
                    grafo[idPessoa][idAmigo]=1;
                    grafo[idAmigo][idPessoa]=1;
                }else{printf("ID invalido!\n");}
            }

        }while(idAmigo<200);
    }else{printf("ID nao encontrado!\n");}



}

void verificarAmizade(ArvAVL *raiz, int id){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        verificarAmizade(&((*raiz)->esq),id);
        //printf("%d\n",(*raiz)->info);
        if((*raiz)->info == id){
            printf("__________________________________\n");
            printf("ID: %d\n",(*raiz)->info);
            printf("CPF: %s\n",(*raiz)->pessoa->CPF);
            printf("NOME: %s\n",(*raiz)->pessoa->nome);
            printf("SOBRENOME: %s\n",(*raiz)->pessoa->sobrenome);           
        }
        verificarAmizade(&((*raiz)->dir),id);
    }
}

void imprimirAmigos(int numeroPessoas,int grafo[TAMBASE][TAMBASE],ArvAVL *raiz){
    int idPessoa;
    int amigos=0;

    printf("Digite o ID da pessoa que ira imprimir os amigos:\n");
    scanf("%d", &idPessoa);

    if(idPessoa <= numeroPessoas && idPessoa>=0 ){//se o id for valido

        for(int x=0;x<=numeroPessoas;x++){//for para verificar se existem amigos 
            amigos+= grafo[idPessoa][x];
        }

        if(amigos>0){
            printf("__________________________________\n");
            printf("         LISTA DE AMIGOS\n"); 

            for(int x=0;x<=numeroPessoas;x++){//percorre a lista de amigos
                if(grafo[idPessoa][x]==1){
                    verificarAmizade(raiz,x);//quando encontrar ira percorrer a arvore e imprimir
                }
            }
            printf("\n\nAmigos:%d\n",amigos);
            printf("__________________________________\n");
        }else{printf("Nenhum amigo encontrado!");printf("__________________________________\n");}
    
    }else{printf("ID nao encontrado!\n");}

}

void imprimirAmigosDeAmigo(int numeroPessoas,int grafo[TAMBASE][TAMBASE],ArvAVL *raiz){
    int idPessoa;
    int amigos=0;

    printf("Digite o ID da pessoa que ira ver os amigos de um de seus amigos:\n");
    scanf("%d", &idPessoa);

    if(idPessoa <= numeroPessoas && idPessoa>=0 ){//se o id for valido

        for(int x=0;x<=numeroPessoas;x++){//for para verificar se existem amigos 
            amigos+= grafo[idPessoa][x];
        }

        if(amigos>0){
            int idAmigo=0;
            while(idAmigo<200){
                printf("Digite o ID do amigo(ou digite um numero maior que 200 para sair):\n");
                scanf("%d", &idAmigo);

                if(idAmigo<200){//se nao for para sair do laço
                    if(idAmigo<=numeroPessoas && idAmigo!= idPessoa && idAmigo>=0){//se for um id valido

                        amigos=0;
                        for(int x=0;x<=numeroPessoas;x++){//for para verificar se o amigo tem amigos
                            amigos+= grafo[idAmigo][x];
                        }

                        if(amigos>0){
                            printf("__________________________________\n");
                            printf("         LISTA DE AMIGOS\n"); 

                            for(int x=0;x<=numeroPessoas;x++){//percorre a lista de amigos
                                if(grafo[idAmigo][x]==1){
                                    verificarAmizade(raiz,x);//quando encontrar ira percorrer a arvore e imprimir
                                }
                            }
                            printf("\n\nAmigos:%d\n",amigos);
                            printf("__________________________________\n");
                        }else{printf("Nenhum amigo encontrado!");printf("__________________________________\n");}


                    }else{printf("ID invalido!\n");}
                }

            }

        }else{printf("Nenhum amigo encontrado!");printf("__________________________________\n");}
    
    }else{printf("ID nao encontrado!\n");}

}




int main(){
    ArvAVL* avl;
    avl = cria_ArvAVL();
    int numeroPessoas= -1;//numero de pessoas cadastradas
    int grafo[TAMBASE][TAMBASE];



    int escolha=100;
    do{

        escolha=menu();
  
        switch (escolha)
        {
            case 1:
                numeroPessoas= cria_Adiciona(avl,numeroPessoas,grafo);
                break;
            case 2:
                emOrdem_ArvAVL2(avl);
                break;
            case 3:
                adicionarAmigo(numeroPessoas,grafo);
                break;
            case 4:
                imprimirAmigos(numeroPessoas,grafo,avl);
                break;
            case 5:
                imprimirAmigosDeAmigo(numeroPessoas,grafo,avl);
                break;
        }   

    }while(escolha != 0);
    

    printf("      EXECUCAO FINALIZADA!");
}
