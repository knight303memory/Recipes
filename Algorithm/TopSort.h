//
// Created by Killua on 2017/11/17.
//

#ifndef UNTITLED_TOPSORT_H
#define UNTITLED_TOPSORT_H
/*
//伪代码
void Graph::topSort()
{
    Queue<Vertex> q;
    int counter = 0;

    q.makeEmpty();
    for each Vertex
    if(v.indegree == 0)
        q.enqueue(v);


    while(!q.isEmpty)
    {
        Vertex v = q.dequeue();
        v.topNum = ++counter;

        for each Vertex w adjacent to w
        if(--w.indegree == 0)
        {
            q.enqueue(w);
        }
    }

    if(counter != NUM_VERTICES)
    {
        throw CycleFoundException();
    }

}
 */
#endif //UNTITLED_TOPSORT_H
