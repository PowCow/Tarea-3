# Tarea-3


# GRAPHQUEST: Juego interactivo utilizando grafos
**DESCRIPCIÓN**

GraphQuest es una aplicacion por consola basada en un sistema de grafos, donde cada nodo representa un escenario dentro de una historia interactiva. El jugador inicia en un nodo inicial y va tomando decisiones para moverse por el grafo, recolectar ítems y avanzar en la historia. El objetivo es llegar al nodo final en el menor tiempo posible, usando las conexiones entre nodos de forma eficiente.

Cada escenario tiene posibles direcciones (arriba, abajo, izquierda, derecha), y puede contener objetos utiles que el jugador guarda en su inventario. El programa carga el grafo desde un archivo CSV, interpretando los nodos, conexiones e items disponibles.

* ¿Como compilar y ejecutar el programa?
  
   Posibles requisitos:
   - Tener un sistema operativo Windows que contenga un compilador C
   - Tener instalado gcc o algun compilador de C commpatible
   - Tener el archivo "graphquest.csv" en la misma carpeta del archivo ejecutable del programa
     
  Se debe descargar la carpeta del programa "GraphQuest", esta contiene el ejecutable y el archivo .CSV para que el programa funcione.
 Adjunto Link Drive del programa:
https://drive.google.com/drive/folders/1_gESC8QMexLxsG7F0sLQPZpWkezT9cj8?usp=sharing

  Al entrar al drive se debe seleccionar la carpeta completa, hacer click derecho y elegir "Descargar",
  Google Drive creara un ZIP y empezara a descargar el archivo, para ejecutarlo debe extraer la carpeta
  del archivo ZIP Y abrir desde ahi, el archivo .exe. Si este se encuentra con el .csv debe abrirse
  correctamente y mostrar directamente el "menu" del juego.

* Opciones que funcionan correctamente y las que no (con sus posibles causas)
   Funcionan correctamente:
     - Carga correctamente el archivo .CSV con todos sus datos
     - Menu se muestra correctamente, tambien se muestra de manera correcta el estado actual del jugador
     - Muestra de manera correcta las acciones posibles que puede ejecutar el jugador
     - Las funciones para "Recoger Items, Descartar Items, Mover Jugador y Jugar" funcionan correctamente, los requisitos que se les pide
     - Se libera memoria correctamente, tanto como del jugador como de los mapas usados y las listas
     - Funciona correctamente los calculos sobre el tiempo, cuando el jugador toma un objeto (que se le descuenta 1) o cuando este se mueve entre escenarios, toma en cuenta cuando lleva peso y cuando no.
     - "Elimina" correctamente los objetos del escenario cuando el jugador los toma
 
   Problemas conocidos:
     - Al llegar a la "SALIDA" el programa no la detecta como salida, entonces no termina su ejecucion mostrando su pantalla final, meustra como si se pudiera continuar jugando, lo cual es erroneo. Toca salirse del programa apretando la opcion 5. "Salir del juego"
     - Al llegar a la "SALIDA" muestra una opcion para moverse a la DERECHA, no funciona porque despues de la salida no hay mas camino para tomar, ademas, no logra saltarse la "linea" de los item en la seccion de salida, la cual no contiene ningun item, es decir, lee los datos de "arriba, abajo e izquierda" como variables del item, siendo que estas variables no existen
     - Se podria especificar de mejor manera que las teclas (w/a/s/d) son para moverse en las direcciones, arriba, izquierda, abajo y derecha
     - El programa no verifica si hay nombres duplicados de items, se asume que no se repiten objetos
     - Al ingresar una tecla aleatoria en el menu de "AVANCE", el programa puede repetir mensajes innecesarios o mostrar de manera errornea o movida los mensajes
     - No se implemento una opcion para guardar y cargar partida de jugadores
     - Al apretar en el ejecutable "Salir del juego" este se sale inmediatamente, sin mostrar los datos obtenidos por el jugador (creo que es por el orden en el que estan colocadas las variables)
       
 * Ejemplo interactivo con la aplicacion
  Se muestra la pantalla del menu

       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ BIENVENIDO A GRAPHQUEST ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                      Cargando escenarios...
                                      Escenarios leidos...
   
       ===== ESTADO ACTUAL DEL JUGADOR =====
       Escenario: Entrada principal
       Descripcion: Una puerta rechinante abre paso a esta mansion olvidada por los dioses y los conserjes. El aire huele a humedad y a misterios sin resolver.

       Inventario :
       ~~~ No hay items en el inventario ~~~

       ~ Tiempo restante: 10.0
       ~ Peso total: 0 [kg]
       ~ Puntaje acumulado: 0 [pts]

       Items disponibles en el escenario:
       ...No hay items en este escenario...

       ACCIONES DISPONIBLES:
       Movimientos:
        - Moverse hacia ABAJO
        - Reiniciar partida
        - Salir del juego
       =====================================
       
       Selecciona una opcion:
       1. Recoger Item
       2. Descartar Item
       3. Avanzar
       4. Reiniciar Partida
       5. Salir del Juego

Seleccionamos la opcion 3. Avanzar

    Indica direccion (w/a/s/d): s
Indicamos la "s", nos movemos hacia abajo

     Indica direccion (w/a/s/d): s
     Te has movido a: "Cocina"
     Tiempo Consumido : 0.1
      
    ===== ESTADO ACTUAL DEL JUGADOR =====
         Escenario: Cocina
         Descripcion: Restos de una batalla culinaria. Cuchillos por aqui, migajas por alla. Alguien cocinoÔÇª o fue cocinado.

     Inventario :
     ~~~ No hay items en el inventario ~~~

     ~ Tiempo restante: 9.9
     ~ Peso total: 0 [kg]
     ~ Puntaje acumulado: 0 [pts]

     Items disponibles en el escenario:
        * Nombre del Item: Cuchillo, Peso: 1 [kg], Valor: 3 [pts]
        * Nombre del Item: Pan, Peso: 1 [kg], Valor: 2 [pts]

     ACCIONES DISPONIBLES:
      Movimientos:
       - Moverse hacia ARRIBA
       - Moverse hacia ABAJO
       - Moverse hacia DERECHA
       - Recoger item
       - Reiniciar partida
       - Salir del juego
     =====================================
     
     Selecciona una opcion:
     1. Recoger Item
     2. Descartar Item
     3. Avanzar
     4. Reiniciar Partida
     5. Salir del Juego

Escogemos la opcion 1. Recoger Item
    
    Items disponibles:
     1. Nombre del Item: Cuchillo, Peso: 1 [kg], Valor: 3 [pts]
     2. Nombre del Item: Pan, Peso: 1 [kg], Valor: 2 [pts]
    Seleccione el item a recoger:
Seleccionamos la opcion 1. el cuchillo

    Seleccione el item a recoger:
    1
    Haz recogido el item Cuchillo
    
    ===== ESTADO ACTUAL DEL JUGADOR =====
         Escenario: Cocina
         Descripcion: Restos de una batalla culinaria. Cuchillos por aqui, migajas por alla. Alguien cocinoÔÇª o fue cocinado.
    
         Inventario :
     * Nombre del Item: Cuchillo, Peso: 1 [kg], Valor: 3 [pts]
    
         ~ Tiempo restante: 8.9
         ~ Peso total: 1 [kg]
         ~ Puntaje acumulado: 3 [pts]
    
         Items disponibles en el escenario:
            * Nombre del Item: Pan, Peso: 1 [kg], Valor: 2 [pts]
    
    ACCIONES DISPONIBLES:
     Movimientos:
      - Moverse hacia ARRIBA
      - Moverse hacia ABAJO
      - Moverse hacia DERECHA
      - Recoger item
      - Reiniciar partida
      - Salir del juego
    =====================================
    
    Selecciona una opcion:
    1. Recoger Item
    2. Descartar Item
    3. Avanzar
    4. Reiniciar Partida
    5. Salir del Juego
Escogemos denuevo la opcion 3. Avanzar, nos dirigiremos hacia la derecha

    Indica direccion (w/a/s/d): d
    Te has movido a: "Comedor"
    Tiempo Consumido : 0.2
    
    ===== ESTADO ACTUAL DEL JUGADOR =====
         Escenario: Comedor
         Descripcion: Una mesa elegante cubierta de polvo. Parece que nadie ha cenado aqui desde la ultima plaga. Al menos dejaron una copa.
    
         Inventario :
     * Nombre del Item: Cuchillo, Peso: 1 [kg], Valor: 3 [pts]
    
         ~ Tiempo restante: 8.7
         ~ Peso total: 1 [kg]
         ~ Puntaje acumulado: 3 [pts]
    
         Items disponibles en el escenario:
            * Nombre del Item: Copa dorada, Peso: 3 [kg], Valor: 8 [pts]
    
    ACCIONES DISPONIBLES:
     Movimientos:
      - Moverse hacia ARRIBA
      - Moverse hacia ABAJO
      - Moverse hacia IZQUIERDA
      - Moverse hacia DERECHA
      - Recoger item
      - Reiniciar partida
      - Salir del juego
    =====================================
Repetimos como antes, seleccionaremos la opcion 1. Recoger Item, para poder guardar la "Copa Dorada"

        Inventario :
    * Nombre del Item: Cuchillo, Peso: 1 [kg], Valor: 3 [pts]
    * Nombre del Item: Copa dorada, Peso: 3 [kg], Valor: 8 [pts]
   
        ~ Tiempo restante: 7.7
        ~ Peso total: 4 [kg]
        ~ Puntaje acumulado: 11 [pts]
   
        Items disponibles en el escenario:
       ...No hay items en este escenario...
En nuestro inventario se verá que tendremos dos items, veremos cuanto tiempo nos queda, para seguir buscando la salida, ademas del peso que llevamos con los objetos que hemos seleccionado, y el puntaje que estos objetos nos entregan

Si seleccionamos la opcion 2. Descartar Items

    Items en tu inventario:
     1. Nombre del Item: Cuchillo, Peso: 1 [kg], Valor: 3 [pts]
     2. Nombre del Item: Copa dorada, Peso: 3 [kg], Valor: 8 [pts]
    Seleccione items a descartar:
Podemos elegir uno de los items para soltar, esto nos puede ayudar si necesitamos llegar mas "rapido" a otro escenario o si tenemos muchos objetos que nos ponen mucho peso encima

En este ejemplo, digamos que el jugador se aburre y aprieta la opcion 5. Salir del Juego
Si se ha abierto el programa desde su version .exe, cerrara automaticamente la ventana que se estaba usando, en cambio, si se compilara el programa desde VS code u otro programa para compilar en C, este mostraria los puntajes obtenidos a la ultima instancia del jugador, mostrando algo como 

      GRACIAS POR JUGAR!. Tu puntaje fue de: 11 [pts]

Y luego de esto cerraria el programa.
