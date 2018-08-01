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
            fscanf(fp1,"%s%d",str,&deg);
            net_list[cnt].no_of_bk=0;
            net_list[cnt].degree=deg;
            i=0;
            while(i<deg)
            {
                fscanf(fp1,"%s",str);
                if(str[0]=='b' && str[1]=='k')
                {
                    i++;
                    net_list[cnt].no_of_bk++;
                    j=0;
                    char name[10];
                    while(str[j+2]!='\0')
                    {
                        name[j]=str[j+2];
                        j++;
                    }
                    name[j]='\0';
                    j=search_block(bk_list,0,B-1,name);
                    insert_bk_component(net_list,cnt,j);
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
    FILE* fp=fopen("Net_Details.txt","w");
    for(j=0;j<N;j++)
    {
        fprintf(fp,"\nThe Net Degree=%d\t Number of Blocks=%d",net_list[j].degree,net_list[j].no_of_bk);
        fprintf(fp,"\nGND pin=%d\tPOW pin=%d",net_list[j].gnd,net_list[j].pwr);
        fprintf(fp,"\n\tBlock components are:");
        print_bk_component(fp,net_list,j);
        //printf("\n\tTerminal components are:");
        //print_ter_component(net_list[j].ter_ptr);

    }
    fclose(fp);
    free(net_list);
    return;

}




