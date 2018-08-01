#include"Header1.h"
void Read_Blocks(FILE* fp1)
{
    int i,j,B,T,cnt=0;
    char name[10];
    fscanf(fp1,"%d",&B);
    printf("\n Number of Blocks : %d",B);
    fscanf(fp1,"%d",&T);
    printf("\n Number of Terminals : %d",T);
    Block* bk_list=(Block*)calloc(B, sizeof(Block));
    printf("\n value of B = %d",B);
    while(cnt<B)
    {
        fscanf(fp1,"%s",name);
        if(name[0]=='b' && name[1]=='k')
        {
            j=0;
            while(name[j+2]!='\0')
            {
                bk_list[cnt].name[j]=name[j+2];
                j++;
            }
            bk_list[cnt].name[j]='\0';
            fscanf(fp1,"%s%d",name,&bk_list[cnt].area);
            cnt++;
        }
    }
    printf("\n Value of cnt =%d",cnt);
    printf("\n value of B=%d",B);
    for(i=0;i<B;i++)
    {
        bk_list[i].index=i;
    }
    printf("\n value of B=%d",B);
    for(i=0;i<B;i++)
    {
        printf("\n Block Name=%s\t Block Area=%d\t Block index=%d",bk_list[i].name,bk_list[i].area,bk_list[i].index);
    }
    fclose(fp1);
    free(bk_list);
    return;
}


