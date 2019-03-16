#include "stdio.h"      // Libreria para interaccion con dispositivos E/S
#include "stdlib.h"     // Libreria estandar de c (Tipos y funciones basicas)
#include "string.h"     // Libreria para manejo de cadenas
#include <sys/types.h>  // Libreria para definiciones de algunas estructuras requeridas
#include <sys/socket.h> // Libreria de sockets
#include <netinet/in.h> // Libreria de protocolos y estandares (Familias de protocolos)
#include <unistd.h>     // Libreria de tipos y constantes simbólicas estándar
#include "netdb.h"      // Libreria para definiciones de operaciones de base de datos de red
#include "arpa/inet.h"  // Libreria para definiciones de operaciones de internet

int main(int argc, char **argv)
{
    int port; // Puerto a ser utilizado
    int socketCliente = socket(AF_INET, SOCK_STREAM, 0); // Definicion del socket

    port = 8080; // Puerto por defecto
    if (argc > 1)
    {
        port = atoi(argv[1]); // Puerto establecido por el usuario
    }

    // asignacion de datos a la estructura deñ cliente
    struct sockaddr_in direccionSocket;
    direccionSocket.sin_family = AF_INET;
    direccionSocket.sin_port = htons(port);
    direccionSocket.sin_addr.s_addr = INADDR_ANY;
    // fin de asignacion de datos a la estructura deñ cliente

    connect(socketCliente, (struct sockaddr *)&direccionSocket, sizeof(direccionSocket)); // Estcablecimiento de la conexion

    char respuestaServidor[1000], mensaje[1000]; // Variables para enviar y recibir cadenas

    char opcion[1]; // Opcion de comunicacion con el server

    // Solicitud de opcion de comunicacion
    printf("Introduce el numero del tipo de comunicacion en la que esta el Server: \n");
    printf("1: Simplex\n");
    printf("2: Half Duplex\n");
    printf("3: Full Duplex\n");

    scanf("%s", opcion);
    // Fin de solicitud de opcion de comunicacion

    if (strcmp(opcion, "1") != 0 && strcmp(opcion, "2") != 0 && strcmp(opcion, "3") != 0)
    {// En caso de que el usuario seleccione una opcion no existente
        printf("Ha elejido una opcion no existente");
        main(argc, argv);
        return EXIT_SUCCESS;
    }

    if (strcmp(opcion, "1") == 0 || strcmp(opcion, "2") == 0)
    {// En caso de que la conexion sea simplex o half duplex
        while (1)
        {
            recv(socketCliente, respuestaServidor, sizeof(respuestaServidor), 0);// Esperando mensajes del servidor

            printf("\nServidor : %s\n", respuestaServidor);

            if (strcmp(opcion, "1") != 1)
            {// En caso de que la conexion sea simplex
                break;
            }

            printf("\nMensaje a enviar :");
            scanf("%s", mensaje);
            send(socketCliente, mensaje, sizeof(mensaje), 0);// Enviando mensajes a el servidor
        }
    }
    else
    {// En caso de que la conexion sea Full duplex
        char mensajeE[1000], mensajeR[1000];// Definicion de nuevas variables de entrada y salida de mensajes
        pid_t proceso = fork();// Creacion de un nuevo proceso

        if (proceso == 0)// Rama 1 enviara los mensajes del server
        {
            while (1)
            {
                bzero(&mensajeE, sizeof(mensajeE));// Formateo del buffer para dejarlo nulo
                printf("\nMensaje a enviar: ");
                fgets(mensajeE, 10000, stdin);// Obtencion de entrada por teclado
                send(socketCliente, mensajeE, strlen(mensajeE) + 1, 0);// Envio de mensaje a el cliente
            }
        }
        else
        {// Rama 2 oira los mensajes del server
            while (1)
            {
                bzero(&mensajeR, sizeof(mensajeR));// Formateo de buffer para dejarlo nulo
                recv(socketCliente, mensajeR, sizeof(mensajeR), 0);// Esperando mensajes del server
                printf("\nServidor : %s\n", mensajeR);
            }
        }
    }

    //Cierre de la conexion
    close(socketCliente);// Cierre de conexion
    return 0;
}

/**
 ************* Webgrafia *************

 * https://es.wikibooks.org/wiki/Programación_en_C/
 * http://man7.org/linux/man-pages/man2/recv.2.html
 * https://docs.microsoft.com/en-us/cpp/preprocessor/hash-define-directive-c-cpp?view=vs-2017
 * https://www.mkssoftware.com/docs/man3/listen.3.asp
 * https://foro.elhacker.net/programacion_cc/punteros_y_en_c_iquestpara_que_sirven-t180730.0.html
 * https://es.ccm.net/contents/708-uso-de-telnet
 * https://es.stackoverflow.com/questions/31601/guardar-cadena-de-caracteres-en-c
 * https://es.stackoverflow.com/questions/383/qu%C3%A9-significa-violacion-de-segmento
 * https://stackoverflow.com/questions/1662909/undefined-reference-to-pthread-create-in-linux/1665110
 * https://www.um.es/earlyadopters/actividades/a3/PCD_Activity3_Session1.pdf
 * https://es.wikibooks.org/wiki/Programaci%C3%B3n_en_C/Cadenas_de_caracteres
 * https://www.tutorialspoint.com/c_standard_library/c_function_fprintf.htm
 * http://www.carlospes.com/curso_de_lenguaje_c/01_11_la_funcion_fflush.php
 * http://eveliux.com/mx/curso/modos-simplex-half-duplex-y-full-duplex.html
 * http://www.cplusplus.com/reference/cstdio/
 * http://pubs.opengroup.org/onlinepubs/009695399/basedefs/stdlib.h.html
 * http://sopa.dis.ulpgc.es/fso/cpp/intro_c/introc53.htm
 * https://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html
 * https://pubs.opengroup.org/onlinepubs/7908799/xns/netdb.h.html
 * https://pubs.opengroup.org/onlinepubs/7990989775/xns/netinetin.h.html

 **/