#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdas/extra.h"
#include "tdas/list.h"

typedef struct {
    char nombre_Item[50] ;
    int peso ;
    int valor ;
} Item ;

typedef struct {
    int ID ;
    char nombre[50] ;
    char descripcion[200] ;
    List*items ;
    int arriba, abajo, izquierda, derecha ;
    int es_final ;
} Escenario ;

Escenario* crearEscenario(char** campos) {
    Escenario* nuevoEscenario = (Escenario*)malloc(sizeof(Escenario)) ;
    if (nuevoEscenario == NULL) return NULL ;

    nuevoEscenario->ID = atoi(campos[0]) ;
    strcpy(nuevoEscenario->nombre, campos[1]) ;
    strcpy(nuevoEscenario->descripcion, campos[2]) ;

    nuevoEscenario->items = list_create() ; //colocamos los items del escenario
    List* listadeItems = split_string(campos[3], ";") ;
    for (char* itemsitos = list_first(listadeItems) ; itemsitos != NULL ; itemsitos = list_next(listadeItems)) {
        List* valores = split_string(itemsitos, ",") ;
        Item* nuevoItem = (Item*)malloc(sizeof(Item)) ;
        if (nuevoItem == NULL) return NULL ;

        strcpy(nuevoItem->nombre_Item, list_first(valores)) ;
        nuevoItem->peso = atoi(list_next(valores)) ;
        nuevoItem->valor = atoi(list_next(valores)) ;
        list_pushBack(nuevoEscenario->items, nuevoItem) ;

        list_clean(valores) ;
        free(valores) ;
    }
    list_clean(listadeItems) ;
    free(listadeItems) ;

    nuevoEscenario->arriba = atoi(campos[4]) ;
    nuevoEscenario->abajo = atoi(campos[5]) ;
    nuevoEscenario->izquierda = atoi(campos[6]) ;
    nuevoEscenario->derecha = atoi(campos[7]) ;
    nuevoEscenario->es_final = atoi(campos[8]) ;

    return nuevoEscenario ;
}

void cargadeEscenarios(List* escenarios){
    FILE *archivo = fopen("graphquest.csv", "r") ;
    if (archivo == NULL) {
        perror("Error al abrir el archivo") ; // Informa si el archivo no puede abrirse
        return;
    }
    char **campos ;
    campos = leer_linea_csv(archivo, ',') ;// Lee encabezados del CSV

    // Lee cada linea del archivo CSV hasta el final
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        Escenario* nuevoEscenario = crearEscenario(campos) ;
        if (nuevoEscenario != NULL) {
            list_pushBack(escenarios, nuevoEscenario) ;
        }
    }
    fclose(archivo) ; // Cierra el archivo después de leer todas las lineas
}

void mostrarEstadoJugador(){

}

int main() {
    List* escenarios = list_create() ;
    cargadeEscenarios(escenarios) ;
    //Prueba sobre que se ve
    for (Escenario* e = list_first(escenarios); e != NULL; e = list_next(escenarios)) {
        printf("Escenario %d: %s\n", e->ID, e->nombre);
        printf("  %s\n", e->descripcion);
        printf("  Ítems:\n");
        for (Item* it = list_first(e->items); it != NULL; it = list_next(e->items)) {
            printf("    - %s (valor: %d, peso: %d)\n", it->nombre_Item, it->valor, it->peso);
        }
        puts("") ;
    }

    return 0;
}
