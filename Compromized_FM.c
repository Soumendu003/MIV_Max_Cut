#include"Header1.h"
void Compromized_FM(FILE* fp1,int** Cost,Gain* gain_list,Block* bk_list,Net* net_list,Tier* tier_list,int B,int N,int T)
{
    int i,j,ret_index;
    int* heap_size=(int*)calloc(1,sizeof(int));
    heap_size[0]=T*B-1;
    build_gain_heap(gain_list,bk_list,heap_size[0]);
    ret_index=Extract_Heap(gain_list,bk_list,heap_size);
    Gain ele=gain_list[ret_index];
    clock_t start_time=clock();
    while(ret_index>=0 && ele.gain_value>0)
    {
        int pre_tier=bk_list[ele.tier_index].tier;
        if(place_block(tier_list,bk_list,ele.bk_index,ele.tier_index,pre_tier))
        {
            Net_Component* tem=bk_list[ele.bk_index].net_ptr;
            while(tem!=NULL)
            {
                update_net_list(net_list,bk_list,tem->net_index,ele.bk_index,ele.tier_index,pre_tier);
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
            heap_size[0]=T*B-1;
            build_gain_heap(gain_list,bk_list,heap_size[0]);
        }
        ret_index=Extract_Heap(gain_list,bk_list,heap_size);
        if(ret_index<0)
        {
            break;
        }
        ele=gain_list[ret_index];
    }
    clock_t end_time=clock();
    printf("\n Placements Done");
    fprintf(fp1,"\nAt End Total Number of MIV after Compromised FM=%d",claculate_MIV(net_list,N,T));
    double time_taken=(double)(end_time-start_time)/CLOCKS_PER_SEC;
    fprintf(fp1,"\nTime Taken to execute Compromized FM:%0.6lf",time_taken);
    start_time=clock();
    Min_area_coverage(bk_list,net_list,tier_list,B,N,T);
    end_time=clock();
    fprintf(fp1,"\nAt End Total Number of MIV after Min area coverage=%d",claculate_MIV(net_list,N,T));
    time_taken=(double)(end_time-start_time)/CLOCKS_PER_SEC;
    fprintf(fp1,"\nTime Taken to execute Min area Coverage:%0.6lf",time_taken);
    FILE* fp=fopen("Final_Block_Placement.txt","w");
    for(i=0;i<B;i++)
    {
        fprintf(fp,"\n Block_Name=%s\t Block_Index=%d\t Block_Tier=%d",bk_list[i].name,bk_list[i].index,bk_list[i].tier);
    }
    fclose(fp);
    double tot_area=0;
    for(i=0;i<B;i++)
    {
        tot_area+=bk_list[i].area;
    }
    fprintf(fp1,"\n Total Area of all the blocks =%lf\n",tot_area);
    tot_area=0;
    for(i=0;i<T;i++)
    {
        tot_area+=tier_list[i].area_consumed;
    }
    fprintf(fp1,"\nTotal area of tiers=%lf\n",tot_area);
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

void Min_area_coverage(Block* bk_list,Net* net_list,Tier* tier_list,int B,int N,int T)
{
    int i,j;
    int* heap_size=(int*)calloc(1,sizeof(int));
    int* Cost=(int*)calloc(B,sizeof(int));
    Gain* gain_list=(Gain*)calloc(B,sizeof(Gain));
    for(i=0;i<T;i++)
    {
        if(tier_list[i].area_consumed<tier_list[i].min_area)
        {
            for(j=0;j<B;j++)
            {
                Cost[j]=calculate_block_cost(bk_list,net_list,j,i);
            }
            calculate_tier_gain_list(Cost,gain_list,bk_list,B);
            heap_size[0]=B-1;
            build_gain_heap(gain_list,bk_list,heap_size[0]);
            int ret_index=Extract_Heap(gain_list,bk_list,heap_size);
            Gain ele=gain_list[ret_index];
            while(tier_list[i].area_consumed<tier_list[i].min_area && ret_index>=0)
            {
                int pre_tier=bk_list[ele.bk_index].tier;
                if(i!=pre_tier && place_block(tier_list,bk_list,ele.bk_index,i,pre_tier))
                {
                    Net_Component* tem=bk_list[ele.bk_index].net_ptr;
                    while(tem!=NULL)
                    {
                        update_net_list(net_list,bk_list,tem->net_index,ele.bk_index,i,pre_tier);
                        tem=tem->right;
                    }
                    for(j=0;j<B;j++)
                    {
                        bk_list[j].Current_Cost=calculate_block_cost(bk_list,net_list,j,bk_list[j].tier);
                    }
                    calculate_tier_gain_list(Cost,gain_list,bk_list,B);
                    heap_size[0]=B-1;
                    build_gain_heap(gain_list,bk_list,heap_size[0]);
                }
                ret_index=Extract_Heap(gain_list,bk_list,heap_size);
                if(ret_index<0)
                {
                    break;
                }
                ele=gain_list[ret_index];
            }
        }
    }
    free(gain_list);
}

void calculate_tier_gain_list(int* Cost,Gain* gain_list,Block* bk_list,int B)
{
    int i;
    for(i=0;i<B;i++)
    {
        gain_list[i].gain_value=Cost[i]-bk_list[i].Current_Cost;
        gain_list[i].bk_index=i;
    }
    return;
}
