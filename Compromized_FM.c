#include"Header1.h"
void Compromized_FM(int** Cost,Gain* gain_list,Block* bk_list,Net* net_list,Tier* tier_list,int B,int N,int T)
{
    int* heap_size=(int*)calloc(1,sizeof(int));
    heap_size[0]=T*B-1;
    build_gain_heap(gain_list,heap_size[0]);
    int i,j,ret_index=Extract_Heap(gain_list,heap_size);
    Gain ele=gain_list[ret_index];
    while(ret_index>=0 && ele.gain_value>0)
    {
        int pre_tier=ele.tier_index;
        if(place_block(tier_list,bk_list,ele.bk_index,ele.tier_index,pre_tier))
        {
            Net_Component* tem=bk_list[ele.bk_index].net_ptr;
            while(tem!=NULL)
            {
                update_net_list(net_list,tem->net_index,ele.bk_index,ele.tier_index);
                tem=tem->right;
            }
            for(i=0;i<B;i++)
            {
                bk_list[i].Current_Cost=calculate_block_cost(bk_list,net_list,i,bk_list[i].tier);
            }
            for(i=0;i<B;i++)
            {
                for(j=0;j<T;j++)
                {
                    Cost[i][j]=calculate_block_cost(bk_list,net_list,i,j);
                }
            }
        }

    }
}
int Extract_Heap(Gain* gain_list,int* heap_size)
{
    if(heap_size[0]<0)
    {
        return -1;
    }
    Gain ret_val=gain_list[0];
    gain_list[0]=gain_list[heap_size[0]];
    gain_list[0].current_index=0;
    gain_list[heap_size[0]]=ret_val;
    gain_list[heap_size[0]].current_index=heap_size[0];
    heap_size[0]-=1;
    Max_Heapify_Gain(gain_list,0,heap_size[0]);
    return ret_val.current_index;
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
    int j;
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
            gain_list[ele_index].current_index=j;
            gain_list[j].current_index=ele_index;
            ele_index=j;
            j=ele_index*2+1;
        }
    }
}
