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
    
    char request[1024];
    char *response =  "Hello from server";
    fd_set set;
    int max = server_socket_fd;

    FD_ZERO(&set);
    FD_SET(server_socket_fd,&set);

    while(1){
        fd_set temp = set;

        if(select(max + 1,&temp,NULL,NULL,NULL) == SOCKET_ERROR){
            continue;
        }

        if(FD_ISSET(server_socket_fd,&temp)){
            if((client_socket_fd = accept(server_socket_fd,(struct sockaddr*)&client_address,&address_size)) == INVALID_SOCKET){
                continue;
            }else{
                printf("Client Connected\n");
            }
            FD_SET(client_socket_fd,&set);
            if(client_socket_fd > max){
                max = client_socket_fd;
                continue;
            }
        }

        for(int i = 0; i <= max; i++){
            if(FD_ISSET(i,&temp)){
                if(i == server_socket_fd)
                    continue;
                int len = recv(i,request,1024,0);
                if(request[0] == '/'){
                    send(i,"set",sizeof("set"),0);
                    continue;
                } 
                if(len > 0){
                    request[len] = '\0';
                    printf("Client Says:\n\t%s",request);
                    if(send(i,response,strlen(response),0) != SOCKET_ERROR){
                         printf("Response sent successfully\n");
                    }else{
                         printf("response not sent\n");
                    }
                }else{
                    printf("Client disconnected\n");
                    closesocket(i);
                    FD_CLR(i,&set);
                }
            }
        }
    }
    closesocket(server_socket_fd);
    stop();

}