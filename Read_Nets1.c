#include"Header1.h"
void Read_Nets(FILE* fp1,Block* bk_list,int B)
{
    int i,j,N,tot_pin,deg,cnt=0;
    fscanf(fp1,"%d",&N);
    Net* net_list=(Net*)calloc(N,sizeof(Net));
    fscanf(fp1,"%d",&tot_pin);
    char str[10];
    while(!feof(fp1))
    {
        if(feof(fp1))
        {
            break;
        }
        fscanf(fp1,"%s",str);
        if(str[0]=='N' && str[1]=='e')
        {
            printf("\n Value of Count=%d",cnt);
            fscanf(fp1,"%s%d",str,&deg);
            net_list[cnt].degree=deg;
            i=0;
            while(i<deg)
            {
                fscanf(fp1,"%s",str);
                if(str[0]=='b' && str[1]=='k')
                {
                    i++;
                    j=0;
                    char name[10];
                    while(str[j+2]!='\0')
                    {
                        name[j]=str[j+2];
                        j++;
                    }
                    name[j]='\0';
                    j=search_block(bk_list,0,B-1,name);
                    printf("\n Value returned:%d",j);
                    insert_bk_component(net_list[cnt],j);
                }
                else if(str[0]=='G')
                {
                    i++;
                    net_list[cnt].gnd=true;
                }
                else if(str[0]=='P' && str[1]=='O')
                {
                    i++;
                    net_list[cnt].pwr=true;
                }
                else if(str[0]=='V')
                {
                    i++;
                    net_list[cnt].V=true;
                }
                else if(str[0]=='P')
                {
                    i++;
                    net_list[cnt].pad=true;
                }
            }
        cnt++;
        }
    }
    printf("\n Reading done");
    for(j=0;j<N;j++)
    {
        printf("\nThe Net Degree=%d",net_list[j].degree);
        printf("\nGND pin=%b\tPOW pin=%b",net_list[j].gnd,net_list[j].pwr);
        printf("\n\tBlock components are:");
        print_bk_component(net_list[j]);
        //printf("\n\tTerminal components are:");
        //print_ter_component(net_list[j].ter_ptr);

    }
    free(net_list);
    return;

}




