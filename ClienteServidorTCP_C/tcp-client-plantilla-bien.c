/* Cliente sobre TCP */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>

/* 
	Código de ejemplo:
	- Crea un socket TCP
	- Se conecta a la dirección 127.0.0.1:3000
	- Envía al servidor el mensaje: "Probando"
	- Recibe un mensaje del servidor
	- Cierra el socket

	Modificaciones solicitadas:
	- El puerto e IP no es fijo sino que será pasado por línea de comando (ver nota LINEA COMANDO)
	- Comentar el código explicando (lo más detallado posible) que hace cada conjunto de sentencias
	- Comprobar todos los posibles errores (como se hace en la creación del socket)
	- Implementar el protocolo de nivel de aplicación de la práctica (ver nota PROTOCOLO)
	- Mostrar mensajes por pantalla indicando el estado actual en cada momento

	Nota LINEA COMANDO:
	- La variable argc indica el número de argumento (en este caso debería ser 3).
	- argv es un array de cadenas de caracteres:
	- argv[0] almacena el nombre del programa
	- argv[1] almacena la IP
	- argv[2] almacena el puerto 

	Nota PROTOCOLO:
	- Conectarse al servidor
	- Recibir texto del servidor
	- Leer de teclado
	- Mientras no se lea TERMINAR
		- Convertir a minúscula
		- Añadir el \n al mensaje
		- Enviar mensaje al servidor
		- Recibir del servidor
	- Enviar BYE
	- Recibir OK
	- Finalizar conexión
*/

void checkError(int condition, const char * message){
	if(condition){
		fprintf(stderr,message);
		exit(-1);
	}
}
/*void comprobar (char *b){
	int i=0;
	printf("%d\n",strlen(b));
	for(i=0;i<strlen(b);i++){
		if(*(b+i)=='\n') printf("b[%d]:\\n ",i);
		else if(*(b+i)=='\0')printf("b[%d]:\\0 ",i);
		else printf("b[%d]:%c ",i,*(b+i));
	}
	printf("\n");
}*/
void minuscula(char *b){
	while(*b != 0){
		*b=tolower(*b);
		b++;
	}
	*(b)='\n';
	*(b+1)=0;
}
void clear(char* b){
	int i;
	for(i=0;i<strlen(b);i++)*b=0;
	}
int main(int argc, char * argv []) {
	checkError((argc<3),"Deberías haber introducido dos valores: <host> <puerto> \n");
	//checkError((argc>3),"Demaciados argumentos: <host> <puerto>\n");
	int 				sockfd;
	struct sockaddr_in 	serv_addr;
	char				buffer[1024],resultado[1024],inicio[1024];
	
	char * IP=argv[1];
	int puerto=atoi(argv[2]);
	
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	checkError( (sockfd < 0), "cliente: no puedo abrir un socket TCP\n");
	
	
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_port = htons(3000);
	serv_addr.sin_port = htons(puerto);
	//inet_aton("127.0.0.1",&serv_addr.sin_addr);
	inet_aton(IP,&serv_addr.sin_addr);
	printf("Conectando con %s:%d\n",IP,puerto);
	int conectado=connect(sockfd, (struct sockaddr *) &serv_addr,	sizeof(serv_addr));
	
	checkError( (conectado < 0), "Error conectando con el host\n");
	printf("Conectado correctamente\n");
	read(sockfd,inicio,sizeof(inicio));
	printf("%s",inicio);
	fflush(stdout);
	while(1){
		printf("Introduzca la frase a invertir: ");
		gets(buffer);//Lee todo lo que se haya escrito en el buffer, poniendo al final el caracter \0. Tiene el riesgo de que si se pasa de los 1024 caracteres, funcione mal esta instrucción
		if(strcmp(buffer,"TERMINAR")==0)strcpy(buffer,"BYE\n");
		else minuscula(buffer);
		printf("Enviado: %s\n",buffer);
		write(sockfd,buffer,strlen(buffer));
		buffer[strlen(buffer)]=0;
		printf("Esperando la respuesta\n");
		int bytes=read(sockfd,resultado,sizeof(resultado));
		checkError((bytes<0),"Error al recibir los datos\n");
		checkError((bytes==0),"Desconexión del servidor sin previo aviso\n");
		resultado[bytes]=0;
		printf("Respuesta: %s\n",resultado);
		//comprobar(resultado);
		if(strcmp(resultado,"OK\n")==0)break;
	}
	printf("Cerrandose conexión\n");
	close(sockfd);
	printf("Conexión cerrada\n");
	return 0;
}

