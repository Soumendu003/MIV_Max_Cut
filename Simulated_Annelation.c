#include"Header1.h"
void Simulated_Annealtion(FILE* fp1,Block* bk_list,Net* net_list,int B,int N,float Initial_Temp,int no_of_steps)
{
    srand(time(0));
    int i,cnt=0;
    clock_t start=clock();
    while(Initial_Temp>=1)
    {
        cnt++;
        for(i=0;i<no_of_steps;i++)
        {
            int move=(rand()%B);
            int next_tier,Next_Cost,Current_Cost=calculate_simulated_block_current_cost(bk_list,net_list,move);
            if(bk_list[move].tier==0)
            {
                place_simulated_block(bk_list,net_list,move,1);
                Next_Cost=calculate_simulated_block_current_cost(bk_list,net_list,move);
                next_tier=0;
            }
            else{
                place_simulated_block(bk_list,net_list,move,0);
                Next_Cost=calculate_simulated_block_current_cost(bk_list,net_list,move);
                next_tier=1;
            }
            int gain=Next_Cost-Current_Cost;
            if(gain<0)
            {
                if(exp(-gain/Initial_Temp)>((float)rand()/(float)(RAND_MAX)))
                {
                    place_simulated_block(bk_list,net_list,move,next_tier);
                }
                else{
                    //fprintf(fp,"\n Block=%d placed at Tier=%d",move,bk_list[move]);
                }
            }
            else{
               // fprintf(fp,"\n Block=%d placed at Tier=%d",move,bk_list[move]);
            }
        }
        Initial_Temp=0.9*Initial_Temp;
    }
    clock_t end=clock();
    fprintf(fp1,"\n\n No of steps per Temperature=%d",no_of_steps);
    fprintf(fp1,"\n\n No of Temperature steps=%d",cnt);
    double time_taken=(double)(end-start)/CLOCKS_PER_SEC;
    fprintf(fp1,"\n\nTime Taken to execute Simulated Annelation :%0.6lf",time_taken);
    fprintf(fp1,"\n\nAfter Simulated Annelation MIV count=%d",claculate_MIV(net_list,N,2));
}

int calculate_simulated_block_current_cost(Block* bk_list,Net* net_list,int bk_index)
{
    int cost=0;
    Net_Component* tem=bk_list[bk_index].net_ptr;
    while(tem!=NULL)
    {
        cost+=net_list[tem->net_index].top_tier.tier_index-net_list[tem->net_index].low_tier.tier_index;
        tem=tem->right;
    }
    return cost;
}

/*int calculate_simulated_block_next_cost(Block* bk_list,Net* net_list,int bk_index)
{
    int next_tier=(bk_list[bk_index].tier+1)%2;
    int cost=0;
    if(next_tier==1)
    {
        Net_Component* tem=bk_list[bk_index].net_ptr;
        while(tem!=NULL)
        {
            int net_index=tem->net_index
            if(!(net_list[net_index].gnd || net_list[tnet_index].pad || net_list[net_index].pwr || net_list[net_index].V))
            {
                if(net_list[net_index].low_tier.bk_count==1)
                {

                }
                else{
                    cost+=1;
                }
            }
            else{
                cost+=1;
            }
            tem=tem->right;
        }
    }
    else
    {
        Net_Component* tem=bk_list[bk_index].net_ptr;
        while(tem!=NULL)
        {
            int net_index=tem->net_index
            if(net_list[net_index].low_tier.tier_index==1)
            {
                cost+=1;
            }
            else
            {
                if(net_list[net_index].top_tier.bk_count==1 && bk_list[bk_index].tier==1)
                {
                    cost+=0;
                }
                else{

                }
            }
        tem=tem->right;
    }
}*/
