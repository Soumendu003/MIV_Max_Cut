#include"Header1.h"
void Compromized_FM(int** Cost,Gain* gain_list,Block* bk_list,Net* net_list,Tier* tier_list,int B,int N,int T)
{
    int i,j,ret_index;
    int* heap_size=(int*)calloc(1,sizeof(int));
    heap_size[0]=T*B-1;
    build_gain_heap(gain_list,bk_list,heap_size[0]);
    ret_index=Extract_Heap(gain_list,bk_list,heap_size);
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
            calculate_gain_list(Cost,gain_list,bk_list,B,T);
            build_gain_heap(gain_list,bk_list,heap_size[0]);
        }
        else{
        }
        ret_index=Extract_Heap(gain_list,bk_list,heap_size);
        ele=gain_list[ret_index];
    }
    printf("\n Placements Done");
    claculate_MIV(net_list,N,T);
    FILE* fp=fopen("Final_Block_Placement.txt","w");
    for(i=0;i<B;i++)
    {
        fprintf(fp,"\n Block_Name=%s\t Block_Index=%d\t Block_Tier=%d",bk_list[i].name,bk_list[i].index,bk_list[i].tier);
    }
    fclose(fp);
}
int Extract_Heap(Gain* gain_list,Block* bk_list,int* heap_size)
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
    Max_Heapify_Gain(gain_list,bk_list,0,heap_size[0]);
    return (heap_size[0]+1);
}


void build_gain_heap(Gain* gain_list,Block* bk_list,int last_index)
{
    int i;
    for(i=(last_index-1)/2;i>=0;i--)
    {
        Max_Heapify_Gain(gain_list,bk_list,i,last_index);
    }
    return;
}

void Max_Heapify_Gain(Gain* gain_list,Block* bk_list,int ele_index,int last_index)
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
            else if(gain_list[j+1].gain_value==gain_list[j].gain_value)
            {
                if(bk_list[gain_list[j+1].bk_index].bk_degree>bk_list[gain_list[j].bk_index].bk_degree)
                {
                    j=j+1;
                }
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
        else if(gain_list[ele_index].gain_value==gain_list[j].gain_value)
        {
            if(bk_list[gain_list[ele_index].bk_index].bk_degree<bk_list[gain_list[ele_index].bk_index].bk_degree)
            {
                Gain tem=gain_list[ele_index];
                gain_list[ele_index]=gain_list[j];
                gain_list[j]=tem;
                gain_list[ele_index].current_index=j;
                gain_list[j].current_index=ele_index;
            }
            return;
        }
        else{
            return;
        }
    }
}
