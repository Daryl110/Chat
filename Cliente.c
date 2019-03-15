#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "netdb.h"
#include "arpa/inet.h"

int main(int argc, char **argv)
{
    int port;
    int socketCliente = socket(AF_INET, SOCK_STREAM, 0);

    port = 8080;
    if (argc > 1)
    {
        port = atoi(argv[1]); // Puerto establecido por el usuario
    }

    struct sockaddr_in direccionSocket;
    direccionSocket.sin_family = AF_INET;
    direccionSocket.sin_port = htons(port);
    direccionSocket.sin_addr.s_addr = INADDR_ANY;

    connect(socketCliente, (struct sockaddr *)&direccionSocket, sizeof(direccionSocket));

    char respuestaServidor[1000], mensaje[1000];

    char opcion[1];

    printf("Introduce el numero del tipo de comunicacion en la que esta el Server: \n");
    printf("1: Simplex\n");
    printf("2: Half Duplex\n");
    printf("3: Full Duplex\n");

    scanf("%s", opcion);

    if (strcmp(opcion, "1") != 0 && strcmp(opcion, "2") != 0 && strcmp(opcion, "3") != 0)
    {
        printf("Ha elejido una opcion no existente");
        main(argc, argv);
        return EXIT_SUCCESS;
    }

    if (strcmp(opcion, "1") == 0 || strcmp(opcion, "2") == 0)
    {
        while (1)
        {
            recv(socketCliente, respuestaServidor, sizeof(respuestaServidor), 0);

            printf("\nServidor : %s\n", respuestaServidor);

            if (strcmp(opcion, "1") != 1)
            {
                break;
            }

            printf("\nMensaje a enviar :");
            scanf("%s", mensaje);
            send(socketCliente, mensaje, sizeof(mensaje), 0);
        }
    }
    else
    {
        char mensajeE[1000], mensajeR[1000];
        pid_t proceso = fork();

        if (proceso == 0)
        {
            while (1)
            {
                bzero(&mensajeE, sizeof(mensajeE));
                printf("\nMensaje a enviar: ");
                fgets(mensajeE, 10000, stdin);
                send(socketCliente, mensajeE, strlen(mensajeE) + 1, 0);
            }
        }
        else
        {
            while (1)
            {
                bzero(&mensajeR, sizeof(mensajeR));
                recv(socketCliente, mensajeR, sizeof(mensajeR), 0);
                printf("\nServidor : %s\n", mensajeR);
            }
        }
    }

    //Cierre de la conexion
    close(socketCliente);
    return 0;
}