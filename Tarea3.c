#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/extra.h"

typedef struct {
    char* nombreItem ;
    int pesoItem ;
    int valorItem ;
} Item ;
typedef struct {
    int ID ;
    char* nombreEscenario ;
    char* descripcion ;
    List* items ;
    int arriba, abajo, izquierda, derecha ; //movimientos posibles
    int esFinal ;
} Escenario ;
typedef struct {
    Escenario* actual ; //escenario en el que se encuentra el jugador
    List* inventario ; //lista de items que ha recogido el jugador
    int tiempoRestante ;
    int puntajeAcumulado ;
    int pesoTotal ;
} Jugador ;

Item* crearItem(char* nombre, int valor, int peso) {
    Item* nuevito = malloc(sizeof(Item)) ;
    nuevito->nombreItem = strdup(nombre) ;
    nuevito->valorItem = valor ;
    nuevito->pesoItem = peso ;
    return nuevito ;
}

int compararEnteros(void* a, void* b) { //Listo
    return *(int*)a == *(int*)b ;
}

void lecturaEscenarios(Map *escenarios) {
    FILE *archivo = fopen("graphquest.csv", "r") ;
    if (archivo == NULL) {
        perror("Error al abrir el archivo") ;
        return ;}

    fgets((char[256]){0}, 256, archivo) ;// Leer la cabecera (línea 1) y descartarla
    char **campos ;
    while ((campos = leer_linea_csv(archivo, ',')) != NULL)
    {
        int id = atoi(campos[0]) ;
        Escenario *nuevoEscenario = malloc(sizeof(Escenario)) ;
        nuevoEscenario->ID = id ;
        nuevoEscenario->nombreEscenario = strdup(campos[1]) ;
        nuevoEscenario->descripcion = strdup(campos[2]) ;
        nuevoEscenario->items = list_create() ;
        nuevoEscenario->esFinal = atoi(campos[8]) ;

        List *items = split_string(campos[3], ";") ;
        for (char *itemsito = list_first(items); itemsito != NULL; itemsito = list_next(items)) {
            List *valores = split_string(itemsito, ",") ;
            char *nombreItem = strdup(list_first(valores)) ;
            int valor = atoi(list_next(valores));
            int peso = atoi(list_next(valores));

            Item *nuevo_item = crearItem(nombreItem, valor, peso) ;
            list_pushBack(nuevoEscenario->items, nuevo_item) ;

            list_clean(valores) ;
            free(valores) ;
        }
        list_clean(items) ;
        free(items) ;

        nuevoEscenario->arriba = atoi(campos[4]) ;
        nuevoEscenario->abajo = atoi(campos[5]) ;
        nuevoEscenario->izquierda = atoi(campos[6]) ;
        nuevoEscenario->derecha = atoi(campos[7]) ;

        int *key = malloc(sizeof(int)) ;
        *key = id;
        map_insert(escenarios, key, nuevoEscenario) ;
    }

    fclose(archivo) ;
    printf("                                Escenarios leidos...\n") ;
}

void mostrarEstadoJugador(Jugador* jugador) {
    Escenario* escenario = jugador->actual ;
    printf("\n===== ESTADO ACTUAL DEL JUGADOR =====\n") ;
    printf("     Escenario: %s\n", escenario->nombreEscenario) ;
    printf("     Descripcion: %s\n", escenario->descripcion) ;
     
    printf("\n     Inventario :\n") ;
    if (list_size(jugador->inventario) == 0) {
        printf("     ~~~ No hay items en el inventario ~~~\n") ; 
    }
    else{
        for (Item* item = list_first(jugador->inventario) ; item != NULL ; item = list_next(jugador->inventario)) {
            printf(" * Nombre del Item: %s, Peso: %d [kg], Valor: %d [pts]\n", item->nombreItem, item->pesoItem, item->valorItem) ;
        }
    }
    printf("\n     ~ Tiempo restante: %d\n", jugador->tiempoRestante) ;   
    printf("     ~ Peso total: %d [kg]\n", jugador->pesoTotal) ;
    printf("     ~ Puntaje acumulado: %d [pts]\n\n", jugador->puntajeAcumulado) ;

    printf("     Items disponibles en el escenario:\n") ;
    Item* item = list_first(escenario->items) ;
    if (item == NULL) printf("    ...No hay items en este escenario...\n") ;
    else{
        printf("        * Nombre del Item: %s, Peso: %d [kg], Valor: %d [pts]\n", item->nombreItem, item->pesoItem, item->valorItem) ;
    }
    printf("\nACCIONES DISPONIBLES:\n") ;
    printf(" Movimientos:\n") ;
    if (escenario->arriba != -1) printf("  - Moverse hacia ARRIBA \n") ;
    if (escenario->abajo != -1) printf("  - Moverse hacia ABAJO \n") ;
    if (escenario->izquierda != -1) printf("  - Moverse hacia IZQUIERDA \n") ;
    if (escenario->derecha != -1) printf("  - Moverse hacia DERECHA \n") ;
    printf("  - Recoger item\n") ;
    printf("  - Salir del juego\n") ;
    printf("=====================================\n\n") ;
}

int calcularTiempoRestante(int pesoTotal) {
    return (pesoTotal + 1) / 10 ;
}

void recogerItem(Jugador* jugador) { //recoge un item del escenario y lo agrega al inventario
    List* itemsJugador = jugador->actual->items ;
    if (list_size(itemsJugador) == 0){
        printf("No hay items en el escenario\n") ;
        return ;}
    printf("\nItems disponibles:\n") ;
    int k = 1 ;
    for (Item* item = list_first(itemsJugador) ; item != NULL ; item = list_next(itemsJugador)) {
        printf(" %d. Nombre del Item: %s, Peso: %d [kg], Valor: %d [pts]\n", k, item->nombreItem, item->pesoItem, item->valorItem) ;
        k++ ;}
    printf("Seleccione la cantidad de items a recoger:\n") ;
    int opcion ;
    scanf("%d", &opcion) ;
    if (opcion < 1 || opcion > list_size(itemsJugador)) {
        printf("Seleccion invalida...\n") ;
        return ;}
    int posicion = 1 ;
    for (Item* item =  list_first(itemsJugador) ; item != NULL ; item = list_next(itemsJugador)) {
        if (posicion == opcion) {
            list_pushBack(jugador->inventario, item) ;
            jugador->pesoTotal += item->pesoItem ;
            jugador->puntajeAcumulado += item->valorItem ;
            list_popCurrent(itemsJugador) ;//elimina el item del escenario
            jugador->tiempoRestante-- ;
            printf("Haz recogido el item %s\n", item->nombreItem) ;
            break ;
        }
        posicion++ ;
    }
}

void descartarItem(Jugador* jugador) { //descarta un item del inventario del jugador
    printf("Items en tu inventario:\n") ;
    if (list_size(jugador->inventario) == 0){
        printf("No tienes items en el inventario\n") ;
        return ;}
    int k = 1 ;
    for (Item* item = list_first(jugador->inventario) ; item != NULL ; item = list_next(jugador->inventario)) {
        printf(" %d. Nombre del Item: %s, Peso: %d [kg], Valor: %d [pts]\n", k, item->nombreItem, item->pesoItem, item->valorItem) ;
        k++ ;}
    printf("Seleccione cantidad de items a descartar:\n") ;
    int opcion ;
    scanf("%d", &opcion) ;
    if (opcion < 1 || opcion > list_size(jugador->inventario)) {
        printf("Seleccion invalida...\n") ;
        return ;}
    int indice = 1 ;
    for (Item* item = list_first(jugador->inventario) ; item != NULL ; item = list_next(jugador->inventario)) {
        if (indice == opcion) {
            jugador->pesoTotal -= item->pesoItem ;
            jugador->puntajeAcumulado -= item->valorItem ;
            printf("Haz descartado el item %s\n", item->nombreItem) ;
            list_popCurrent(jugador->inventario) ;
            break ;
        }
        indice++ ;
    }
    
}

void moverJugador(Jugador* jugador, Map* grafo, char direccion) {
    int idNulo = -1 ;
    if (direccion == 'w') idNulo = jugador->actual->arriba ;
    else if (direccion == 's') idNulo = jugador->actual->abajo ;
    else if (direccion == 'a') idNulo = jugador->actual->izquierda ;
    else if (direccion == 'd') idNulo = jugador->actual->derecha ;
    if (idNulo == -1){
        printf("No puedes moverte en esa direccion\n") ;
        return ;}
    
    MapPair* parcito = map_first(grafo) ;
    while (parcito) {
        int* llave = parcito->key ;
        if (*llave == idNulo) {
            jugador->actual = parcito->value ;
            printf("Te has movido a: \"%s\"\n", jugador->actual->nombreEscenario) ;
            jugador->tiempoRestante-- ;
            return ;
        }
        parcito = map_next(grafo) ;
    }
    printf("No se encontro el escenario....\n") ;
}  //mueve al jugador a un nuevo escenario
 
Jugador* crearJugador(Map* grafo) {
    Jugador* jugador = malloc(sizeof(Jugador)) ;
    jugador->actual = ((MapPair*)map_first(grafo))->value ; //inicia en el primer escenario
    jugador->inventario = list_create() ;
    jugador->tiempoRestante = 10 ;
    jugador->puntajeAcumulado = 0 ;
    jugador->pesoTotal = 0 ;
    return jugador ;
}

void liberarMemoria(Jugador* player, Map* grafo){
     list_clean(player->inventario) ;
    free(player->inventario) ;
    free(player) ;

    MapPair* par = map_first(grafo) ;
    while (par != NULL) {
        Escenario* esc = par->value ;
        list_clean(esc->items) ;
        free(esc->items) ;
        free(esc->nombreEscenario) ;
        free(esc->descripcion) ;
        free(esc) ;
        free(par->key) ;
        par = map_next(grafo) ;
    }
    free(grafo) ;
}

void jugar() { //mete todas las funciones para formar el juego
    printf("                                Cargando escenarios...\n") ;
    Map* grafo = map_create(compararEnteros) ;
    if (grafo == NULL){
        printf("ERROR EN ARCHIVOS DE LECTURA... INTENTE MAS TARDE") ;
        return ;
    }
    lecturaEscenarios(grafo) ;
    Jugador* player = crearJugador(grafo) ;

    int salir = 0 ;
    while (!salir) {
        mostrarEstadoJugador(player) ;
        puts("Selecciona una opcion:") ;
        puts("1. Recoger Item") ;
        puts("2. Descartar Item") ;
        puts("3. Avanzar") ;
        puts("4. Reiniciar Partida") ;
        puts("5. Salir del Juego") ;

        int opcion ;
        scanf("%d", &opcion) ;
        switch (opcion) {
            case 1: 
                system("cls||clear") ;
                recogerItem(player) ;
                break ;
            case 2: 
                system("cls||clear") ;
                descartarItem(player) ;
                break ;
            case 3: 
                system("cls||clear") ;
                printf("Indica direccion (w/a/s/d)") ;
                char direccion ;
                scanf(" %c", &direccion) ;
                moverJugador(player, grafo, direccion) ;
                break ;
            case 4:
                system("cls||clear") ;
                printf("\n ~~~~~ REINICIANDO PARTIDA ~~~~~ \n") ;
                player = crearJugador(grafo) ;
                break ;
            case 5:
                system("cls||clear") ;
                printf("GRACIAS POR JUGAR!. Tu puntaje fue de: %d\n\n", player->puntajeAcumulado) ;
                salir = 1 ;
                break ;
            default:
                system("cls||clear") ;
                puts("Reintente Nuevamente....") ;
        }
        if (player->tiempoRestante <= 0) {
            printf("SE ACABO EL TIEMPO!!!. Fin del Juego") ;
            salir = 1 ;
        }
        if (player->actual->esFinal){
            printf("\n ~~~ FELICIDADES! ~~~ . \n") ;
            printf("Llegaste al escenario final") ;
            salir = 1 ;
        }
    }
    liberarMemoria(player, grafo) ;
}

int main() {
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ BIENVENIDO A GRAPHQUEST ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n") ;
    jugar() ;
    return 0 ;
}
