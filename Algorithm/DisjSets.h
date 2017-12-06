//
// Created by Killua on 2017/10/24.
//

#ifndef UNTITLED_DISJSETS_H
#define UNTITLED_DISJSETS_H

class DisjSets
{
public:
    explicit DisjSets(int numElements);

    int find(int x)const;
    int find(int x);

    void unionSets(int root1, int root2);

private:
    vector<int> s;
};

DisjSets::DisjSets(int numElements) : s(numElements)
{
    for(int i = 0; i < s.size(); i++)
    {
        s[i] = -1;
    }
}

void DisjSets::unionSets(int root1, int root2)
{
    s[root2] = root1;
}

int DisjSets::find(int x) const
{
    if(s[x] < 0)
    {
        return x;
    }
    else
    {
        return find(s[x]);
    }
}

#endif //UNTITLED_DISJSETS_H
