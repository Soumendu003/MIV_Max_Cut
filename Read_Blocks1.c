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
        printf("\n Block Name=%s\t Block Area=%d",bk_list[i].name,bk_list[i].area);
    }
    //fp1=fopen("Nets.txt","r");
    //Read_Nets(fp1,bk_list,B);
    return;
}
