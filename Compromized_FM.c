#include"Header1.h"
void Compromized_FM(Gain* gain_list,Block* bk_list,Net* net_list,Tier* tier_list,int B,int N,int T)
{
    build_gain_heap(gain_list,T*B-1);
}

void build_gain_heap(Gain* gain_list,int last_index)
{
    int i=(last_index-1)/2;
    for(i;i>=0;i--)
    {
        Max_Heapify_Gain(gain_list,i,last_index);
    }
    return;
}

void Max_Heapify_Gain(Gain* gain_list,int ele_index,int last_index)
{
    int i,j;
    j=ele_index*2+1;
    while(j<=last_index)
    {
        if(j+1<=last_index)
        {
            if(gain_list[j+1].gain_value>gain_list[j].gain_value)
            {
                j=j+1;
            }
        }
        if(gain_list[ele_index].gain_value<gain_list[j].gain_value)
        {
            Gain tem=gain_list[ele_index];
            gain_list[ele_index]=gain_list[j];
            gain_list[j]=tem;
            ele_index=j;
            j=ele_index*2+1;
        }
    }
}
