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
    int bk_index;
    Block_Component* left;
    Block_Component* right;
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
    int no_of_bk;
    bool gnd;
    bool pwr;
    bool pad;
    bool V;
    Block_Component* bk_ptr;
    Terminal_Component* ter_ptr;
};
void Read_Nets(FILE* fp1,Block* bk_list,int B);
void Read_Blocks(FILE* fp1);
int search_block(Block* bk_list,int lwr,int uppr,char* name);
void insert_bk_component(Net* bk_list,int index,int bk_index);
void print_bk_component(FILE* fp,Net* net_list,int j);
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



