#ifndef protocolostpte_practicas_headerfile
#define protocolostpte_practicas_headerfile
#endif

// COMANDOS DE APLICACION
#define HELO "HELO"
#define QUIT  "QUIT"  // Finalizacion de la conexion de aplicacion

// RESPUESTAS A COMANDOS DE APLICACION
#define OK  "2"
#define OKDATA "3"
#define ER  "5"

//FIN DE RESPUESTA
#define CRLF "\r\n"

//ESTADOS
#define S_CONNECT 0
#define S_HELO 1
#define S_DATA 2
#define S_MAIL_FROM 3
#define S_RCPT_TO 4
#define S_SEND 5
#define S_DATA_MAIL 6
#define S_QUIT 7
#define S_EXIT 8

//PUERTO DEL SERVICIO
#define TCP_SERVICE_PORT	25
