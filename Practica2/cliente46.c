/*******************************************************
Protocolos de Transporte
Grado en Ingeniería Telemática
Dpto. Ingeníería de Telecomunicación
Univerisdad de Jaén
Fichero: cliente.c
Versión: 2.0
Fecha: 09/2017
Descripción:
Cliente sencillo TCP.
Autor: Juan Carlos Cuevas Martínez
Alumnos Salvador Trujillo Diaz y Salvador Leon Ortega
*******************************************************/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <ws2tcpip.h>//Necesaria para las funciones IPv6
#include <conio.h>
#include "protocol.h"


#pragma comment(lib, "Ws2_32.lib")

int main(int *argc, char *argv[])
{
	SOCKET sockfd;
	struct sockaddr *server_in;
	struct sockaddr_in server_in4;
	struct sockaddr_in6 server_in6;
	int ipdestl = 0;
	int address_size = sizeof(server_in4);
	char buffer_in[1024], buffer_out[1024], input[1024], remit[50], dest[50], asunto[50] = "", fecha[1024] = "", mensaje[1024] = "", entrada[1024] = ".";
	int recibidos = 0, enviados = 0;
	int estado = S_HELO;
	char option;
	int ipversion = AF_INET;//IPv4 por defecto
	char ipdest[256];
	char default_ip4[16] = "127.0.0.1";
	char default_ip6[64] = "::1";

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	//Inicialización Windows sockets - SOLO WINDOWS
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
		return(0);

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return(0);
	}
	//Fin: Inicialización Windows sockets

	printf("**************\r\nCLIENTE TCP SENCILLO SOBRE IPv4 o IPv6\r\n*************\r\n");


	do {

		printf("CLIENTE> ¿Que version de IP desea usar? 6 para IPv6, 4 para IPv4 [por defecto] ");
		gets_s(ipdest, sizeof(ipdest));

		if (strcmp(ipdest, "6") == 0) {
			ipversion = AF_INET6;

		}
		else { //Distinto de 6 se elige la versión 4
			ipversion = AF_INET;
		}

		sockfd = socket(ipversion, SOCK_STREAM, 0);
		if (sockfd == INVALID_SOCKET) {
			printf("CLIENTE> ERROR\r\n");
			exit(-1);
		}
		else {

			struct in_addr address;

			printf("Introduzcala direccion IP o el dominio destino: ");
			gets_s(ipdest, sizeof(ipdest));
			ipdestl = inet_addr(ipdest);
			if (ipdestl == INADDR_NONE) {
				//La dirección introducida por teclado no es correcta o
				//corresponde con un dominio.
				struct hostent*host;
				host = gethostbyname(ipdest);
				if (host != NULL) {
					memcpy(&address, host->h_addr_list[0], 4);
					printf("\nDireccion%s\n", inet_ntoa(address));

				}

				strcpy_s(ipdest, sizeof(ipdest), inet_ntoa(address));
			}

			//printf("CLIENTE> Introduzca la IP destino (pulsar enter para IP por defecto): ");
			//gets_s(ipdest,sizeof(ipdest));

			//Dirección por defecto según la familia
			if (strcmp(ipdest, "") == 0 && ipversion == AF_INET)
				strcpy_s(ipdest, sizeof(ipdest), default_ip4);

			if (strcmp(ipdest, "") == 0 && ipversion == AF_INET6)
				strcpy_s(ipdest, sizeof(ipdest), default_ip6);

			if (ipversion == AF_INET) {
				server_in4.sin_family = AF_INET;
				server_in4.sin_port = htons(TCP_SERVICE_PORT);
				//server_in4.sin_addr.s_addr=inet_addr(ipdest);
				inet_pton(ipversion, ipdest, &server_in4.sin_addr.s_addr);
				server_in = (struct sockaddr*)&server_in4;
				address_size = sizeof(server_in4);
			}

			if (ipversion == AF_INET6) {
				memset(&server_in6, 0, sizeof(server_in6));
				server_in6.sin6_family = AF_INET6;
				server_in6.sin6_port = htons(TCP_SERVICE_PORT);
				inet_pton(ipversion, ipdest, &server_in6.sin6_addr);
				server_in = (struct sockaddr*)&server_in6;
				address_size = sizeof(server_in6);
			}

			estado = S_HELO;

			if (connect(sockfd, server_in, address_size) == 0) {
				printf("CLIENTE> CONEXION ESTABLECIDA CON %s:%d\r\n", ipdest, TCP_SERVICE_PORT);
				//Recibo
				recibidos = recv(sockfd, buffer_in, 512, 0);
				buffer_in[recibidos] = 0x00;
				printf(buffer_in);
				//Inicio de la máquina de estados
				do {
					switch (estado) {
					case S_RSET:
						sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", RS, CRLF);
						system("cls");
						estado = S_HELO;
						break;

					case S_HELO:
						sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", HELO, CRLF);
						break;
					case S_MAIL_RT:
						// ENVIO
						printf("CLIENTE> Introduce tu correo: ");
						gets_s(input, sizeof(input));
						if (strlen(input) == 0) {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", SD, CRLF);
						}
						else
							//send(sockfd, input, strlen(input), 0);
							strcpy_s(remit, sizeof(remit), input);
						//recv(sockfd, input, sizeof(input), 0);
						sprintf_s(buffer_out, sizeof(buffer_out), "MAIL FROM:<%s>%s", remit, CRLF);

						break;
					case S_MAIL_DT:
						printf("CLIENTE> Introduce el correo destinatario: ");
						gets_s(input, sizeof(input));
						if (strlen(input) == 0) {
							sprintf_s(buffer_out, sizeof(buffer_out), "%s%s", SD, CRLF);

						}
						else
							strcpy_s(dest, sizeof(dest), input);
						//recv(sockfd, input, sizeof(input), 0);
						sprintf_s(buffer_out, sizeof(buffer_out), "RCPT TO:<%s>%s", dest, CRLF);

						//sprintf_s (buffer_out, sizeof(buffer_out), "%s %s%s",DEST,CRLF);
						break;

					case S_DATA:
						sprintf_s(buffer_out, sizeof(buffer_out), "DATA%s", CRLF);

						break;



					case S_MENSAJE:

						//Asunto
						printf("\nAsunto: ");
						gets(asunto);

						time_t t;
						struct tm *tm;
						char fechayhora[100];

						t = time(NULL);
						tm = localtime(&t);
						strftime(fechayhora, 100, "%d/%m/%Y a las %H:%M", tm);

						strcpy_s(fecha, sizeof(fecha), fechayhora);

						//Cabeceras  del mensaje
						sprintf_s(mensaje, sizeof(mensaje), "Date: %s%sFrom: %s%sTo: %s%sSubject: %s%s", fecha, CRLF, remit, CRLF, dest, CRLF, asunto, CRLF);
						printf("\nMENSAJE: (escribe un '.' para finalizar)\r\n");
						do {
							gets(entrada);
							sprintf_s(mensaje, sizeof(mensaje), "%s%s", entrada, CRLF);
							enviados = send(sockfd, mensaje, (int)strlen(mensaje), 0);
						} while (strcmp(entrada, ".") != 0);
						sprintf_s(buffer_out, sizeof(mensaje), "%s%s", mensaje, CRLF);
						break;

					}

					enviados = send(sockfd, buffer_out, (int)strlen(buffer_out), 0);
					if (enviados<0) {
						DWORD error = GetLastError();

						printf("Error %d en el envio de datos%s", error, CRLF);
						break;
					}

					recibidos = recv(sockfd, buffer_in, 512, 0);
					if (recibidos <= 0) {
						DWORD error = GetLastError();
						if (recibidos<0) {
							printf("CLIENTE> Error %d en la recepción de datos\r\n", error);
							estado = S_QUIT;
						}
						else {
							printf("CLIENTE> Conexión con el servidor cerrada\r\n");
							estado = S_QUIT;
						}
					}
					else

						if (estado == S_HELO || estado == S_MAIL_DT || estado == S_MAIL_RT) {
							buffer_in[recibidos] = 0x00;
							printf(buffer_in);
						}


					if (strncmp(buffer_in, "554", 3) == 0) {
						printf("Destinatario incorrecto,intentelo de nuevo \r\n");
					}

					if (estado == S_MENSAJE && strncmp(buffer_in, "250", 3) == 0) {
						char opcion = "";

						printf("Mensaje enviado correctamente \r\n");
						printf("¿Desea enviar otro mensaje? S/N \r\n");
						opcion = _getche();
						if (opcion == 'S' || opcion == 's') {
							estado = S_RSET;
						}
						else { estado = S_QUIT; }

					}

					//Avance de estado
					if (strncmp(buffer_in, "2", 1) == 0 || strncmp(buffer_in, "3", 1) == 0) {
						estado++;
					}




				} while (estado != S_QUIT);
			}
			else {
				int error_code = GetLastError();
				printf("CLIENTE> ERROR AL CONECTAR CON %s:%d\r\n", ipdest, TCP_SERVICE_PORT);
			}
			// fin de la conexion de transporte
			closesocket(sockfd);

		}
		printf("-----------------------\r\n\r\nCLIENTE> Volver a conectar (S/N)\r\n");
		option = _getche();

	} while (option != 'n' && option != 'N');

	WSACleanup();
	return(0);
}