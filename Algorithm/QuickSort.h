//
// Created by Killua on 2017/10/24.
//

#ifndef UNTITLED_QUICKSORT_H
#define UNTITLED_QUICKSORT_H

#include <vector>
using namespace std;

template <typename Comparable>
void quickSort(vector<Comparable & a>)
{
    quickSort(a, 0, a.size() - 1);
}

//三点中值法
template <typename Comparable>
const Comparable & median3(vector<Comparable> & a, int left, int right) {
    int center = (left + right) / 2;
    if (a[center] < a[left]) {
        swap(a[left], a[center]);
    }
    if (a[right] < a[left]) {
        swap(a[left], a[right]);
    }
    if (a[right] < a[center]) {
        swap(a[center], a[right]);
    }

    //将枢纽放在最后一个位置
    swap(a[center], a[right - 1]);
    return a[right - 1];
}

template <typename Comparable>
void quickSort(vector<Comparable> & a, int left, int right)
{
    if(left + 10 <= right)
    {
        Comparable pivot = median3(a, left, right);

        int i = left;
        int j = right - 1;

        for(;;)
        {
            while(a[++i] < pivot){}
            while(pivot < a[--j]){}
            if(i < j)
            {
                swap(a[i],a[j]);
            } else{
                break;
            }
        }

        swap(a[i],a[right - 1]);

        quickSort(a, left, i - 1);
        quickSort(a, i + 1, right);

    } else{
        insertionSort(a,left,right); //插入排序 小数组
    }
}

#endif //UNTITLED_QUICKSORT_H
