#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <winsock2.h>

void start(void){
    WSADATA data;
    if(WSAStartup(MAKEWORD(2,2),&data) != 0)
        printf("Winsock initialization failed\n");
}

void stop(){
    WSACleanup();
}

int main(void){
    start();
    SOCKET socket_fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(80);
    address.sin_addr.s_addr = inet_addr("172.217.170.174");

    if(connect(socket_fd,(struct sockaddr *)&address,sizeof(address)) == SOCKET_ERROR){
        printf("Connection Failed\n");
        closesocket(socket_fd);
        stop();
    }
    char * get_request = "GET / HTTP/1.1\r\nhost:google.com\r\n\r\n";
    char response[1024];
    if(send(socket_fd,get_request,strlen(get_request),0) != INVALID_SOCKET){
        printf("Sent success\n");
    }else{
        printf("Not Sent\n");
    }
    if(recv(socket_fd,response,sizeof(response),0) != INVALID_SOCKET)
        printf("The response was: \n%s",response);
    
    closesocket(socket_fd);
    stop();
    return 0;
}