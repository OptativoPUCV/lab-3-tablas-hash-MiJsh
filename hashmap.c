#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long indice = hash(key, map-> capacity);

    while (map -> buckets[indice] != NULL && map -> buckets[indice]->key != NULL) {
        if (is_equal(map -> buckets[indice]->key, key)) return;
        indice = (indice + 1) % map-> capacity;
    }

    map -> buckets[indice] = createPair(key, value);
    map -> current = indice;
    map -> size++;
}

//a - Cree una variable auxiliar de tipo Pair** para mantener el arreglo map->buckets (*old_buckets*);

//b - Duplique el valor de la variable capacity.

//c - Asigne a map->buckets un nuevo arreglo con la nueva capacidad.

//d - Inicialice size a 0.

//e - Inserte los elementos del arreglo *old_buckets* en el mapa (use la función insertMap que ya implementó).


void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)

    // creo variable aux 
    Pair ** old_buckets = map->buckets;
    long old_capacity = map -> capacity;

    //duplico
    map->capacity *= 2; 

    //asigno a nuevo
    map->buckets = (Pair **) calloc(map->capacity,sizeof(Pair *));
    if (!map -> buckets) 
    {
        return;
    }

    //inicio size a 0
    map->size = 0;

    //inserto 
    for (long i = 0; i < old_capacity; i++)
    {
        if(old_buckets[i] != NULL && old_buckets[i] -> key != NULL) 
        {
            insertMap(map, old_buckets[i] -> key, old_buckets[i] -> value);
        }
    }
}   


HashMap * createMap(long capacity) {
    HashMap * mapa = (HashMap *)malloc(sizeof(HashMap));
    if (mapa == NULL) return NULL;

    mapa->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
    if (mapa->buckets == NULL) {
        free(mapa);
        return NULL;
    }

    mapa -> size = 0;
    mapa -> capacity = capacity;
    mapa -> current = -1;

    return mapa;
}

Pair * searchMap(HashMap * map,  char * key) {   
    long indice = hash(key, map->capacity);

    while (map -> buckets[indice] != NULL) {
        if (map -> buckets[indice]-> key != NULL && is_equal(map -> buckets[indice]->key, key)) {
            map -> current = indice;
            return map -> buckets[indice];
        }
        indice = (indice + 1) % map->capacity;
    }
    return NULL;
}


void eraseMap(HashMap * map,  char * key) {    
    Pair *pair = searchMap(map, key);

    if (pair == NULL) return;

    //no eliminar lol, solo invalido
    pair -> key = NULL;

    map -> size--;
}

Pair * firstMap(HashMap * map) {
    for (long i = 0; i < map-> capacity; i++)
    {
        if (map -> buckets[i] != NULL && map -> buckets[i]->key != NULL) 
        {
            map -> current = i;
            return map -> buckets [i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    //>:3
    for (long i = map -> current + 1; i < map-> capacity; i++)
    {
        if (map -> buckets[i] != NULL && map -> buckets[i]->key != NULL) 
        {
            map -> current = i;
            return map -> buckets [i];
        }
    }
    return NULL;
}
