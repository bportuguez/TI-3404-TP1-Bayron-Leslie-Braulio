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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int cliente()

{

        int sock, bytes_recieved;  
        char Datos_Enviados[1024],Datos_Recibidos[1024];
        struct hostent *host;
        struct sockaddr_in server_addr;  

        host = gethostbyname("127.0.0.1");

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		{
			perror("Error no se puede establecer el socket");
        	exit(1);
        }

        server_addr.sin_family = AF_INET;     
        server_addr.sin_port = htons(5000);   
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
        bzero(&(server_addr.sin_zero),8); 

        if (connect(sock, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1) 
        {
			perror("Error al intentar conectarse");
        	exit(1);
        }

        while(1)
        {
        	bytes_recieved=recv(sock,Datos_Recibidos,1024,0);
        	Datos_Recibidos[bytes_recieved] = '\0';

			printf("\ Datos Recibidos = %s " , Datos_Recibidos);
           
			gets(Datos_Enviados);

			send(sock,Datos_Enviados,strlen(Datos_Enviados), 0);
        }
		return 0;
}

/* tcpserver.c */
int servidor()
{
	int sock, connected, bytes_recieved , true = 1;  
        char Datos_Enviados[1024] , Datos_Recibidos[1024];       

        struct sockaddr_in server_addr,client_addr;    
        int sin_size;
        
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		{
        	perror("Error no se puede establecer el socket");
			exit(1);
        }

        if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) 
		{
            perror("Setsockopt");
            exit(1);
        }
        
        server_addr.sin_family = AF_INET;         
        server_addr.sin_port = htons(5000);     
        server_addr.sin_addr.s_addr = INADDR_ANY; 
        bzero(&(server_addr.sin_zero),8); 

        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) 
		{
            perror("Error en la funcion bind");
            exit(1);
        }

        if (listen(sock, 5) == -1) 
		{
            perror("Error a la hora de recibir conexion");
            exit(1);
        }
		
		printf("\ Esperando conexiones en el puerto numero 5000");
        fflush(stdout);

        while(1)
        {  

            sin_size = sizeof(struct sockaddr_in);

            connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

            printf("\n Esta conectado con la ip %s y el puerto %d ",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

            while (1)
            {
				gets(Datos_Enviados);

				send(connected, Datos_Enviados,strlen(Datos_Enviados), 0);  

            	bytes_recieved = recv(connected,Datos_Recibidos,1024,0);

            	Datos_Recibidos[bytes_recieved] = '\0';

            	printf("\n Datos Recibidos = %s " , Datos_Recibidos);
            	fflush(stdout);
            }
        }       
		close(sock);
      	return 0;
}

int main()
{
	printf("Por favor digite 0 para servidor o 1 para cliente \n");
	int type;
	scanf("%i",type);
	 
	if (type==1)
	{
		cliente();
	}
	else
	{
		servidor();
	}
}




