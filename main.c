#include"Header1.h"
int main()
{
    FILE* fp1=fopen("ami49.blocks","r");
    if(fp1!=NULL)
    {
        printf("\n File open successful");
    }
    Net* net_list=(Net*)calloc(1000,sizeof(Net));
    if(net_list!=NULL)
    {
        printf("\n Declaration sucessfull");
        free(net_list);
        printf("\n Memory freed");
    }
    else{
        printf("\n Declaration unsucessfull");
    }
    Read_Blocks(fp1);
    return 0;
}
