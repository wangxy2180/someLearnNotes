#include "../include/static_sort.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "../include/timer.h"
#include <random>
using namespace std;

int main()
{
    const int NUM=1000;
    vector<int> a(NUM);
    for(int i = 0;i<NUM;++i)
    {
        a[i]=rand()%NUM+1;
    }
    vector<int> b(a);
    cout<<a[0]<<", "<<b[0]<<endl;

    Timer time_1;
    time_1.start();
    sort(a.begin(),a.end());
    time_1.stop();
    cout<<"time is: "<<time_1.getElapsedMilliseconds()<<endl;

    cout<<a[0]<<", "<<b[0]<<endl;

    StaticTimSort<NUM> my_sort;
    Timer time_2;
    time_2.start();
    my_sort(b);
    time_2.stop();
    cout<<"time is: "<<time_2.getElapsedMilliseconds()<<endl;


}
