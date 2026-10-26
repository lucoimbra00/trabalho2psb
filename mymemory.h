#ifndef MYMEMORY_H
#define MYMEMORY_H
#include <stddef.h>   //importa o tipo size_t 

//Registro de um bloco alocado dentro do pool (no da lista encadeada) 
typedef struct allocation {
    void* start;                 //inicio do bloco dentro do pool 
    size_t size;                 //tamanho do bloco em bytes 
    struct allocation *next;     //proximo registro da lista 
} allocation_t;

//Gerenciador do pool inteiro 
typedef struct {
    void* pool;                  // ponteiro para o bloco de memória real
    size_t total_size;           //tamanho total do pool 
    allocation_t *head;          //primeira alocacao (NULL se vazia)
} mymemory_t;

//Funcoes principais (prototipos)
mymemory_t* mymemory_init(size_t size);
void*       mymemory_alloc(mymemory_t* memory, size_t size);
void        mymemory_free(mymemory_t* memory, void *ptr);
void        mymemory_display(mymemory_t* memory);
void        mymemory_stats(mymemory_t* memory);
void        mymemory_cleanup(mymemory_t* memory);

#endif /* MYMEMORY_H */