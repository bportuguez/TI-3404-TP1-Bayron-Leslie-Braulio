/* tcpclient.c */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <wait.h>


int cliente(int PuertoCliente,char IpCliente[20]){       
        int sock;
        char Datos_Enviados [1024];
        struct hostent *host; // Estructura hostent que contiene informacion 
        struct sockaddr_in server_addr;   //Estructuras de sockets direcciones 
		host = gethostbyname(IpCliente);

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) // Funcion que abre el sockets o despcriptor
        { 
            perror("Error al abrir el socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;   //variable de tipo struct sockaddr_in. contiene un código de la familia de direcciones.  
        server_addr.sin_port = htons(PuertoCliente); //  convertir el puertocliete en de bytes de red el puerto 
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
        bzero(&(server_addr.sin_zero),8);  //establece todos los valores del búfer a cero. 
        
        while(1)
        {
		   if (connect(sock, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1) { 
				
				}
				else
				{
					break;
				}
		 }
   
        

        while(1) //Ciclo que envia datos 
        {  
			printf("\n \E[1;36m YO \E[0;36m (Adios para cerrar el MSN) :");
			fflush(stdout); //Limpia el buffer 
			gets(Datos_Enviados); // Captura el dato
           
		   if (strcmp(Datos_Enviados, "adios") != 0 || strcmp(Datos_Enviados, "Adios") != 0) // verifica si se envia adios 
			   send(sock,Datos_Enviados,strlen(Datos_Enviados), 0); 
			
		   else
		   {                                                 // Si es el array adios manda el datos y cierra el socket
			   send(sock,Datos_Enviados,strlen(Datos_Enviados), 0);  
			   close(sock);
			   break;
		   }
       }
       return 0;
}

int servidor(int PuertoServer)
{
      
        int sock, connected, Bytes_Recibidos, true = 1;  
        char Datos_Recibidos[1024];       
		struct sockaddr_in server_addr,client_addr;    
        int sin_size;
        
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)  // Funcion que abre el sockets o despcriptor
        {
            perror("Error en la creación del sockte");
            exit(1);
        }

        if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) //Configura el socket.
        {  
            perror("Error en la configuración del socket");
            exit(1);
        }
        
        server_addr.sin_family = AF_INET; //variable de tipo struct sockaddr_in. contiene un código de la familia de direcciones.        
        server_addr.sin_port = htons(PuertoServer); //  convertir el puertocliete en de bytes de red el puerto    
        server_addr.sin_addr.s_addr = INADDR_ANY; //  Este campo contiene la dirección IP del servidor
        bzero(&(server_addr.sin_zero),8); // //establece todos los valores del búfer a cero. 

        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))== -1) // Intenta conectarse a la direccion host y realizar bien sino tira error
        { 
            perror("Error en el bind al intentar conectarse");
            exit(1);
        }

        if (listen(sock, 3) == -1)// Escucha o espera a que si hay alguna peticion de conectarse 
        { 
            perror("Error al recibir peticion de conexion");
            exit(1);
        }
		
		printf("\nEsperando la conexion en el puerto  %i   \n ",PuertoServer);
        

		while(1){  
			sin_size = sizeof(struct sockaddr_in);
			connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size); //Acepta la coneccion 
			printf("\n Se establecio la conexion (%s , %d)\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		
            while (1)// Ciclo que permite la recrpción de datos 
            {  
				//printf("\n \E[1;36m  Yo \E[0;36m (Adios para cerrar el MSN) : ");
				Bytes_Recibidos = recv(connected,Datos_Recibidos,1024,0); // Recibe los bytes de red 
				Datos_Recibidos[Bytes_Recibidos] = '\0'; 

				if (strcmp(Datos_Recibidos, "adios") == 0 || strcmp(Datos_Recibidos , "Adios") == 0)  // Verifica que lo recibido es un adios
				{
					close(connected);
					break;
				}

				printf("\n \E[1;33m Otro Usuario = \E[0;33m %s \n " , Datos_Recibidos);
				fflush(stdout); //Limpia el Buffer
          }
          break;
      }       
	  close(sock); // Cierra sockets
      return 0;
} 

int main(){
int PuertoServer,PuertoCliente; // Variable para almacenar el pueto de cliente y el del servidor
char PuertoS[5],PuertoC[5];     // Variable que almacenan temporamente los puertos para luego castear
char IpCliente[20];
printf("\E[0;34m Bienvenido al MSM \nPor favor ingrese el Puerto del servidor \n");
gets(PuertoS);
printf("Por favor ingrese la Ip \n");
PuertoServer=atoi(PuertoS);
gets(IpCliente);
printf("Por favor ingrese el Puerto del cliente \n");
gets(PuertoC);

PuertoCliente=atoi(PuertoC);
int idProceso;
idProceso=fork(); // Se lleva acabo la duplicación de procesos mediante el fock
 if(idProceso==0 ){
		 cliente(PuertoCliente,IpCliente); // LLamada a la función cliente con los parametros de Puerto y IP
	 }
 else {
	 servidor(PuertoServer); // Llamada a la función servidor con los parametros Puerto y Nombre
	 	 
		 	 }
      return 0;  
}
