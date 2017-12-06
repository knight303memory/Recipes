 //无权最短路径伪代码

 void Graph::unweighted(Vertex e
 {
    for each Vertex v
    {
        v.dist = INFINITY;
        v.knwon = false;
    }

    s.dist = 0;

    for(int currDist = 0; currDist < NUM_VERTICES; currDist++)
    {
        for each Vertex v
            if(!v.known && v.dist == currDist)
            {
                v.known = true;
                for each Vertex w adjacent to v
                    if(w.dist == INFINITY)
                    {
                        w.dist = currDist + 1;
                        w.path = v;
                    }

            }
    }
 }
 
 
 //改进 用队列 ，队列只含距离为currDist+1的顶点
 
void Graph::unweighted(Vertex s)
 {
    Queue<Vertex> q;
    for each Vertex v
        v.dist = INFINITY;

    s.dist = 0;
    q.enqueue(s);

    while(!q.empty())
    {
        Vertex v = q.dequeue();

        for each Vertex w adjacent to v
            if(w.dist == INFNITY)
            {
                w.dist = v.dist + 1;
                w.path = v;
                q.enqueue(w);
            }
    }
 }
