#include"Header1.h"
void Simulated_Annealtion(FILE* fp1,Block* bk_list,Net* net_list,int B,int N,float Initial_Temp,int no_of_steps)
{
    srand(time(0));
    int i,j,cnt=0;
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
    fprintf(fp1,"\n\nAfter Simulated Annelation MIV count=%d",claculate_MIV(bk_list,net_list,N));
    double tot_area=0;
    for(i=0;i<B;i++)
    {
        tot_area+=bk_list[i].area;
    }
    fprintf(fp1,"\n\n Total Area of all the blocks =%lf\n",tot_area);
    double avg_area=tot_area/2;
    tot_area=0;
    double divergence=0;
    for(i=0;i<2;i++)
    {
        double tier_area=0;
        for(j=0;j<B;j++)
        {
            if(bk_list[j].tier==i)
            {
                tier_area+=bk_list[j].area;
            }
        }
        tot_area+=tier_area;
        divergence=(tier_area-avg_area)/avg_area;
        fprintf(fp1,"\n Divergence of tier area from avg area=%lf",divergence);
    }
    fprintf(fp1,"\n\nTotal area of tiers=%lf\n",tot_area);
    double TWL=0;
    double hwl=0;
    for(i=0;i<2;i++)
    {
        for(j=0;j<N;j++)
        {
            hwl=0;
            Block_Component* tem=net_list[j].bk_ptr;
            while(tem!=NULL)
            {
                if(bk_list[tem->bk_index].tier==i)
                {
                   // printf("\n Block No=%d\t Net_No=%d\tTier_No=%d",tem->bk_index,j,i);
                    hwl+=(bk_list[tem->bk_index].length+bk_list[tem->bk_index].width);

                }
                tem=tem->right;
            }
            //printf("\n HWL=%lf",hwl);
            TWL+=(hwl)*(hwl);
        }
    }
    fprintf(fp1,"\n\n TWL Calculated =%lf\n",TWL );
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
