//
//  AbstractBoard.h
//  TicTacToeServer
//
//  Created by Tusko on 2017/4/10.
//  Copyright © 2017年 Tusko. All rights reserved.
//

#ifndef AbstractBoard_h
#define AbstractBoard_h

#define EMPTY 0
#define COM -1
#define MAN  1
#define DRAW 0
#define STEP 9
#define ROW 3
#define COL 3
#define BUFFER_SIZE   1024

typedef struct  _Move
{
    int x  ;
    int y  ;
    char mesg[50] ;
    
    _Move()
    {
        x = -1 ;
        y = -1 ;
        memset(mesg,0,sizeof(mesg));
    }
    
    _Move(int x,int y)
    {
        this->x = x ;
        this->y = y ;
    }
    
    void setMessage(int state)
    {
        if(MAN == state)
        {
            memcpy(mesg, "you win!\n", sizeof(mesg));
            std::cout<<"man win"<<std::endl;
        }
        else if(COM == state)
        {
            memcpy(mesg, "computer win!\n", sizeof(mesg));
            std::cout<<"computer"<<std::endl;
        }
        else
        {
            memcpy(mesg, "draw !\n", sizeof(mesg));
            std::cout<<"draw"<<std::endl;
        }
        
    }
    
}Move;


class AbstractBoard {
    
public:
    
    AbstractBoard():currentDepth(9),step(1),manFirst(false)
    {}
    
    virtual~AbstractBoard(){}
    
    int player ;
    int currentDepth  ;
    int step ;
    bool manFirst ;
    int board[3][3] = {0};
    
    virtual bool manPlay() = 0;
    virtual bool comPlay() = 0;
    
    void printBoard() {
        int i, j;
        
        if(player == COM)
        {
            std::cout<<"man plays:"<<std::endl;
        }
        else if(player == MAN)
        {
            std::cout<<"computer plays:"<<std::endl;
        }
        
        for (i = 0; i < COL; i++)
        {
            printf("-------------\n");
            for (j = 0; j < ROW; j++)
            {
                if (board[i][j] == COM)
                {
                    printf("| X ");
                }
                else if (board[i][j] == MAN)
                {
                    printf("| O ");
                }
                else
                {
                    printf("|   ");
                }
                
            }
            printf("|\n");
        }
        printf("-------------\n");
        
    }
    
    Move recvMove(int &clnt_sock)
    {
        
        Move result ;
        char buffer[BUFFER_SIZE] = {0} ;
        
        
        while (1) {
            if(recv(clnt_sock, buffer, BUFFER_SIZE, 0))
            {
                break ;
            }
        }
        
        memcpy(&result,buffer,sizeof(Move));
        
        return result ;
    }
    
    void sendMove(const Move &m,int &clnt_sock)
    {
        
        char buffer[BUFFER_SIZE] = {0} ;
        memcpy(buffer,&m,sizeof(Move));
        send(clnt_sock,buffer,sizeof(Move),0);
        
    }
    
};

#endif /* AbstractBoard_h */
