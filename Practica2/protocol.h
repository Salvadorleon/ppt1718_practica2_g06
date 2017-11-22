#ifndef protocolostpte_practicas_headerfile
#define protocolostpte_practicas_headerfile
#endif

// COMANDOS DE APLICACION
#define CONNECT "CONNECT"
#define HELO "HELO"
#define SD  "QUIT"  // Finalizacion de la conexion de aplicacion
#define RS "RESET"

// RESPUESTAS A COMANDOS DE APLICACION
#define OK  "2"
#define OKDATA "3"
//#define ERROR  "5"

//FIN DE RESPUESTA
#define CRLF "\r\n"

//ESTADOS
#define S_RSET -1
#define S_HELO 0
#define S_MAIL_RT 1
#define S_MAIL_DT 2
#define S_DATA 3
#define S_MENSAJE 4
#define S_QUIT 5
#define S_ERROR 6
//PUERTO DEL SERVICIO
#define TCP_SERVICE_PORT	25
