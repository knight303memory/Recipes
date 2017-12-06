//
//  main.cpp
//  
//
//  Created by Tusko on 2017/4/26.
//  Copyright © 2017年 Tusko. All rights reserved.
//


#include <sys/types.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <iostream>

 
const int port = 9090 ;
const int buffer_size = 1<<20;
const int method_size  = 1<<10 ;
const int filename_size = 1<<10 ;
const int common_buffer_size = 1<<10 ;

void handleError(const std::string &message);
void requestHandling(int *sock);
void sendError(int *error);
void sendData(int *sock,char *filename);
void sendHTML(int *sock,char *filename);
void sendJPG(int *sock,char *filename);

int main(int argc, const char * argv[]) {

    int serv_sock ;
    int clnt_sock ;
    
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    
    socklen_t client_address_size ;
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    
    if(serv_sock == -1)
    {
        handleError("socket error");
    }
    
    
    memset(&server_address, 0, sizeof(server_address));
    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    
    
    if(bind(serv_sock, (sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
         handleError("bind error");
    }
    
    if(listen(serv_sock, 5) == -1)
    {
        handleError("listen error");
    }
    
    while (true) {
        client_address_size = sizeof(client_address);
        
        clnt_sock = accept(serv_sock, (struct sockaddr *)&client_address, &client_address_size);
           
        if (clnt_sock == -1) {
            handleError("accept error");
        }
        
        requestHandling(&clnt_sock);
    }
    
    //system("open  ");
    close(serv_sock);
    
    return 0;
}


void requestHandling(int *sock)
{
    int client_sock = *sock;
    char buffer[buffer_size];
    char method[method_size];
    char filename[filename_size];
    
    
    read(client_sock, buffer, sizeof(buffer)-1);
    
       fputs(buffer,stdout);
    
    if(!strstr(buffer, "HTTP/"))
    {
        sendError(sock);
        
        close(client_sock);
        
        return ;
    }
    
    
    strcpy(method, strtok(buffer," /"));
    strcpy(filename, strtok(NULL, " /"));
    
    if(0 != strcmp(method, "GET"))
    {
        sendError(sock);
        close(client_sock);
        return ;
    }
    
    sendData(sock, filename);
    
}

void sendData(int *sock,char *filename)
{
    int client_sock = *sock;
    char buffer[common_buffer_size];
    char type[common_buffer_size];
    
    strcpy(buffer, filename);
    strtok(buffer, ".");
    strcpy(type, strtok(NULL, "."));

    if(0 == strcmp(type, "html")){
        sendHTML(sock, filename);
    }else if(0 == strcmp(type, "jpg")){
        sendJPG(sock, filename);
    }else{
        sendError(sock);
        close(client_sock);
        return ;
    }  
  
}

void sendHTML(int *sock,char *filename)
{
    int client_sock = *sock ;
    char buffer[buffer_size];
    FILE *fp;
    
    char status[] = "HTTP/1.0 200 OK\r\n";
    char header[] = "Server: A Simple Web Server\r\nContent-Type: text/html\r\n\r\n";

    write(client_sock, status, strlen(status));
    write(client_sock, header, strlen(header));
      

    fp = fopen( "****", "r"); //**** xcode  资源在模拟器路径下，用绝对路径吧
    if(!fp){
        sendError(sock);
        close(client_sock);
        handleError("failed to open file");
        return ;
    }
    
    fgets(buffer,sizeof(buffer), fp);
    while(!feof(fp)) {
        write(client_sock, buffer, strlen(buffer));
        fgets(buffer, sizeof(buffer), fp);
    }
    
    fclose(fp);
    close(client_sock);
    
}

void sendJPG(int *sock,char *filename)
{
    int client_sock = *sock ;
    char buffer[buffer_size];
    FILE *fp;
    FILE *fw;
    
    char status[] = "HTTP/1.0 200 OK\r\n";
    char header[] = "Server: A Simple Web Server\r\nContent-Type: image/jpeg\r\n\r\n";

    
    write(client_sock, status, strlen(status));
    write(client_sock, header, strlen(header));
    
    fp = fopen(filename, "rb");
    if(NULL == fp){
        sendError(sock);
        close(client_sock);
        handleError("failed to open file");
        return ;
    }
    
    fw = fdopen(client_sock, "w");
    fread(buffer, 1, sizeof(buffer), fp);
    while (!feof(fp)){
        fwrite(buffer, 1, sizeof(buffer), fw);
        fread(buffer, 1, sizeof(buffer), fp);
    }
    
    fclose(fw);
    fclose(fp);
    
    close(client_sock);
    
}

void handleError(const std::string &message) {
    std::cout<<message;
    exit(1);
}

void sendError(int *sock){
    int client_sock = *sock;
    
    char status[] = "HTTP/1.0 400 Bad Request\r\n";
    char header[] = "Server: A Simple Web Server\r\nContent-Type: text/html\r\n\r\n";
    char body[] = "<html><head><title>Bad Request</title></head><body><p>400 Bad Request</p></body></html>";
    
    write(client_sock, status, sizeof(status));
    write(client_sock, header, sizeof(header));
    write(client_sock, body, sizeof(body));
}



//#include <stdio.h>
//#include <netinet/in.h>
//#include <sys/socket.h> // socket
//#include <sys/types.h>  // 基本数据类型
//#include <unistd.h> // read write
//#include <string.h>
//#include <stdlib.h>
//#include <fcntl.h> // open close
//#include <sys/shm.h>
//
//#include <signal.h>
//
//#define PORT 8888
//#define SERV "0.0.0.0"
//#define QUEUE 20
//#define BUFF_SIZE 1024
//
//
//typedef struct doc_type{
//    char *key;
//    char *value;
//}HTTP_CONTENT_TYPE;
//
//HTTP_CONTENT_TYPE http_content_type[] = {
//    { "html","text/html" },
//    { "gif" ,"image/gif" },
//    { "jpeg","image/jpeg" }
//};
//
//int sockfd;
//char *http_res_tmpl = "HTTP/1.1 200 OK\r\n"
//"Server: Cleey's Server V1.0\r\n"
//"Accept-Ranges: bytes\r\n"
//"Content-Length: %d\r\n"
//"Connection: close\r\n"
//"Content-Type: %s\r\n\r\n";
//
//void handle_signal(int sign); // 退出信号处理
//void http_send(int sock,char *content); // http 发送相应报文
//
//int main(){
//    signal(SIGINT,handle_signal);
//    
//    int count = 0; // 计数
//    // 定义 socket
//    sockfd = socket(AF_INET,SOCK_STREAM,0);
//    // 定义 sockaddr_in
//    struct sockaddr_in skaddr;
//    skaddr.sin_family = AF_INET; // ipv4
//    skaddr.sin_port   = htons(PORT);
//    skaddr.sin_addr.s_addr = inet_addr(SERV);
//    // bind，绑定 socket 和 sockaddr_in
//    if( bind(sockfd,(struct sockaddr *)&skaddr,sizeof(skaddr)) == -1 ){
//        perror("bind error");
//        exit(1);
//    }
//    
//    // listen，开始添加端口
//    if( listen(sockfd,QUEUE) == -1 ){
//        perror("listen error");
//        exit(1);
//    }
//    
//    // 客户端信息
//    char buff[BUFF_SIZE];
//    struct sockaddr_in claddr;
//    socklen_t length = sizeof(claddr);
//    
//    
//    while(1){
//        int sock_client = accept(sockfd,(struct sockaddr *)&claddr, &length);
//        printf("%d\n",++count);
//        if( sock_client <0 ){
//            perror("accept error");
//            exit(1);
//        }
//        memset(buff,0,sizeof(buff));
//        int len = recv(sock_client,buff,sizeof(buff),0);
//        fputs(buff,stdout);
//        //send(sock_client,buff,len,0);
//        http_send(sock_client,"Hello World!");
//        close(sock_client);
//    }
//    fputs("Bye Cleey",stdout);
//    close(sockfd);
//    return 0;
//}
//
//void http_send(int sock_client,char *content){
//    char HTTP_HEADER[BUFF_SIZE],HTTP_INFO[BUFF_SIZE];
//    int len = strlen(content);
//    sprintf(HTTP_HEADER,http_res_tmpl,len,"text/html");
//    len = sprintf(HTTP_INFO,"%s%s",HTTP_HEADER,content);
//    
//    send(sock_client,HTTP_INFO,len,0);
//}
//
//void handle_signal(int sign){
//    fputs("\nSIGNAL INTERRUPT \nBye Cleey! \nSAFE EXIT\n",stdout);
//    close(sockfd);
//    exit(0);
//}

