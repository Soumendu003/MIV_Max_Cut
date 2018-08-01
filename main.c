#include"Header1.h"
int main()
{
    int i,B,N,P;
    printf("\n Enter No of blocks:");
    scanf("%d",&B);
    printf("\n Enter No of Nets:");
    scanf("%d",&N);
    FILE* fp1=fopen("Blocks1.txt","r");
    if(fp1!=NULL)
    {
        printf("\n File open sucessful");
    }
    Read_Blocks(fp1);
    return 0;
}
