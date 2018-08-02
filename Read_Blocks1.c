#include"Header1.h"
void Read_Blocks(FILE* fp1)
{
    int i,B,T,cnt=0;
    char* str=(char*)calloc(10,sizeof(char));
    int area;
    fscanf(fp1,"%d",&B);
    Block* bk_list=(Block*)calloc(B,sizeof(Block));
    fscanf(fp1,"%d",&T);
    while(!feof(fp1))
    {
         fscanf(fp1,"%s",str);
         if(str[0]=='b' && str[1]=='k')
         {
             i=0;
             while(str[i+2]!='\0')
             {
                 bk_list[cnt].name[i]=str[i+2];
                 i++;
             }
             bk_list[cnt].name[i]='\0';
             fscanf(fp1,"%s%d",str,&area);
             bk_list[cnt].area=area;
             bk_list[cnt].index=cnt;
             cnt++;
         }
         /*else if(str[0]=='G' || str[0]=='P' || str[0]=='V')
         {
             i=0;
             while(str[i+2]!='\0')
             {
                 tr_list[tr_cnt].name[i]=str[i+2];
                 i++;
             }
             tr_list[tr_cnt].name[i]='\0';
             tr_list[tr_cnt].index=tr_cnt;
             tr_cnt++;
         }*/
    }
    free(str);
    fclose(fp1);
    for(i=0;i<B;i++)
    {
        bk_list[i].index=i;
    }
    fp1=fopen("test_nets.txt","r");
    Read_Nets(fp1,bk_list,B);
    fclose(fp1);
    return;
}

int search_block(Block* bk_list,int lwr,int uppr,char* name)
{
    int val=(lwr+uppr)/2;
    if(strcmp(bk_list[val].name,name)==0)
    {
        return val;
    }
    else if(strcmp(bk_list[val].name,name)>0)
    {
        return search_block(bk_list,lwr,val-1,name);
    }
    else{
        return search_block(bk_list,val+1,uppr,name);
    }
}

void insert_bk_component(Net* net_list,int index,int bk_index)
{
    Block_Component* tem=net_list[index].bk_ptr;
    while(tem!=NULL)
    {
        if(tem->bk_index==bk_index)
        {
            return;
        }
        tem=tem->right;
    }
    tem=(Block_Component*)calloc(1,sizeof(Block_Component));
    tem->bk_index=bk_index;
    tem->right=net_list[index].bk_ptr;
    net_list[index].bk_ptr=tem;
    return;
}

void print_bk_component(FILE* fp,Net* net_list,int j)
{
    Block_Component* tem=net_list[j].bk_ptr;
    while(tem!=NULL)
    {
        fprintf(fp,"\nbk_component=%d",tem->bk_index);
        tem=tem->right;
    }
}

double Calculate_Total_Area(Block* bk_list,int B)
{
    int i;
    double area=0;
    for(i=0;i<B;i++)
    {
        area+=bk_list[i].area;
    }
    return area;
}

void default_blocks_placement(Block* bk_list,int B)
{
    int i;
    for(i=0;i<B;i++)
    {
        bk_list[i].tier=-1;
    }
}

int place_block(Tier* tier_list,Block* bk_list,Net* net_list,int net_index,int bk_index,int tier_cnt)
{
    if(tier_list[tier_cnt].rem_area-bk_list[bk_index].area>=0)
    {
        tier_list[tier_cnt].rem_area-=bk_list[bk_index].area;
        bk_list[bk_index].tier=tier_cnt;
        //update_net_list(net_list,net_index,tier_cnt);
        return 1;
    }
    else{
        return 0;
    }
}

int calculate_block_cost(Block* bk_list,Net* net_list,int bk_index,int tier_no)
{
    int Cost=0;
    Net_Component* net_com_ptr;
    net_com_ptr=bk_list[bk_index].net_ptr;
    while(net_com_ptr!=NULL)
    {
        Cost+=cost(net_list,net_com_ptr->net_index,tier_no);
        net_com_ptr=net_com_ptr->right;
    }
    return Cost;
}

void calculate_gain_list(int** Cost,Gain* gain_list,int* Pre_Cost,int B,int T)
{
    int i,j,k=0;
    for(i=0;i<B;i++)
    {
        for(j=0;j<T;j++)
        {
            gain_list[k].bk_index=i;
            gain_list[k].tier_index=j;
            gain_list[k].gain_value=Cost[i][j]-Pre_Cost[i];
            k++;
        }
    }
}
