#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <winsock2.h>

void start(void){
    WSADATA data;
    if(WSAStartup(MAKEWORD(2,2),&data) != 0){
        printf("Winsock initialization failed\n");
    }
}

void stop(void){
    WSACleanup();
}

int main(void){
    start();
    SOCKET client_socket_fd, server_socket_fd;
    server_socket_fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(2000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket_fd,(struct sockaddr *)&server_address,sizeof(server_address)) != SOCKET_ERROR){
        printf("Server Bound successfully\n");
    }else{
        printf("Server bound failure\n");
        closesocket(server_socket_fd);
        stop();
        return 1;
    }
        
    
    if(listen(server_socket_fd,10) != SOCKET_ERROR){
        printf("Server Listening on port %d\n",ntohs(server_address.sin_port));
    }else{
        printf("Server listening failed\n");
    }
        

    int address_size =  sizeof(client_address);
    if((client_socket_fd = accept(server_socket_fd,(struct sockaddr*)&client_address,&address_size)) != INVALID_SOCKET){
        printf("Client connected\n");
    }
    char request[1024];
    char *response =  "Hello from server";

    int len = recv(client_socket_fd,request,1024,0);
    if(len > 0){
        request[len] = '\0';
        printf("Client Says:\n\t%s",request);
    }
    if(send(client_socket_fd,response,strlen(response),0) != SOCKET_ERROR){
        printf("Response sent successfully\n");
    }else{
        printf("response not sent\n");
    }
    closesocket(client_socket_fd);
    closesocket(server_socket_fd);
    stop();

}