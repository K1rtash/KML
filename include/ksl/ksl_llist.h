/*#ifndef KSL_LLIST_H
#define KSL_LLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nodo Nodo;
typedef struct Dato Dato;

struct Dato {
    int num;
    const char* str;
};

struct Nodo {
    Dato dato;
    struct Nodo* ptr;
};

void recorre(Nodo* cabeza) {
    while (cabeza != NULL) {
        printf("{%d, %s} -> ", cabeza->dato.num, cabeza->dato.str);
        cabeza = cabeza->ptr;
    }
    printf("NULL\n");
}

Nodo* busca(Nodo* cabeza, const char* nombre) {
    while(cabeza != NULL) {
        if(strcmp(cabeza->dato.str, nombre) == 0) return cabeza;
        cabeza = cabeza->ptr;
    }
    return NULL;
}

int elimina(Nodo* cabeza, Nodo* nodo) {
    while(cabeza != NULL) {
        if(cabeza->ptr == nodo) {
            cabeza->ptr = nodo->ptr;
            free(nodo);
            return 1;
        }
        cabeza = cabeza->ptr;
    } 
    return 0;
}

Nodo* nuevo(Nodo* cabeza, Dato dato) {
    Nodo* ant = cabeza;
    while(cabeza != NULL) { 
        ant = cabeza;
        cabeza = cabeza->ptr;
    }
    Nodo* n = (Nodo*)malloc(sizeof(Nodo));
    n->dato = dato;
    n->ptr = NULL;
    if(ant) ant->ptr = n;
    return n;
}

void libera(Nodo** cabeza) {
    Nodo* act;
    while(*cabeza != NULL) {
        act = (*cabeza)->ptr;
        free(*cabeza);
        *cabeza = act;
    }
}

int main(void) {    
    Nodo* primero = nuevo(NULL, (Dato){1, "uno"}); 
    nuevo(primero, (Dato){2, "dos"});
    nuevo(primero, (Dato){3, "tres"});
    
    recorre(primero);
    Nodo* encontrado = busca(primero, "tres");
    if(encontrado) {
        printf("Buscado: tres, encontrado: '%d'\n", encontrado->dato.num);  
        printf("%s\n",(elimina(primero, encontrado) ? "Eliminado" : "No se puede Eliminado"));
    }else
       printf("No se peude encotnrar\n");
    recorre(primero); 
    nuevo(primero, (Dato){4, "cuatro"});
    recorre(primero);   
    libera(&primero);   
    primero = nuevo(NULL, (Dato){9, "nueve"});
    recorre(primero);
    return 0;
}

#endif*/