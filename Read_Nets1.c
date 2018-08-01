#include"Header1.h"
void Read_Nets(FILE* fp1,Block* bk_list,int B)
{
    printf("\n Inside Read Nets");
    int i,j,N,tot_pin,deg,cnt=0;
    fscanf(fp1,"%d",&N);
    printf("\nNumber of Nets=%d",N);
    Net* net_list=(Net*)calloc(N,sizeof(Net));
    fscanf(fp1,"%d",&tot_pin);
    printf("\nNumber of Total Pin=%d",tot_pin);
    char* str=(char*)calloc(20,sizeof(char));
    while(!feof(fp1))
    {
        fscanf(fp1,"%s",str);
        if(strcmp(str,"NetDegree"))
        {
            printf("\n Found One Net");
            fscanf(fp1,"%s%d",str,&deg);
            printf("\t Degree of Net=%d",deg);
            net_list[cnt].degree=deg;
            cnt++;
            i=0;
            while(i<deg)
            {
                fscanf(fp1,"%s",str);
                if(str[0]=='b' && str[1]=='k')
                {
                    printf("\n Found One Block");
                    i++;
                    j=0;
                    char name[10];
                    while(str[j+2]!='\0')
                    {
                        name[j]=str[j+2];
                        j++;
                    }
                    name[j]='\0';
                    //j=search_block(bk_list,B/2,0,name);
                    //insert_bk_component(net_list[cnt].bk_ptr);
                }
                else if(str[0]=='G')
                {
                    net_list[cnt].gnd=true;
                }
                else if(str[0]=='P' && str[1]=='O')
                {
                    net_list[cnt].pwr=true;
                }
                else if(str[0]=='V')
                {
                    net_list[cnt].V=true;
                }
                else{
                    net_list[cnt].pad=true;
                }
            }
        }
    }
    for(j=0;j<N;j++)
    {
        printf("\nThe Net Degree=%d",net_list[j].degree);
        printf("\n\tBlock components are:");
       // print_bk_component(net_list[j].bk_ptr);
        //printf("\n\tTerminal components are:");
        //print_ter_component(net_list[j].ter_ptr);

    }

}




