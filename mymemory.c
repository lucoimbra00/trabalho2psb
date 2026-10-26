#include <stdio.h>      //printf (display e stats)
#include <stdlib.h>     //malloc e free (só para pool, mymemory_t e nós)
#include "mymemory.h"   //structs e protótipos (aspas, pois é arquivo seu)

mymemory_t* mymemory_init(size_t size)
{
    mymemory_t* gerenciador = malloc(sizeof(mymemory_t)); //ponteiro para o gerenciador do pool
    if (gerenciador == NULL) {
        return NULL; 
    }

    //se conseguiu alocar o gerenciador, parte para definir suas propriedades. Primeiro o pool de memoria em si:
    gerenciador->pool = malloc(size); //aloca o bloco grande de memoria
    if (gerenciador->pool == NULL) {
        free(gerenciador);            //desfaz o 1º malloc se não conseguir alocar o pool
        return NULL;
    }

    gerenciador->total_size = size; //define o tamanho total do pool
    gerenciador->head = NULL;       //inicializa a lista de alocações como vazia (NULL) -> que é só outro controlador/gerenciador, nao a memoria em si
    return gerenciador;
    //pool é o caminho direto para os bytes. head é o caminho para a anotação sobre quem ocupa os bytes.
}

void* mymemory_alloc(mymemory_t* memory, size_t size)
{
    if (size == 0 || size > memory->total_size) {
        return NULL; //não há espaço suficiente no pool
    }

    // Para tudo que eu quiser somar bytes, uso char*, pq char tem tamanho 1 byte. 
    char *inicio_pool = memory->pool;
    char *fim_pool    = inicio_pool + memory->total_size;


    //inicializa ponteiros para percorrer a lista encadeada de alocações
    allocation_t* anterior = NULL; // bloco ANTES da lacuna
    allocation_t* proximo = memory->head; // bloco DEPOIS da lacuna

    int terminou = 0;
    while (!terminou) {

    //primeira lacuna que ele olha é justamente a que fica entre o início do pool e o head

    // 1) Onde a lacuna COMEÇA
    char *inicio_lacuna;
    if (anterior == NULL) {
        // não há bloco antes: a lacuna começa no início do pool
        inicio_lacuna = inicio_pool;
    } else {
        // a lacuna começa onde o bloco anterior TERMINA
        inicio_lacuna = anterior->start + anterior->size;
    }
    
    // 2) Onde a lacuna TERMINA
    char *fim_lacuna;
    if (proximo == NULL) {
        //não tem bloco alocado ainda
        fim_lacuna = fim_pool;
    } else {
        // a lacuna termina onde o próximo bloco COMEÇA
        fim_lacuna = proximo->start;
    }

    // 3) Tamanho da lacuna em bytes
    size_t tamanho_lacuna = fim_lacuna - inicio_lacuna;

     // 4) Cabe o que foi pedido?
        if (tamanho_lacuna >= size) { // cabe, então aloca
            allocation_t *novo = malloc(sizeof(allocation_t)); //cria novo nó para a lista encadeada
            if (novo == NULL) return NULL;

            //preenche suas propriedades
            novo->start = inicio_lacuna;
            novo->size  = size;
            novo->next  = proximo;           // o novo aponta para o bloco que vinha depois

            if (anterior == NULL) {
                memory->head = novo;         // lista estava vazia 
            } else {
                anterior->next = novo;       // encaixa depois do anterior
            }
            return novo->start; //retorna o ponteiro para o inicio do bloco alocado
        }

        if (proximo == NULL) { //não coube e lista acabou
            terminou = 1; //sai do loop
        } else {
            //não coube, mas ainda tem mais blocos na lista. Avança os ponteiros para o próximo bloco para avaliar a próxima lacuna
            anterior = proximo;
            proximo  = proximo->next;
        }
    }

    return NULL;   // nenhuma lacuna serviu
}


void mymemory_free(mymemory_t *memory, void *ptr)
{
    /* TODO */
}

void mymemory_display(mymemory_t *memory)
{
    /* TODO */
}

void mymemory_stats(mymemory_t *memory)
{
    /* TODO */
}

void mymemory_cleanup(mymemory_t *memory)
{
    /* TODO */
}