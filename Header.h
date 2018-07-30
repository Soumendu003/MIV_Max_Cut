#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
typedef struct block Block;
struct block{
    char name[10];
    int index;
    int area;
    int tier;
};

typedef struct terminal Terminal;
struct terminal{
    char name[10];
    int index;
    int tier;
};
typedef struct bk_com Block_Component;
struct bk_com{
    Block* Add;
    Block_Component* Next;
};
typedef struct tr_com Terminal_Component;
struct tr_com{
    Terminal* Add;
    Terminal* Next;
};
typedef struct net Net;
struct net{
    int index;
    int degree;
    int top_tier;
    int low_tier;
    bool gnd;
    bool pwr;
    bool pad;
    bool V;
    Block_Component* bk_ptr;
    Terminal_Component* ter_ptr;
};
void Read_Nets(FILE* fp1,Block* bk_list,int B);
void Read_Blocks(FILE* fp1)
{
    int i,B,T,cnt=0,tr_cnt=0;
    char* str=(char*)calloc(10,sizeof(char));
    int area;
    fscanf(fp1,"%d",&B);
    Block* bk_list=(Block*)calloc(B,sizeof(Block));
    fscanf(fp1,"%d",&T);
    while(!feof(fp1))
    {
         fscanf(fp1,"%s",str);
         if(str[0]=='b' && str[1]=='k')
         {
             i=0;
             while(str[i+2]!='\0')
             {
                 bk_list[cnt].name[i]=str[i+2];
                 i++;
             }
             bk_list[cnt].name[i]='\0';
             fscanf(fp1,"%s%d",str,&area);
             bk_list[cnt].area=area;
             bk_list[cnt].index=cnt;
             cnt++;
         }
         /*else if(str[0]=='G' || str[0]=='P' || str[0]=='V')
         {
             i=0;
             while(str[i+2]!='\0')
             {
                 tr_list[tr_cnt].name[i]=str[i+2];
                 i++;
             }
             tr_list[tr_cnt].name[i]='\0';
             tr_list[tr_cnt].index=tr_cnt;
             tr_cnt++;
         }*/
    }
    free(str);
    fclose(fp1);
    fp1=fopen("Nets.txt","r");
    Read_Nets(fp1,bk_list,B);
    return;
}
void print_bk_component(Block_Component* ptr);
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
                if(str[0]=='b')
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
                    if(str[0]=='b')
                    {
                        for(j=0;j<B;j++)
                        {
                            if(bk_list[j].name==name)
                            {
                                printf("\t Block Name=%s",name);
                                Block_Component* bk=(Block_Component*)calloc(1,sizeof(Block_Component));
                                bk->Add=&bk_list[j];
                                bk->Next=net_list[cnt].bk_ptr;
                                net_list[cnt].bk_ptr=bk;
                                break;
                            }
                        }
                    }
                }
                if(str[0]=='G')
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
        print_bk_component(net_list[j].bk_ptr);
        //printf("\n\tTerminal components are:");
        //print_ter_component(net_list[j].ter_ptr);

    }

}

void print_bk_component(Block_Component* ptr)
{
    if(ptr==NULL)
    {
        return;
    }
    else{
        print_bk_component(ptr->Next);
        printf("\n\tCom=%s",ptr->Add->name);
        return;
    }
}

/*print_ter_component(Terminal_Component* ptr)
{
    if(ptr==NULL)
    {
        return;
    }
    else{
        print_ter_component(ptr->Next);
        printf("\n\tCom=%s",ptr->Add->name);
        return;
    }
}*/

