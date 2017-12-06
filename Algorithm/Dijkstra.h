//
//  Dijkstra.h
//  Algorrithm
//
//  Created by Killua on 2017/9/28.
//  Copyright © 2017年 Tusko. All rights reserved.
//

#ifndef Dijkstra_h
#define Dijkstra_h

template<typename T>
struct Vertex
{
    std::map<int,int> adj;
    bool known;
    int path;
    T dist;
    int index;
};
#endif /* Dijkstra_h */



#include <stdio.h>
#include <list>
#include <iostream>
#include <vector>
#include <map>
#include "Dijkstra.h"
using namespace std;


template<typename T>
void printPath(Vertex<T> v,const vector<Vertex<T>> &vertexs)
{
  if(v.path == 0)
  {
      return ;
  }
        printPath(vertexs[v.path-1],vertexs);
    
       if(v.path != 1) cout << " to ";
    
       cout << v.path;
    
    
}


template<typename T>
Vertex<T> findSmallestDist(const vector<Vertex<T>> vertexs)
{
    T smallestDist = (T)10000;
    Vertex<T> smallV;
    smallV.dist = 10000;
    for(auto &v : vertexs)
    {
        if(!v.known)
        {
            if(smallestDist > v.dist)
            {
                smallV = v;
                smallestDist = v.dist;
            }
        }
    }
    return smallV;
}

template<typename T>
Vertex<T> dijkstra(vector<Vertex<T>> &vertexs,Vertex<T> s)
{

    for(auto & v : vertexs)
    {
        v.dist = 10000;
        v.known = false;

    }
    
    vertexs[0].dist = 0;
    
    for(;;)
    {
        Vertex<T> v = findSmallestDist(vertexs);
        if(v.dist == 10000)
            break;

        vertexs[v.index-1].known = true;
        map<int,int> m = v.adj;
        
        for(auto itr = m.begin(); itr != m.end() ;itr++)
        {
            int index = itr->first;
            if(!vertexs[index].known)
            {
                if(v.dist + itr->second < vertexs[index].dist)
                {
                    vertexs[index].dist = v.dist + itr->second;
                    vertexs[index].path = v.index;
                }
            }
        }
    }
    return vertexs[0];
}

void run()
{
    Vertex<int> v1;
    map<int,int> adj1;
    adj1[1] = 2;
    adj1[3] = 1;
    
    
    Vertex<int> v2;
    map<int,int> adj2;
    adj2[4] = 10;
    adj2[3] = 3;
    
    Vertex<int> v3;
    map<int,int> adj3;
    adj3[0] = 4;
    adj3[5] = 5;
    
    Vertex<int> v4;
    map<int,int> adj4;
    adj4[2] = 2;
    adj4[4] = 2;
    adj4[6] = 4;
    
    
    Vertex<int> v5;
    map<int,int> adj5;
    adj5[6] = 6;
    
    Vertex<int> v6;
    map<int,int> adj6;
    
    Vertex<int> v7;
    map<int,int> adj7;
    adj7[5] = 1;
    
    v7.adj = adj7;
    v6.adj = adj6;
    v5.adj = adj5;
    v4.adj = adj4;
    v3.adj = adj3;
    v2.adj = adj2;
    v1.adj = adj1;
    
    vector<Vertex<int>> vertexs;
    vertexs.push_back(v1);
    vertexs.push_back(v2);
    vertexs.push_back(v3);
    vertexs.push_back(v4);
    vertexs.push_back(v5);
    vertexs.push_back(v6);
    vertexs.push_back(v7);
    
    for(int i = 0; i < vertexs.size(); i++)
    {
        vertexs[i].index = i+1;
    }
    v1.index = 1;
    Vertex<int> v0 = dijkstra(vertexs, v1);
    
    printPath(vertexs[5],vertexs);
}
