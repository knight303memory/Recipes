//
//  Board.hpp
//  TicTacToeServer
//
//  Created by Tusko on 2017/4/9.
//  Copyright © 2017年 Tusko. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include "AbstractBoard.h"
#define X -1
#define O  1
#define BUFFER_SIZE   1024



class TTTClient : public AbstractBoard{
    
    
public:
    TTTClient(){
        ClntSocket();
    }
    
private:
    
    
    int clnt_sock ;
    
    void ClntSocket()
    {
        
        clnt_sock = socket(AF_INET, SOCK_STREAM, 0);
        
        struct sockaddr_in serv_addr;
        bzero(&serv_addr, sizeof(serv_addr));
        
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serv_addr.sin_port = htons(6666);
        
        
        connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        
        char buffer[BUFFER_SIZE] = {0};
        
        while (1) {
            
            if ( read(clnt_sock, buffer, sizeof(buffer) - 1) )
            {
                printf("Message form TTTserver: %s\n", buffer);
                
                char str[] = "connect TTTClient successfully\n";
                write(clnt_sock, str, sizeof(str));
                break ;
            }
        }
        
        
        
        std::cout<<"Who plays first? m - man(O) , c - computer(X)"<<std::endl ;
        std::cout<<"Enter your select:"<<std::endl ;
        
        char ch[20] = {0};
        std::cin>>ch;
        
        while ( !( ch[0]  == 'c' || ch[0] == 'm') || strlen(ch) > 1) {
            std::cout<<"Select wrong,enter again:"<<std::endl ;
            std::cin>>ch;
        }
        
        if(ch[0] == 'm')
        {
            manFirst = true ;
        }
        
        write(clnt_sock,ch, sizeof(ch));
        
        startPlay();
    }
    
    void startPlay()
    {
        printBoard();
        
        if(manFirst)
        {
            player = MAN;
            while (1)
            {
                manPlay();
                if(!comPlay())
                {
                    break ;
                }
            }
        }
        else
        {
            player = COM;
            while (1) {
                if(!comPlay())
                {
                    break ;
                }
                manPlay();
            }
            
        }
        
        sleep(3);
        disconnect();
    }
    
    virtual bool manPlay()
    {
        
        Move move ;
        int x, y;
        printf("请输入位置坐标  e.g ：（0 0）为左上角 （2，2）为右下角 \n");
        scanf("%d", &x);
        scanf("%d", &y);
        
        while (x < 0 || x > 2 || y < 0 || y > 2)
        {
            printf("您输入的坐标错误，请重新输入:x:(0~2) , y:(0~2)\n");
            scanf("%d", &x);
            scanf("%d", &y);
        }
        while (board[x][y] != 0)
        {
            printf("该位置已有棋，请重新输入:\n");
            scanf("%d", &x);
            scanf("%d", &y);
        }
        move.x = x ;
        move.y = y ;
        
        board[move.x][move.y] = MAN;
        
        sendMove(move, clnt_sock);
        
        return true ;
    }
    
    
    
    virtual bool comPlay()
    {
        
        Move move = recvMove(clnt_sock);
        
        if(strlen(move.mesg) > 0)
        {
            if(-1 !=move.x)
            {
                board[move.x][move.y] = COM;
                printBoard();
            }
            
            std::cout<<move.mesg<<std::endl;
            
            return false;
        }
        
        board[move.x][move.y] = COM;
        
        printBoard();
        
        return true ;
    }
    
    
    void disconnect()
    {
        close(clnt_sock);
    }
    
};


#endif /* Board_hpp */
