#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/extra.h"

typedef struct { //estructura sobre los datos de los items encontrados en cada escenario
    char* nombreItem ;
    int pesoItem ;
    int valorItem ;
} Item ;
typedef struct { //estructura que tiene los datos de un escenario
    int ID ;
    char* nombreEscenario ;
    char* descripcion ; //explica el como se ve el escenario 
    List* items ; //contiene los items que se encuentran en el escenario
    int arriba, abajo, izquierda, derecha ; //movimientos posibles
    int esFinal ;
} Escenario ;
typedef struct { //estructura que contiene los datos del jugador
    Escenario* actual ; //escenario en el que se encuentra el jugador
    List* inventario ; //lista de items que ha recogido el jugador
    float tiempoRestante ; //tiempo que le queda al jugador
    int puntajeAcumulado ;
    int pesoTotal ; //suma del peso de los items que tenga el jugador
} Jugador ;

Item* crearItem(char* nombre, int valor, int peso) { //reserva memoria para guardar (crear) un nuevo item
    Item* nuevito = malloc(sizeof(Item)) ;
    nuevito->nombreItem = strdup(nombre) ;
    nuevito->valorItem = valor ;
    nuevito->pesoItem = peso ;
    return nuevito ;
}

int compararEnteros(void* a, void* b) { //compara dos claves 'int' almacenadas en un mapa para que este pueda buscar, insertar o eliminar elementos de manera correcta
    return *(int*)a == *(int*)b ;
}
/* lee cada dato sobre los escenarios desde el archivo "graphquest.csv" y los almacena en un mapa*/
void lecturaEscenarios(Map *escenarios) { 
    FILE *archivo = fopen("graphquest.csv", "r") ; //abre el archivo
    if (archivo == NULL) {
        perror("Error al abrir el archivo") ;
        return ;}

    fgets((char[256]){0}, 256, archivo) ;// Leer la cabecera (linea 1) y la descarta (ignora en resumen)
    char **campos ;
    while ((campos = leer_linea_csv(archivo, ',')) != NULL)
    {//procesa datos de los escenarios
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

void mostrarEstadoJugador(Jugador* jugador) { //muestra la informacion actual del jugador (osea cada vez que hace algun cambio)
    Escenario* escenario = jugador->actual ;
    printf("\n===== ESTADO ACTUAL DEL JUGADOR =====\n") ;
    printf("     Escenario: %s\n", escenario->nombreEscenario) ;
    printf("     Descripcion: %s\n", escenario->descripcion) ;
     
    printf("\n     Inventario :\n") ;
    if (list_size(jugador->inventario) == 0) {
        printf("     ~~~ No hay items en el inventario ~~~\n") ; 
    }
    else{ //muestra toodos los items que tenga el jugador en el inventario
        for (Item* item = list_first(jugador->inventario) ; item != NULL ; item = list_next(jugador->inventario)) {
            printf(" * Nombre del Item: %s, Peso: %d [kg], Valor: %d [pts]\n", item->nombreItem, item->pesoItem, item->valorItem) ;
        }
    }
    printf("\n     ~ Tiempo restante: %.1f\n", jugador->tiempoRestante) ;   
    printf("     ~ Peso total: %d [kg]\n", jugador->pesoTotal) ;
    printf("     ~ Puntaje acumulado: %d [pts]\n\n", jugador->puntajeAcumulado) ;

    printf("     Items disponibles en el escenario:\n") ; //muestra los items del escenario en el momento
    Item* item = list_first(escenario->items) ;
    if (item == NULL) printf("    ...No hay items en este escenario...\n") ;
    else{ //arreglado, ahora imprime todos los items del escenario
        while (item != NULL){
            printf("        * Nombre del Item: %s, Peso: %d [kg], Valor: %d [pts]\n", item->nombreItem, item->pesoItem, item->valorItem) ;
            item = list_next(escenario->items) ;
        }
    } //muestra las posibles acciones que puede realizar el jugador
    printf("\nACCIONES DISPONIBLES:\n") ;
    printf(" Movimientos:\n") ;
    if (escenario->arriba != -1) printf("  - Moverse hacia ARRIBA \n") ;
    if (escenario->abajo != -1) printf("  - Moverse hacia ABAJO \n") ;
    if (escenario->izquierda != -1) printf("  - Moverse hacia IZQUIERDA \n") ;
    if (escenario->derecha != -1) printf("  - Moverse hacia DERECHA \n") ;
    if (list_first(escenario->items) != NULL) printf("  - Recoger item\n") ;
    else if (list_first(jugador->inventario) != NULL) printf("  - Descartar item\n") ;
    printf("  - Reiniciar partida\n") ;
    printf("  - Salir del juego\n") ;
    printf("=====================================\n\n") ;
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
    printf("Seleccione el item a recoger:\n") ;
    int opcion ;
    scanf("%d", &opcion) ;
    if (opcion < 1 || opcion > list_size(itemsJugador)) {
        printf("Seleccion invalida...\n") ;
        return ;}
    int posicion = 1 ; //agrega el item al inventario y lo elimina del escenario
    for (Item* item =  list_first(itemsJugador) ; item != NULL ; item = list_next(itemsJugador)) {
        if (posicion == opcion) {
            list_pushBack(jugador->inventario, item) ;
            jugador->pesoTotal += item->pesoItem ; //suma el peso de los objetos (lo mas seguro en kilos)
            jugador->puntajeAcumulado += item->valorItem ; //suma los valores de los objetos (lo cuento en puntos)
            list_popCurrent(itemsJugador) ;//elimina el item del escenario
            jugador->tiempoRestante -= 1.0 ; //cada item recogido consume 1 (segundo?)
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
    int k = 1 ; //muestra los posibles objetos que el jugador puede dejar
    for (Item* item = list_first(jugador->inventario) ; item != NULL ; item = list_next(jugador->inventario)) {
        printf(" %d. Nombre del Item: %s, Peso: %d [kg], Valor: %d [pts]\n", k, item->nombreItem, item->pesoItem, item->valorItem) ;
        k++ ;}
    printf("Seleccione items a descartar:\n") ;
    int opcion ;
    scanf("%d", &opcion) ;
    if (opcion < 1 || opcion > list_size(jugador->inventario)) {
        printf("Seleccion invalida...\n") ;
        return ;}
    int indice = 1 ; //elimina el item que se haya seleccionado
    for (Item* item = list_first(jugador->inventario) ; item != NULL ; item = list_next(jugador->inventario)) {
        if (indice == opcion) { //quita el objeto de la lista creada para guardar los objetos en el inventario
            jugador->pesoTotal -= item->pesoItem ;
            jugador->puntajeAcumulado -= item->valorItem ;
            printf("Haz descartado el item %s\n", item->nombreItem) ;
            list_popCurrent(jugador->inventario) ;
            break ;
        }
        indice++ ;
    }
    
}
//funciona para que el jugador pueda moverse entre escenarios
void moverJugador(Jugador* jugador, Map* grafo, char direccion) { 
    int idNulo = -1 ;
    if (direccion == 'w') idNulo = jugador->actual->arriba ;
    else if (direccion == 's') idNulo = jugador->actual->abajo ;
    else if (direccion == 'a') idNulo = jugador->actual->izquierda ;
    else if (direccion == 'd') idNulo = jugador->actual->derecha ;
    if (idNulo == -1){
        printf("No puedes moverte en esa direccion\n") ;
        return ;}
    
    MapPair* parcito = map_first(grafo) ; //busca el siguiente escenario al que puede ir el jugador
    while (parcito) {
        int* llave = parcito->key ;
        if (*llave == idNulo) {
            jugador->actual = parcito->value ;
            printf("Te has movido a: \"%s\"\n", jugador->actual->nombreEscenario) ;
            
            int pesoTotal_aux = 0 ;
            for (Item* item = list_first(jugador->inventario) ; item != NULL ; item = list_next(jugador->inventario)) {
                pesoTotal_aux += item->pesoItem ;
            } //toda esta parte sirve para calcular el tiempo que consume el jugador mientras mas peso lleve encima
            float tiempoConsumido = (pesoTotal_aux + 1) / 10.0 ;
            jugador->tiempoRestante -= tiempoConsumido ; //cada movimiento consume 1 segundo por cada kg de peso total
            printf("Tiempo Consumido : %.1f \n", tiempoConsumido) ;

            if (jugador->actual->esFinal == 1){ //verifica si el jugador llega al escenario final
                printf(" HAS LLEGADO A LA SALIDA!\n") ;
                printf("  JUEGO TERMINADO!\n") ;
                printf("  Tu puntaje final fue de: %d [pts]\n", jugador->puntajeAcumulado) ;
                printf("  Tiempo restante: %.1f\n", jugador->tiempoRestante) ;
                exit(0) ; //termina el juego inmediatamente si se llega al escenario final
            }
            return ;
        }
        parcito = map_next(grafo) ;
    }
    printf("No se encontro el escenario....\n") ;
}  //mueve al jugador a un nuevo escenario
 
Jugador* crearJugador(Map* grafo) { //crea un nuevo jugador e inicializa su estado, osea que todos sus datos estan en 0
    Jugador* jugador = malloc(sizeof(Jugador)) ;
    jugador->actual = ((MapPair*)map_first(grafo))->value ; //inicia en el primer escenario
    jugador->inventario = list_create() ;//inventario vacio
    jugador->tiempoRestante = 10 ;
    jugador->puntajeAcumulado = 0 ;
    jugador->pesoTotal = 0 ;
    return jugador ;
}

void liberarMemoria(Jugador* player, Map* grafo){ //libera toda la momoria usada por el jugador y el grafo
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
    //crea el grafo principal que contiene los escenarios usando la funcion de comparacion para IDs
    Map* grafo = map_create(compararEnteros) ; 
    if (grafo == NULL){
        printf("ERROR EN ARCHIVOS DE LECTURA... INTENTE MAS TARDE") ;
        return ;
    }
    lecturaEscenarios(grafo) ; //carga los escenarios desde el archivo CSV al grafo
    Jugador* player = crearJugador(grafo) ; //crea al jugador y lo inicializa

    int salir = 0 ;
    while (!salir) {
        mostrarEstadoJugador(player) ; //meustra el estado actual del jugador: items, puntaje, ubicacion.. etc
        puts("Selecciona una opcion:") ; //menu de opciones
        puts("1. Recoger Item") ;
        puts("2. Descartar Item") ;
        puts("3. Avanzar") ;
        puts("4. Reiniciar Partida") ;
        puts("5. Salir del Juego") ;

        int opcion ;
        scanf("%d", &opcion) ;
        switch (opcion) {
            case 1: 
                system("cls||clear") ; //este limpia la pantalla :)
                recogerItem(player) ; //agrega items del escenario en el que se encuentra el juagdor
                break ;
            case 2: 
                system("cls||clear") ;
                descartarItem(player) ; //elimina items del inventario
                break ;
            case 3: 
                system("cls||clear") ;
                printf("Indica direccion (w/a/s/d): ") ;
                char direccion ;
                scanf(" %c", &direccion) ;
                moverJugador(player, grafo, direccion) ; //hace que el jugador se pueda mover por el "mapa"
                break ;
            case 4:
                system("cls||clear") ;
                printf("\n ~~~~~ REINICIANDO PARTIDA ~~~~~ \n") ;
                player = crearJugador(grafo) ; //reincia al jugador, todo desde 0
                break ;
            case 5:
                system("cls||clear") ;
                printf("GRACIAS POR JUGAR!. Tu puntaje fue de: %d [pts]\n\n", player->puntajeAcumulado) ;
                salir = 1 ; //hace que termine el juego
                break ;
            default:
                system("cls||clear") ;
                puts("Reintente Nuevamente....") ; //por si se selecciona una opcion no valida
        }
        if (player->tiempoRestante <= 0 ) { //verifica si se acabo el tiempo
            printf("SE ACABO EL TIEMPO!!!. Fin del Juego\n") ;
            printf("Tu puntaje final fue de: %d [pts]\n\n", player->puntajeAcumulado) ;
            salir = 1 ;
        }
        if (player->actual->esFinal){ //verifica si se llego al final
            printf("\n ~~~ FELICIDADES! ~~~ . \n") ;
            printf("Llegaste al escenario final\n") ;
            printf("Tu puntaje final fue de: %d [pts]\n\n", player->puntajeAcumulado) ;
            salir = 1 ;
        }
    }
    liberarMemoria(player, grafo) ; //libera toda la memoria usada
}

int main() {
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ BIENVENIDO A GRAPHQUEST ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n") ;
    jugar() ; //inicia el juego
    return 0 ; //termina el programa
}
