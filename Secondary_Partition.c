#include"Header1.h"
void Secondary_Partition(Net* net_list,Block* bk_list,Tier* tr_list,int N,int B,int T)
{
    int i,j;
    int** Cost=(int**)calloc(B,sizeof(int*));
    int* Pre_Cost=(int*)calloc(B,sizeof(int));
    for(i=0;i<B;i++)
    {
        Pre_Cost[i]=calculate_block_cost(bk_list,net_list,i,bk_list[i].tier);
    }
    for(i=0;i<B;i++)
    {
        Cost[i]=(int*)calloc(T,sizeof(int));
    }
    for(i=0;i<B;i++)
    {
        for(j=0;j<T;j++)
        {
            Cost[i][j]=calculate_block_cost(bk_list,net_list,i,j);
        }
    }
    Gain* gain_list=(Gain*)calloc(T*B,sizeof(Gain));
    calculate_gain_list(Cost,gain_list,Pre_Cost,B,T);


}