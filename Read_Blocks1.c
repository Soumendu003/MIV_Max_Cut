#include"Header1.h"
void Read_Blocks(FILE* fp1)
{
    int i,j,k,B,T,cnt=0;
    char* str=(char*)calloc(50,sizeof(char));
    fscanf(fp1,"%d",&B);
    printf("\n No of Blocks=%d",B);
    Block* bk_list=(Block*)calloc(B,sizeof(Block));
    fscanf(fp1,"%d",&T);
    while(cnt<B)
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
             printf("\n Block Name=%s",bk_list[cnt].name);
             int area;
             fscanf(fp1,"%s%d",str,&area);
             printf("\t Area=%d",area);
             bk_list[cnt].area=area;
             bk_list[cnt].length=bk_list[cnt].width=sqrt(area);
             /*fscanf(fp1,"%[^\n]",str);
             j=0;
             k=0;
             while(str[j]!='\0')
             {
                 if(str[j]=='(')
                 {
                     if(k==2)
                     {
                         j++;
                         int val=0;
                         while(str[j]!=',')
                         {
                             val=val*10+(str[j]-48);
                             j++;
                         }
                         bk_list[cnt].length=val;
                         j+=2;
                         val=0;
                         while(str[j]!=')')
                         {
                             val=val*10+(str[j]-48);
                             j++;
                         }
                         bk_list[cnt].width=val;
                         break;
                     }
                     else{
                        k++;
                     }
                 }
                 j++;
             }*/
             //printf("\n Block Length=%d\t Block Width=%d",bk_list[cnt].length,bk_list[cnt].width);
             bk_list[cnt].area=(bk_list[cnt].length*bk_list[cnt].width);
             bk_list[cnt].index=cnt;
             cnt++;
         }
    }
    //free(str);
    fclose(fp1);
    for(i=0;i<B;i++)
    {
        bk_list[i].index=i;
    }
    fp1=fopen("ami33.nets","r");
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
        bk_list[i].Current_Cost=0;
    }
}

/*int place_block(Tier* tier_list,Block* bk_list,int bk_index,int tier_cnt,int prev_tier)
{
    if(tier_list[tier_cnt].area_consumed+bk_list[bk_index].area<=tier_list[tier_cnt].max_area && prev_tier>=0 )
    {
        tier_list[tier_cnt].area_consumed+=bk_list[bk_index].area;
        tier_list[prev_tier].area_consumed-=bk_list[bk_index].area;
        bk_list[bk_index].tier=tier_cnt;
        //update_net_list(net_list,net_index,tier_cnt);
        return 1;
    }
    else if(prev_tier<0)
    {
        tier_list[tier_cnt].area_consumed+=bk_list[bk_index].area;
        bk_list[bk_index].tier=tier_cnt;
        return 1;
    }
    else{
        return 0;
    }
}*/

int place_block(Tier* tier_list,Block* bk_list,int bk_index,int tier_cnt,int prev_tier)
{
    if(tier_list[tier_cnt].area_consumed+bk_list[bk_index].area<=tier_list[tier_cnt].max_area && prev_tier>=0 && tier_list[prev_tier].area_consumed-bk_list[bk_index].area>=tier_list[tier_cnt].min_area)
    {
        tier_list[tier_cnt].area_consumed+=bk_list[bk_index].area;
        tier_list[prev_tier].area_consumed-=bk_list[bk_index].area;
        bk_list[bk_index].tier=tier_cnt;
        //update_net_list(net_list,net_index,tier_cnt);
        return 1;
    }
    else if(prev_tier<0)
    {
        tier_list[tier_cnt].area_consumed+=bk_list[bk_index].area;
        bk_list[bk_index].tier=tier_cnt;
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
        Cost+=cost(net_list,net_com_ptr->net_index,bk_index,tier_no);
        net_com_ptr=net_com_ptr->right;
    }
    return Cost;
}

void calculate_gain_list(int** Cost,Gain* gain_list,Block* bk_list,int B,int T)
{
    int i,j,k=0;
    for(i=0;i<B;i++)
    {
        for(j=0;j<T;j++)
        {
            gain_list[k].bk_index=i;
            gain_list[k].tier_index=j;
            gain_list[k].gain_value=Cost[i][j]-bk_list[i].Current_Cost;
            k++;
        }
    }
}

void free_block_component(Block_Component* ele)
{
    if(ele->right!=NULL)
    {
        free_block_component(ele->right);
    }
    free(ele);
    return;
}

/*void create_and_link_gain_list(Gain** gain_list,Block* bk_list,int tier_size,int block_cnt)
{
    int i,j,k=0;
    for(i=0;i<block_cnt;i++)
    {
        bk_list[i].gain_list=(Gain**)calloc(tier_size,sizeof(Gain*));
    }
    for(i=0;i<block_cnt;i++)
    {
        for(j=0;j<tier_size;j++)
        {
            gain_list[k]=(Gain*)calloc(1,sizeof(Gain));
            bk_list[i].gain_list[j]=gain_list[k];
            gain_list[k]->current_index=k;
            k++;
        }

    }
}*/

void insert_adj_bk_component(Block* bk_list,Net* net_list,int net_index,int bk_index)
{
    Block_Component* adj_tem=net_list[net_index].bk_ptr;
    while(adj_tem!=NULL)
    {
        if(adj_tem->bk_index!=bk_index)
        {
            insert_adj_component(bk_list,bk_index,adj_tem->bk_index);
        }
        adj_tem=adj_tem->right;
    }
}
void insert_adj_component(Block* bk_list,int bk_index,int adj_bk_index)
{
    Block_Component* tem=bk_list[bk_index].adj_bk_ptr;
    while(tem!=NULL)
    {
        if(tem->bk_index==adj_bk_index)
        {
            return;
        }
        tem=tem->right;
    }
    tem=(Block_Component*)calloc(1,sizeof(Block_Component));
    tem->bk_index=adj_bk_index;
    tem->right=bk_list[bk_index].adj_bk_ptr;
    bk_list[bk_index].adj_bk_ptr=tem;
    bk_list[bk_index].no_of_adj_bk++;
    return;

}
void print_adj_bk_component(FILE* fp,Block* bk_list,int bk_index)
{
    Block_Component* tem=bk_list[bk_index].adj_bk_ptr;
    while(tem!=NULL)
    {
        fprintf(fp,"\nadj_bk_component=%d",tem->bk_index);
        tem=tem->right;
    }
    return;
}
