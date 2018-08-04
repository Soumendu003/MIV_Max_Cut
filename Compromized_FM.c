#include"Header1.h"
void Compromized_FM(Gain** gain_list,Block* bk_list,Net* net_list,Tier* tier_list,int B,int N,int T)
{
    int* heap_size=(int*)calloc(1,sizeof(int));
    heap_size[0]=T*B-1;
    build_gain_heap(gain_list,heap_size[0]);
    Wait_List* wait=(Wait_List*)calloc(T,sizeof(Wait_List));
    Gain* ele=Extract_Heap(gain_list,heap_size);
    while(ele!=NULL && ele->gain_value>0)
    {
        if(place_block(tier_list,bk_list,ele->bk_index,ele->tier_index))
        {

        }

    }
}
Gain* Extract_Heap(Gain** gain_list,int* heap_size)
{
    if(heap_size[0]<0)
    {
        return NULL;
    }
    Gain* ret_val=gain_list[0];
    gain_list[0]=gain_list[heap_size[0]];
    gain_list[0]->current_index=0;
    gain_list[heap_size[0]]=ret_val;
    gain_list[heap_size[0]]->current_index=heap_size[0];
    heap_size[0]-=1;
    Max_Heapify_Gain(gain_list,0,heap_size[0]);
    return ret_val;
}


void build_gain_heap(Gain** gain_list,int last_index)
{
    int i=(last_index-1)/2;
    for(i;i>=0;i--)
    {
        Max_Heapify_Gain(gain_list,i,last_index);
    }
    return;
}

void Max_Heapify_Gain(Gain** gain_list,int ele_index,int last_index)
{
    int j;
    j=ele_index*2+1;
    while(j<=last_index)
    {
        if(j+1<=last_index)
        {
            if(gain_list[j+1]->gain_value>gain_list[j]->gain_value)
            {
                j=j+1;
            }
        }
        if(gain_list[ele_index]->gain_value<gain_list[j]->gain_value)
        {
            Gain* tem=gain_list[ele_index];
            gain_list[ele_index]=gain_list[j];
            gain_list[j]=tem;
            gain_list[ele_index]->current_index=j;
            gain_list[j]->current_index=ele_index;
            ele_index=j;
            j=ele_index*2+1;
        }
    }
}
