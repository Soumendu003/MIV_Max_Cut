#include <stdio.h>
#include <stdlib.h>
#include"Header.h"
int main()
{
    int i,B,N,P;
    printf("\n Enter No of blocks:");
    scanf("%d",&B);
    printf("\n Enter No of Nets:");
    scanf("%d",&N);
    FILE* fp1=fopen("Blocks.txt","r+");
    Block* bk_list=(Block*)calloc(B,sizeof(Block));
    Read_Blocks(fp1,bk_list);
    for(i=0;i<B;i++)
    {
        printf("\nBlock_Name=%s\t Block_Area=%f\t Block_Index=%d",bk_list[i].name,bk_list[i].area,bk_list[i].index);
    }
    fclose(fp1);
    return 0;
}
