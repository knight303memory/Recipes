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
#include <string>
#include "AbstractBoard.h"

#define MAX_NUM 1000;





class TTTServer :public AbstractBoard {
    
    
public:
    TTTServer(){
        ServSocket();
    }
    
    ~TTTServer(){}
    
    int tempBoard[3][3] = {0};
    
    
private:
    
    int serv_sock ;
    int clnt_sock ;
    
    
    Move bestMove ;
    
    void ServSocket()
    {
        serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        
        struct sockaddr_in serv_addr ;
        bzero(&serv_addr, sizeof(serv_addr));
        
        serv_addr.sin_family = AF_INET;
        
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        serv_addr.sin_port = htons(6666);
        
        
        bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        
        listen(serv_sock, 20);
        
        
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        
        char str[] = "connect TTTserver successfully\n";
        write(clnt_sock, str, sizeof(str));
        
        
        char buffer[BUFFER_SIZE] = {0};
        read(clnt_sock, buffer, sizeof(buffer) - 1);
        
        printf("Message form TTTclient: %s\n", buffer);
        
        
        setFirst();
        
        startPlay();
        
    }
    
    void startPlay()
    {
        if(manFirst)
        {
            player = MAN;
            while (1) {
                if(!manPlay())
                {
                    break ;
                }
                printBoard();
                if (!comPlay())
                {
                    
                    break ;
                }
                printBoard();
            }
            printBoard();
        }
        else
        {
            player = COM;
            while (1) {
                
                if(!comPlay())
                {
                    
                    break ;
                }
                printBoard();
                
                if (!manPlay())
                {
                    
                    break ;
                }
                printBoard();
                
                
            }
            printBoard();
        }
        
        sleep(2);
        disconnect();
        
    }
    
    
    
    
    //set who plays first
    void setFirst()
    {
        char buffer[BUFFER_SIZE];
        read(clnt_sock, buffer, sizeof(buffer) - 1);
        
        
        if(buffer[0] == 'm')
        {
            manFirst = true ;
        }

    }
    
    
    int isWin() {
        for (int i = 0; i < COL; i++)
        {
            if (board[i][0] + board[i][1] + board[i][2] == 3)
                return 1;
            else if (board[i][0] + board[i][1] + board[i][2] == -3)
                return -1;
        }
        for (int j = 0; j < ROW; j++)
        {
            if (board[0][j] + board[1][j] + board[2][j] == 3)
                return 1;
            else if (board[0][j] + board[1][j] + board[2][j] == -3)
                return -1;
        }
        if (board[0][0] + board[1][1] + board[2][2] == 3 || board[0][2] + board[1][1] + board[2][0] == 3)
            return 1;
        else if (board[0][0] + board[1][1] + board[2][2] == -3 || board[0][2] + board[1][1] + board[2][0] == -3)
            return -1;
        
        else  return 0;
        
    }
    
    //评估函数
    int evaluteMap() {
        
        
        int i, j;
        
        if (isWin() == COM)
        {
            return MAX_NUM;
        }
        
        if (isWin() == MAN)
        {
            return -MAX_NUM;
        }
        
        
        int count = 0;
        
        for (i = 0; i < ROW; i++)
            for (j = 0; j < COL; j++)
            {
                if (board[i][j] == EMPTY)
                    tempBoard[i][j] = COM;
                else
                    tempBoard[i][j] = board[i][j];
            }
        
        
        for (i = 0; i < ROW; i++)
            count += (tempBoard[i][0] + tempBoard[i][1] + tempBoard[i][2]) / 3;
        for (i = 0; i < COL; i++)
            count += (tempBoard[0][i] + tempBoard[1][i] + tempBoard[2][i]) / 3;
        
        count += (tempBoard[0][0] + tempBoard[1][1] + tempBoard[2][2]) / 3;
        count += (tempBoard[2][0] + tempBoard[1][1] + tempBoard[0][2]) / 3;
        
        
        for (i = 0; i < ROW; i++)
            for (j = 0; j < COL; j++)
            {
                if (board[i][j] == EMPTY)
                    tempBoard[i][j] = MAN;
                else tempBoard[i][j] = board[i][j];
            }
        
        
        for (i = 0; i < 3; i++)
            count += (tempBoard[i][0] + tempBoard[i][1] + tempBoard[i][2]) / 3;
        for (i = 0; i < 3; i++)
            count += (tempBoard[0][i] + tempBoard[1][i] + tempBoard[2][i]) / 3;
        count += (tempBoard[0][0] + tempBoard[1][1] + tempBoard[2][2]) / 3;
        count += (tempBoard[2][0] + tempBoard[1][1] + tempBoard[0][2]) / 3;
        
        
        
        return count;
    }
    
    
    void makeMove(const Move &curMove)
    {
        board[curMove.x][curMove.y] = player;
        player = (player == COM) ? MAN : COM;
    }
    
    void unMakeMove(Move curMove) {
        board[curMove.x][curMove.y] = 0;
        player = (player == COM) ? MAN : COM;
    }
    
    //get empty place
    int getMoveList(Move moveList[]) {
        int moveCount = 0;
        int i, j;
        for (i = 0; i < COL; i++)
        {
            for (j = 0; j < ROW; j++)
            {
                if (board[i][j] == 0)
                {
                    moveList[moveCount].x = i;
                    moveList[moveCount].y = j;
                    moveCount++;
                }
                
            }
        }
        return moveCount;
    }
    
    
    int miniMaxsearch(int depth)
    {
        int value;
        int bestValue = 0;
        int moveCount = 0;
        int i;
        
        Move moveList[9];
        
        if (isWin() == COM || isWin() == MAN)
        {
            return evaluteMap();
        }
        
        //if depth equal zero,return value
        if (depth == 0)
        {
            return evaluteMap();
        }
        
        
        if (COM == player) {
            bestValue = -MAX_NUM;
        }
        else if (MAN == player)
        {
            bestValue = MAX_NUM;
        }
        
        
        moveCount = getMoveList(moveList);
        
        for (i = 0; i < moveCount; i++)
        {
            Move curMove = moveList[i];
            
            makeMove(curMove);
            value = miniMaxsearch(depth - 1);
            unMakeMove(curMove);
            
            if (player == COM)
            {
                if (value > bestValue)
                {
                    bestValue = value;
                    if (depth == currentDepth)
                    {
                        bestMove = curMove;
                    }
                }
            }
            else if (player == MAN)
            {
                if (value < bestValue)
                {
                    bestValue = value;
                    if (depth == currentDepth)
                    {
                        bestMove = curMove;
                    }
                }
            }
            
        }
        
        return bestValue;
    }
    
    virtual bool manPlay()
    {
        Move move = recvMove(clnt_sock);
        board[move.x][move.y] = MAN;
        step++;
        currentDepth--;
        
        if (player == isWin()) {
            bestMove.setMessage(MAN);
            sendMove(bestMove, clnt_sock);
            
            
            return false ;
        }
        else if(10 == step)
        {
            bestMove.setMessage(DRAW);
            sendMove(bestMove, clnt_sock);
            
            return false ;
            
        }
        player = (player == COM) ? MAN : COM;
        return true ;
    }
    
    
    
    virtual bool comPlay()
    {
        miniMaxsearch(currentDepth);
        board[bestMove.x][bestMove.y] = COM;
        step++;
        currentDepth--;
        
        if (player == isWin()) {
            bestMove.setMessage(COM);
            sendMove(bestMove, clnt_sock);
            return false ;
        }
        
        else if(10 == step)
        {
            bestMove.setMessage(DRAW);
            sendMove(bestMove, clnt_sock);
            return false;
        }
        
        sendMove(bestMove, clnt_sock);
        player = (player == COM) ? MAN : COM;
        return true ;
        
    }
    
    
    void disconnect()
    {
        close(serv_sock);
        close(clnt_sock);
    }
    
    
};



#endif /* Board_hpp */
