#include"Header1.h"
void Read_Blocks(FILE* fp1)
{
    int i,B,T,cnt=0,tr_cnt=0;
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
    FILE* fp=fopen("Block_details.txt","w+");
    for(i=0;i<B;i++)
    {
        fprintf(fp,"\n Block_Name=%s\t Block_Index=%d",bk_list[i].name,bk_list[i].index);
    }
    fp1=fopen("Nets1.txt","r");
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
    Block_Component* tem=(Block_Component*)calloc(1,sizeof(Block_Component));
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
