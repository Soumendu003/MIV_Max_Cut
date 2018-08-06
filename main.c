#include"Header1.h"
int main()
{
    FILE* fp1=fopen("ami49.blocks","r");
    if(fp1!=NULL)
    {
        printf("\n File open successful");
    }
    Read_Blocks(fp1);
    return 0;
}
