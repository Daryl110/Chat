#include <stdio.h>      // Libreria para interaccion con dispositivos E/S
#include <stdlib.h>     // Libreria estandar de c (Tipos y funciones basicas)
#include <string.h>     // Libreria para manejo de cadenas
#include <unistd.h>     // Libreria de tipos y constantes simbólicas estándar
#include <sys/socket.h> // Libreria de sockets
#include <arpa/inet.h>  // Libreria para definiciones de operaciones de internet
#include <netinet/in.h> // Libreria de protocolos y estandares (Familias de protocolos)
#include <netdb.h>      // Libreria para definiciones de operaciones de base de datos de red
#include <sys/types.h>
#include <resolv.h>

typedef struct // Fragmento de Codigo tomado de https://es.wikibooks.org/wiki/Programaci%C3%B3n_en_C/Sockets --Modificado
{
    short int sin_family;        //Protocolo para la conexión
    unsigned short int sin_port; //Puerto para la conexión
    struct in_addr sin_addr;     //Estructura a la dirección IP
    unsigned char sin_zero[8];   //Relleno
} sockaddr_in;

typedef struct // Fragmento de Codigo tomado de https://es.wikibooks.org/wiki/Programaci%C3%B3n_en_C/Sockets --Modificado
{
    unsigned long s_addr; //Contiene la dirección IP del host
} in_addr;

typedef struct // Fragmento de Codigo tomado de https://es.wikibooks.org/wiki/Programaci%C3%B3n_en_C/Sockets --Modificado
{
    unsigned short sa_family; //Protocolo
    char sa_data[14];         //Dirección IP del host
} sockaddr;

int main(int argc, char const **argv)
{

    char opcion[1]; // Opcion de comunicacion Cliente/Servidor

    //Muestra por consola las opciones que el usuario puede elegir
    printf("Introduce el numero del tipo de comunicacion que quieres: \n");
    printf("1: Simplex\n");
    printf("2: Half Duplex\n");
    printf("3: Full Duplex\n");

    //Obtencion de la opcion elegida por el usuario
    scanf("%s", opcion);

    //Valida el caso de que la opcion seleccionada por el usuario no este en la lista de seleccion
    if (strcmp(opcion, "1") != 0 && strcmp(opcion, "2") != 0 && strcmp(opcion, "3") != 0)
    {
        printf("Ha elejido una opcion no existente");
        main(argc, argv);
        return EXIT_SUCCESS;
    }

    int conexionServidor, puerto;// Declaracion de la conexion local (Server) y el puerto a utilizar

    puerto = 8080; // Puerto por defecto
    if (argc > 1)
    {
        puerto = atoi(argv[1]); // Puerto establecido por el usuario
    }

    sockaddr_in servidor, cliente; // Declaracion de detalles del servidor y el cliente
    conexionServidor = socket(AF_INET, SOCK_STREAM, 0); // Crecion del servidor

    bzero((char *)&servidor, sizeof(servidor)); // Formateando Servidor prevenir errores

    // Llenado de datos para el server
    servidor.sin_family = AF_INET;         // Establecimiento de protocolo TCP/IP v4
    servidor.sin_port = htons(puerto);     // Casteo de puerto a utilizar
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);; // Establecimiento de direcciones ip (Cualquiera)
    // Fin de llenado de datos para el server

    if (bind(conexionServidor, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    { // Asosiacion del puerto con el socket server
        printf("Error al asociar el puerto con la conexion\n");
        return 0;
    }
    else
    {
        printf("Puerto asignado...\n");
    }

    listen(conexionServidor, 5); // Escuchando el servidor (Establecimiento de las caracteristicas de la conexion)

    printf("Escuchando el puerto: %d\n", ntohs(servidor.sin_port));

    socklen_t longuitudCliente = sizeof(cliente);// Variable de longitud del cliente

    char mensajeE[1000], mensajeR[1000];// Variables para entrada y salida de bytes

    if(atoi(opcion) == 1){//Simplex
        printf("\nHa Seleccionado Simplex\n");

        while (1) //Ciclo infinito que tomara el cliente conectado y le enviara bytes
        {
            int conexionCliente = accept(conexionServidor, (struct sockaddr *)&cliente, &longuitudCliente); //End point Espera cliente (En teoria))
            printf("Conectado con\nip: %s\npuerto: %d\n\n", inet_ntoa(cliente.sin_addr), htons(cliente.sin_port));

            printf("\nMensaje a enviar: ");
            scanf("%s", mensajeE);// Obtencion de mensaje a enviar

            send(conexionCliente, mensajeE, sizeof(mensajeE), 0);// Envio de mensaje a cliente
        }

    }
    else if(atoi(opcion) == 2){// Half Duplex
        printf("\nHa Seleccionado Half Duplex\n");

        int conexionCliente = accept(conexionServidor, (struct sockaddr *)&cliente, &longuitudCliente); // End point (Espera cliente (En teoria))

        while (1)
        {
            printf("\nMensaje a enviar: ");
            scanf("%s", mensajeE);// Obtencion de mensaje a enviar

            send(conexionCliente, mensajeE, sizeof(mensajeE), 0);// Envio de mensaje

            recv(conexionCliente, &mensajeR, sizeof(mensajeR), 0);// En espera de respuesta

            printf("%s:%d : %s\n", inet_ntoa(cliente.sin_addr), cliente.sin_port, mensajeR);
        }

    }
    else if(atoi(opcion) == 3){// Full Duplex
        printf("\nHa Seleccionado Full Duplex\n");

        int conexionCliente = accept(conexionServidor, (struct sockaddr *)&cliente, &longuitudCliente);

        pid_t proceso = fork();// Creacion de proceso en paralelo

        if (proceso)// ALternativa 1 del proceso
        {// En espera de mensajes del cliente
            while (1)
            {
                bzero(&mensajeR,sizeof(mensajeR));// Formateo del buffer mensajeR
                recv(conexionCliente, mensajeR, sizeof(mensajeR), 0);// En espera de mensaje del cliente
                printf("%s:%d : %s", inet_ntoa(cliente.sin_addr), cliente.sin_port, mensajeR);
            }
        }
        else
        {// En espera de mensajes a enviar a el cliente
            while (1)
            {
                bzero(&mensajeE,sizeof(mensajeE));// Formateo de buffer mensajeE
                printf("Mensaje a enviar: ");
                fgets(mensajeE,10000,stdin);// Obtencion de mensaje a enviar a el cliente
                send(conexionCliente, mensajeE, strlen(mensajeE)+1, 0);// Envio de mensaje a el cliente
            }
        }
    }
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