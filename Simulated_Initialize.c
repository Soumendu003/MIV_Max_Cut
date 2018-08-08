#include"Header1.h"
void Simulated_Initialize(FILE* fp1,Block* bk_list,Net* net_list,int B,int N)
{
    printf("\n Inside Simulated Initialize");
    srand((unsigned int)time(NULL));
    int i,j;
    float a=1;
    clock_t start=clock();
    for(i=0;i<B;i++)
    {
        float r=((float)rand()/(float)(RAND_MAX)) * a;
        if(r>=0.5)
        {
            place_simulated_block(bk_list,net_list,i,1);
        }
        else{
            place_simulated_initial_block(bk_list,net_list,i,0);
        }
    }
    clock_t end=clock();
    printf("\n Initial Partition Ends");
    fprintf(fp1,"\n***********************************************************************************************");
    fprintf(fp1,"\n");
    fprintf(fp1,"\n***********************************************************************************************");
    fprintf(fp1,"\n\nAfter Initialization MIV count=%d",claculate_MIV(net_list,N,2));
    double time_taken=(double)(end-start)/CLOCKS_PER_SEC;
    fprintf(fp1,"\nTime Taken to execute Initialize function:%0.6lf",time_taken);
    //FILE* fp=fopen("Simulated_placement_leger.txt","w");

    int no_of_steps=20000;
    for(i=0;i<10;i++)
    {
        /*fprintf(fp,"\n***********************************************************************************************");
        fprintf(fp,"\n");
        fprintf(fp,"\n***********************************************************************************************");
        fprintf(fp,"\n\nFor No of steps=%d\n\n",no_of_steps);*/
        Simulated_Annealtion(fp1,bk_list,net_list,B,N,60000,no_of_steps);
        printf("\n Simulated Annelation with no of steps=%d completes",no_of_steps);
        no_of_steps+=5000;
    }

}

void place_simulated_initial_block(Block* bk_list,Net* net_list,int bk_index,int tier_no)
{
    if(tier_no==1)
    {
        bk_list[bk_index].tier=1;
        Net_Component* tem=bk_list[bk_index].net_ptr;
        while(tem!=NULL)
        {
            net_list[tem->net_index].top_tier.tier_index=1;
            net_list[tem->net_index].top_tier.bk_count++;
            tem=tem->right;
        }
    }
    else if(tier_no==0)
    {
        bk_list[bk_index].tier=0;
        Net_Component* tem=bk_list[bk_index].net_ptr;
        while(tem!=NULL)
        {
            net_list[tem->net_index].low_tier.tier_index=0;
            net_list[tem->net_index].low_tier.bk_count++;
            tem=tem->right;
        }
    }
}

void place_simulated_block(Block* bk_list,Net* net_list,int bk_index,int tier_no)
{
    int pre_tier=(tier_no+1)%2;
    if(tier_no==1)
    {
        bk_list[bk_index].tier=1;
        Net_Component* tem=bk_list[bk_index].net_ptr;
        while(tem!=NULL)
        {
            int net_index=tem->net_index;
            if(!(net_list[net_index].gnd || net_list[net_index].pad || net_list[net_index].pwr || net_list[net_index].V))
            {
                if(net_list[net_index].low_tier.bk_count==1)         //because pre_tier will be 0 here
                {
                    net_list[net_index].low_tier.tier_index=1;
                    net_list[net_index].low_tier.bk_count=net_list[net_index].degree;
                }
                else{
                    net_list[net_index].low_tier.bk_count-=1;
                }
            }
            else
            {
                net_list[net_index].low_tier.bk_count-=1;
            }
            if(net_list[net_index].top_tier.tier_index==0)
            {
                net_list[net_index].top_tier.tier_index=1;
                net_list[net_index].top_tier.bk_count=1;
            }
            else{
                net_list[net_index].top_tier.bk_count++;
            }
            tem=tem->right;
        }
    }
    else{
        bk_list[bk_index].tier=0;
        Net_Component* tem=bk_list[bk_index].net_ptr;
        while(tem!=NULL)
        {
            int net_index=tem->net_index;
            if(net_list[net_index].top_tier.bk_count==1)
            {
                net_list[net_index].top_tier.tier_index=0;
                net_list[net_index].top_tier.bk_count=net_list[net_index].degree;
            }
            else{
                net_list[net_index].top_tier.bk_count-=1;
            }
            if(net_list[net_index].low_tier.tier_index==1)
            {
                net_list[net_index].low_tier.tier_index=0;
                net_list[net_index].low_tier.bk_count=1;
            }
            else{
                net_list[net_index].low_tier.bk_count=+1;
            }
            tem=tem->right;
        }
    }
}
