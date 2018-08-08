#include"Header1.h"
void Read_Nets(FILE* fp1,Block* bk_list,int B)
{
    printf("\n Inside Read net");
    int i,j,N,tot_pin,deg,cnt=0;
    fscanf(fp1,"%d",&N);
    Net* net_list=(Net*)calloc(N,sizeof(Net));
    fscanf(fp1,"%d",&tot_pin);
    char str[10];
    while(!feof(fp1))
    {
        if(feof(fp1))
        {
            break;
        }
        fscanf(fp1,"%s",str);
        if(str[0]=='N' && str[1]=='e')
        {
            fscanf(fp1,"%s%d",str,&deg);
            net_list[cnt].no_of_bk=0;
            net_list[cnt].degree=deg;
            i=0;
            while(i<deg)
            {
                fscanf(fp1,"%s",str);
                if(str[0]=='b' && str[1]=='k')
                {
                    i++;
                    net_list[cnt].no_of_bk++;
                    j=0;
                    char name[10];
                    while(str[j+2]!='\0')
                    {
                        name[j]=str[j+2];
                        j++;
                    }
                    name[j]='\0';
                    j=search_block(bk_list,0,B-1,name);
                    insert_bk_component(net_list,cnt,j);
                }
                else if(str[0]=='G')
                {
                    i++;
                    net_list[cnt].gnd=true;
                }
                else if(str[0]=='P' && str[1]=='O')
                {
                    i++;
                    net_list[cnt].pwr=true;
                }
                else if(str[0]=='V')
                {
                    i++;
                    net_list[cnt].V=true;
                }
                else if(str[0]=='P')
                {
                    i++;
                    net_list[cnt].pad=true;
                }
            }
        cnt++;
        }
    }
    printf("\n Reading Done");
    FILE* fp=fopen("Net_Details.txt","w");
    if(fp!=NULL)
    {
        printf("\n Net_Details file opened");
    }
    for(j=0;j<N;j++)
    {
        fprintf(fp,"\nThe Net Degree=%d\t Number of Blocks=%d",net_list[j].degree,net_list[j].no_of_bk);
        fprintf(fp,"\nGND pin=%d\tPOW pin=%d",net_list[j].gnd,net_list[j].pwr);
        fprintf(fp,"\n\tBlock components are:");
        print_bk_component(fp,net_list,j);
        //printf("\n\tTerminal components are:");
        //print_ter_component(net_list[j].ter_ptr);

    }
    fclose(fp);
    printf("\n Net Details Printed");
    for(i=0;i<N;i++)
    {
        Block_Component* tem=net_list[i].bk_ptr;
        while(tem!=NULL)
        {
            insert_net_component(bk_list,tem->bk_index,i);
            tem=tem->right;
        }
    }
    printf("\n Net components inserted");
    /*for(i=0;i<B;i++)
    {
        Net_Component* tem=bk_list[i].net_ptr;
        while(tem!=NULL)
        {
            insert_adj_bk_component(bk_list,net_list,tem->net_index,i);
            tem=tem->right;
        }
    }*/
    fp=fopen("Block_details.txt","w+");
    for(i=0;i<B;i++)
    {
        fprintf(fp,"\n Block_Name=%s\t Block_Index=%d",bk_list[i].name,bk_list[i].index);
        fprintf(fp,"\n Net List=");
        print_net_component(fp,bk_list,i);
        //fprintf(fp,"\nTotal_adj_blocks=%d\t Adjacency Blocks=",bk_list[i].no_of_adj_bk);
        //print_adj_bk_component(fp,bk_list,i);
    }
    fclose(fp);
    float relaxation=0.05;
    fp=fopen("ami33_output_with_min_restriction.txt","w");
    for(j=2;j<=5;j++)
    {
        for(i=1;i<=5;i++)
        {
            Initial_Partition(fp,bk_list,net_list,B,N,j,i*relaxation);
        }
    }
  //  Simulated_Initialize(fp,bk_list,net_list,B,N);
    for(i=0;i<N;i++)
    {
        free_block_components(net_list[i]);

    }
    for(i=0;i<B;i++)
    {
        free_net_components(bk_list[i]);
    }
    free(net_list);
    free(bk_list);
    return;

}

void free_net_components(Block ele)
{
    Net_Component* tem=ele.net_ptr;
    if(tem!=NULL)
    {
        free_net_component(tem);
    }
}
void free_net_component(Net_Component* ele)
{
    if(ele->right!=NULL)
    {
        free_net_component(ele->right);
    }
    free(ele);
    return;
}

void initialize_net_list(Net* net_list,int N)
{
    int i;
    for(i=0;i<N;i++)
    {
        if(net_list[i].gnd || net_list[i].pwr || net_list[i].V)
        {
            net_list[i].low_tier.tier_index=0;
            net_list[i].low_tier.bk_count=0;
        }
        else{
            net_list[i].low_tier.tier_index=INT_MAX;
            net_list[i].low_tier.bk_count=0;
        }
        net_list[i].top_tier.tier_index=-1;
        net_list[i].top_tier.bk_count=0;
    }
}
void update_net_list(Net* net_list,Block* bk_list,int net_index,int bk_index,int tier_cnt,int pre_tier)
{
    if(net_list[net_index].low_tier.tier_index>tier_cnt)
    {
        net_list[net_index].low_tier.tier_index=tier_cnt;
        net_list[net_index].low_tier.bk_count=1;
    }
    if(net_list[net_index].top_tier.tier_index<tier_cnt)
    {
        net_list[net_index].top_tier.tier_index=tier_cnt;
        net_list[net_index].top_tier.bk_count=1;
    }
    if(net_list[net_index].low_tier.tier_index==tier_cnt)
    {
        net_list[net_index].low_tier.bk_count++;
    }
    if(net_list[net_index].top_tier.tier_index==tier_cnt)
    {
        net_list[net_index].top_tier.bk_count++;
    }
    if(net_list[net_index].low_tier.tier_index<tier_cnt)
    {
        if(!(net_list[net_index].gnd || net_list[net_index].pad || net_list[net_index].pwr || net_list[net_index].V))
        {
            if(net_list[net_index].low_tier.bk_count==1 && net_list[net_index].low_tier.tier_index==pre_tier)
            {
                Block_Component* tem=net_list[net_index].bk_ptr;
                int least_tier;
                int least_cnt=0;
                if(tem!=NULL)
                {
                    least_tier=bk_list[tem->bk_index].tier;
                    while(tem!=NULL)
                    {
                        if(bk_list[tem->bk_index].tier<least_tier)
                        {
                            least_tier=bk_list[tem->bk_index].tier;
                            least_cnt=1;
                        }
                        if(bk_list[tem->bk_index].tier==least_tier)
                        {
                            least_cnt++;
                        }
                        tem=tem->right;
                    }
                }
                net_list[net_index].low_tier.tier_index=least_tier;
                net_list[net_index].low_tier.bk_count=least_cnt;
            }
        }
    }
    if(net_list[net_index].top_tier.tier_index>tier_cnt)
    {
        if(net_list[net_index].top_tier.bk_count==1 && net_list[net_index].top_tier.tier_index==pre_tier)
        {
            Block_Component* tem=net_list[net_index].bk_ptr;
            int highest_tier;
            int highest_cnt=0;
            if(tem!=NULL)
            {
                highest_tier=bk_list[tem->bk_index].tier;
                while(tem!=NULL)
                {
                    if(bk_list[tem->bk_index].tier>highest_tier)
                    {
                        highest_tier=bk_list[tem->bk_index].tier;
                        highest_cnt=1;
                    }
                    if(bk_list[tem->bk_index].tier==highest_tier)
                    {
                        highest_cnt++;
                    }
                    tem=tem->right;
                }
            }
            net_list[net_index].top_tier.tier_index=highest_tier;
            net_list[net_index].top_tier.bk_count=highest_cnt;
        }
    }

}

/*void update_net_list(Net* net_list,int net_index,int bk_index,int tier_cnt)
{
    if(net_list[net_index].low_tier.tier_index>tier_cnt)
    {
        net_list[net_index].low_tier.tier_index=tier_cnt;
        net_list[net_index].low_tier.bk_count=0;
        free_net_tier_block_components(net_list[net_index].low_tier);
        insert_net_tier_block_components(net_list[net_index].low_tier,bk_index);
    }
    if(net_list[net_index].top_tier.tier_index<tier_cnt)
    {
        net_list[net_index].top_tier.tier_index=tier_cnt;
        net_list[net_index].top_tier.bk_count=0;
        free_net_tier_block_components(net_list[net_index].top_tier);
        insert_net_tier_block_components(net_list[net_index].top_tier,bk_index);
    }
    if(net_list[net_index].low_tier.tier_index==tier_cnt)
    {
        insert_net_tier_block_components(net_list[net_index].low_tier,bk_index);
    }
    if(net_list[net_index].top_tier.tier_index==tier_cnt)
    {
        insert_net_tier_block_components(net_list[net_index].top_tier,bk_index);
    }
    if(net_list[net_index].low_tier.tier_index<tier_cnt)
    {
        Block_Component* tem=net_list[net_index].low_tier.bk_ptr;
        Block_Component* pre=NULL;
        while(tem!=NULL)
        {
            if(tem->bk_index==bk_index)
            {
                if(pre==NULL)
                {
                    if(!(net_list[net_index].gnd || net_list[net_index].pad || net_list[net_index].pwr || net_list[net_index].V))
                    {
                        net_list[net_index].low_tier.tier_index=tier_cnt;
                        net_list[net_index].low_tier.bk_count=0;
                        free_net_tier_block_components(net_list[net_index].low_tier);
                        insert_net_tier_block_components(net_list[net_index].low_tier,bk_index);
                        break;
                    }
                    else{
                        net_list[net_index].low_tier.bk_ptr=NULL;
                        free(tem);
                    }
                }
                else{
                    pre->right=tem->right;
                    free(tem);
                }
                net_list[net_index].low_tier.bk_count--;
                break;
            }
            pre=tem;
            tem=tem->right;
        }
    }
    if(net_list[net_index].top_tier.tier_index>tier_cnt)
    {
        Block_Component* tem=net_list[net_index].top_tier.bk_ptr;
        Block_Component* pre=NULL;
        while(tem!=NULL)
        {
            if(tem->bk_index==bk_index)
            {
                if(pre==NULL)
                {
                    if(!(net_list[net_index].gnd || net_list[net_index].pad || net_list[net_index].pwr || net_list[net_index].V))
                    {
                        net_list[net_index].top_tier.tier_index=tier_cnt;
                        net_list[net_index].top_tier.bk_count=0;
                        free_net_tier_block_components(net_list[net_index].top_tier);
                        insert_net_tier_block_components(net_list[net_index].top_tier,bk_index);
                        break;
                    }
                    else{
                        net_list[net_index].top_tier.bk_ptr=NULL;
                        free(tem);
                    }
                }
                else{
                    pre->right=tem->right;
                    free(tem);
                }
                net_list[net_index].top_tier.bk_count--;
                break;
            }
            pre=tem;
            tem=tem->right;
        }
    }
    return;
}*/

int claculate_MIV(Net* net_list,int N,int T)
{
    int i,MIV=0;
    for(i=0;i<N;i++)
    {
        MIV+=net_list[i].top_tier.tier_index-net_list[i].low_tier.tier_index;
    }
    return MIV;
}

void custom_update_net_list(Net* net_list,Block* bk_list,int N,int B,int T)
{
    int i;
    for(i=0;i<N;i++)
    {
        if(net_list[i].top_tier.tier_index<0 || net_list[i].low_tier.tier_index>=T)
        {
            Block_Component* tem=net_list[i].bk_ptr;
            while(tem!=NULL)
            {
                update_net_list(net_list,bk_list,i,tem->bk_index,bk_list[tem->bk_index].tier,-1);  //Before Custom Update all the previous tier of the blocks was by default -1.
                tem=tem->right;
            }
        }
    }
}

void insert_net_component(Block* bk_list,int bk_index,int net_index)
{
    Net_Component* tem=(Net_Component*)calloc(1,sizeof(Net_Component));
    tem->net_index=net_index;
    tem->right=bk_list[bk_index].net_ptr;
    bk_list[bk_index].net_ptr=tem;
    bk_list[bk_index].bk_degree++;
    return;
}

void print_net_component(FILE* fp,Block* bk_list,int bk_index)
{
    Net_Component* tem=bk_list[bk_index].net_ptr;
    while(tem!=NULL)
    {
        fprintf(fp,"\nnet_component=%d",tem->net_index);
        tem=tem->right;
    }
    return;
}

int cost(Net* net_list,int net_index,int bk_index,int tier_no)
{
    if(net_list[net_index].top_tier.tier_index<tier_no)
    {
        return (tier_no-net_list[net_index].low_tier.tier_index);
    }
    if(net_list[net_index].low_tier.tier_index>tier_no)
    {

        return (net_list[net_index].top_tier.tier_index-tier_no);
    }
    if(net_list[net_index].top_tier.tier_index>tier_no)
    {
        if(net_list[net_index].top_tier.bk_count==1)
        {
            if(net_list[net_index].top_tier.bk_ptr->bk_index==bk_index)
            {
                return (tier_no-net_list[net_index].low_tier.tier_index);
            }
        }
    }
    if(net_list[net_index].low_tier.tier_index<tier_no)
    {
        if(!(net_list[net_index].gnd || net_list[net_index].pad || net_list[net_index].pwr || net_list[net_index].V) && net_list[net_index].low_tier.bk_count==1)
        {
            if(net_list[net_index].low_tier.bk_ptr->bk_index==bk_index)
            {
                return (net_list[net_index].top_tier.tier_index-tier_no);
            }
        }
    }
    return (net_list[net_index].top_tier.tier_index-net_list[net_index].low_tier.tier_index);

}

void free_block_components(Net ele)
{
    Block_Component* tem=ele.bk_ptr;
    if(tem!=NULL)
    {
        free_block_component(tem);
    }

}

void insert_net_tier_block_components(Net_Tier_Component ele,int bk_index)
{
    if(ele.bk_ptr==NULL)
    {
        ele.bk_ptr=(Block_Component*)calloc(1,sizeof(Block_Component));
        ele.bk_ptr->bk_index=bk_index;
    }
    else{
        Block_Component* tem=ele.bk_ptr;
        while(tem->right!=NULL)
        {
            tem=tem->right;
        }
        tem->right=(Block_Component*)calloc(1,sizeof(Block_Component));
        tem->right->bk_index=bk_index;
    }
    ele.bk_count++;
}
