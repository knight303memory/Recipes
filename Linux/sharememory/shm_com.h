//
// Created by Killua on 2017/11/8.
//

#ifndef UNTITLED_SHM_COM_H
#define UNTITLED_SHM_COM_H

#define TEXT_SZ 2048

struct shared_use_st
{
    int written_by_you;
    char some_text[TEXT_SZ];
};



#endif //UNTITLED_SHM_COM_H
