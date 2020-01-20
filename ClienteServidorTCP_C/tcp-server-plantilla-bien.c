/* Servidor sobre TCP */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>

/* 
	Código de ejemplo:
	- Crea un socket pasivo TCP
	- Empieza a escuchar en el puerto 3000
	- Recibe un único cliente
	- Recibe un mensaje del cliente
	- Lo pasa a minúscula
	- Envía el mensaje cambiado al cliente
	- Cierra el socket activo (conectado al cliente)
	- Cierra el socket pasivo (el que recibe peticiones)

	Modificaciones solicitadas:
	- El puerto no es fijo sino que será pasado por línea de comando (ver nota LINEA COMANDO)
	- Comentar el código explicando (lo más detallado posible) que hace cada conjunto de sentencias
	- Comprobar todos los posibles errores (como se hace en la creación del socket)
	- Implementar el protocolo de nivel de aplicación de la práctica (ver nota PROTOCOLO)
	- Mostrar mensajes por pantalla indicando el estado actual en cada momento:
		- Además de los mensajes normales es importante mostrar la dirección IP/puerto del cliente conectado
	- La atención de clientes debe ser un bucle infinito, es decir, tras finalizar la atención al cliente debe volver al accept

	Nota LINEA COMANDO:
	- La variable argc indica el número de argumento (en este caso debería ser 2).
	- argv es un array de cadenas de caracteres:
	- argv[0] almacena el nombre del programa
	- argv[1] almacena el puerto 

	Nota PROTOCOLO:
	- Aceptar a un nuevo cliente
	- Enviar texto al cliente
	- Recibir mensaje de cliente		
	- Mientras no se reciba BYE
		- Invertir texto
		- Enviar mensaje al cliente
		- Recibir del cliente
	- Enviar OK
	- Finalizar conexión con el cliente (y volver a empezar con otro cliente)
*/

void checkError(int condition, const char * message){
	if(condition){
		fprintf(stderr,message);
		exit(-1);
	}
}
/*void comprobar (char *b){
	int i=0;
	for(i=0;i<strlen(b);i++){
		if(*(b+i)=='\n') printf("\\n ");
		else if(*(b+i)==0)printf("\\0 ");
		else printf("%c ",*(b+i));
	}
	printf("\n");
}*/

void invertir(char *b){
	int i,tam=strlen(b)-1;
	for(i=0;i<tam/2;i++){
		char k=*(b+i);
		*(b+i)=*(b+(tam-1-i));
		*(b+(tam-1-i))=k;
	}
}

int main(int argc, char * argv []) {
	checkError((argc<2),"Deberia introducir un argumento: <puerto>\n");
	//checkError((argc>2),"Demaciados argumentos: <puerto>\n");
	int 				sockfd, bytes, serv_sock, clilen;
	struct sockaddr_in 	serv_addr, cli_addr;
	char				buffer[1024],resultado[1024];
	
	int puerto=atoi(argv[1]);

	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	checkError( (serv_sock < 0), "cliente: no puedo abrir un socket TCP\n");

	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_port = htons(3000);
	serv_addr.sin_port = htons(puerto);
	serv_addr.sin_addr.s_addr=0;

	int asociar=bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	checkError((asociar<0),"Error al asociar el puerto a la conexion\n");
	printf("Creación del servidor correcta\n");
	listen(serv_sock,5);
	clilen = sizeof(cli_addr);
	while(1){
		printf("Esperando a una conexión\n");
		sockfd = accept(serv_sock, (struct sockaddr *) &cli_addr, &clilen);
		checkError((sockfd<0),"Error al aceptar trafico\n");
		char * IP=	inet_ntoa((&cli_addr)->sin_addr);
		printf("Conexión establecida con alguien: %s:%d\n",IP,cli_addr.sin_port);
		char * inicio="Bienvenido al servicio de inversión de cadenas\n";
		write(sockfd,inicio,strlen(inicio));
		while(1){
			bytes = read(sockfd,buffer,sizeof(buffer));
			checkError((bytes<0),"Error al recibir los datos\n");
			checkError((bytes==0),"Desconexión del cliente sin previo aviso\n");
			buffer[bytes]=0;
			resultado[bytes]=0;
			printf("Recibido: %s\n",buffer);
			//comprobar(buffer);
			strcpy(resultado,buffer);
			if(strcmp(resultado,"BYE\n")==0)strcpy(resultado,"OK\n");
			else invertir(resultado);
			write(sockfd,resultado,strlen(resultado));
			printf("Enviado: %s\n",resultado);
			//comprobar(resultado);
			if(strcmp(resultado,"OK\n")==0)break;
		}
		printf("Cerrando la conexión");
	   	close(sockfd);
	   	printf("Conexión cerrada\n");
	}
   	close(serv_sock);
   	return 0;
}

