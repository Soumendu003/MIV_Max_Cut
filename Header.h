#include<stdio.h>
#include<stdlib.h>
typedef struct block Block;
struct block{
    char name[10];
    int index;
    int lock;
    float area;
    float power;
    Block* next;
    Block* pre;
    Block* Hash_Add;
};
void Read_Blocks(FILE* fp1,Block* bk_list)
{
    printf("\n Inside Read Block");
    int cnt=0;
    char b,k;
    while(feof(fp1)!=1)
    {
         fscanf(fp1,"%c%c",&b,&k);
         printf("\n b=%c\tk=%c",b,k);
         if(b=='b' && k=='k');
         {
             fscanf(fp1,"%s%f",&bk_list[cnt].name,&bk_list[cnt].area);
             cnt++;
         }

         fscanf(fp1,"\n");

    }
    return;
}


