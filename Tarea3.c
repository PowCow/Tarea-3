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
    Item items ;
    int arriba ;
    int abajo ;
    int izquierda ; 
    int derecha ;
    int es_final ;
} Escenario ;

void leer_escenarios(Escenario *escenarios, int *cantEscenarios) { //carga canciones del archivo csv
    
    FILE *archivo = fopen("graphquest.csv", "r") ;
    if (archivo == NULL) {
        perror("Error al abrir el archivo") ; // Informa si el archivo no puede abrirse
        return;
    }
    char **campos ;
    campos = leer_linea_csv(archivo, ',') ;// Lee los encabezados del CSV

    // Lee cada línea del archivo CSV hasta el final
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {

        Escenario *lugar = &escenarios[*cantEscenarios] ; 
        lugar->ID = atoi(campos[0]) ;
        





        printf("ID: %d\n", atoi(campos[0])) ;
        printf("Nombre: %s\n", campos[1]) ;
        printf("Descripcion: %s\n", campos[2]) ;

        List* items = split_string(campos[3], ";") ;

        printf("Items: \n");
        for(char *item = list_first(items) ; item != NULL ; item = list_next(items)) {
            List* values = split_string(item, ",") ;
            char* item_name = list_first(values) ;
            int item_weight = atoi(list_next(values)) ;
            int item_value = atoi(list_next(values)) ;
            printf("  - %s (%d pts, %d kg)\n", item_name, item_value, item_weight) ;
            list_clean(values);
            free(values) ;
        }

        int arriba = atoi(campos[4]) ;
        int abajo = atoi(campos[5]) ;
        int izquierda = atoi(campos[6]) ; 
        int derecha = atoi(campos[7]) ;

        if (arriba != -1) printf("Arriba: %d\n", arriba) ;
        if (abajo != -1) printf("Abajo: %d\n", abajo) ;
        if (izquierda != -1) printf("Izquierda: %d\n", izquierda) ;
        if (derecha != -1) printf("Derecha: %d\n", derecha) ;

        int is_final = atoi(campos[8]) ;
        if (is_final) printf("Es final\n") ;
        puts("") ; // Imprime una linea en blanco para separar los escenarios

        list_clean(items) ;
        free(items) ;
    }
    fclose(archivo) ; // Cierra el archivo después de leer todas las líneas
}












int main() {
    Escenario escenario ;
    int cantidad_escenarios = 0 ;

    leer_escenarios(escenario, &cantidad_escenarios) ;
    mostrarEscenarios(escenario, cantidad_escenarios) ;

    return 0;
}
