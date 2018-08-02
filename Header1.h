#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<limits.h>
typedef struct net_com Net_Component;
struct net_com{
    int net_index;
    Net_Component* left;
    Net_Component* right;
};
typedef struct block Block;
struct block{
    char name[10];
    int index;
    int area;
    int tier;
    Net_Component* net_ptr;
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
typedef  struct tier Tier;
struct tier{
    double rem_area;
    int tot_bk;
    Block_Component* bk_com;
};
typedef struct gain Gain;
struct gain{
    int bk_index;
    int net_index;
    int tier_index;
    int gain_value;
};
void Read_Nets(FILE* fp1,Block* bk_list,int B);
void Read_Blocks(FILE* fp1);
int search_block(Block* bk_list,int lwr,int uppr,char* name);
void insert_bk_component(Net* bk_list,int index,int bk_index);
void print_bk_component(FILE* fp,Net* net_list,int j);
double Calculate_Total_Area(Block* bk_list,int B);
void Initial_Partition(Block* bk_list,Net* net_list,int B,int N);
void default_blocks_placement(Block* bk_list,int B);
void initialize_net_list(Net* net_list,int N);
void update_net_list(Net* net_list,int net_index,int tier_cnt);
int place_block(Tier* tier_list,Block* bk_list,Net* net_list,int net_index,int bk_index,int tier_cnt);
void claculate_MIV(Net* net_list,int N,int T);
void custom_update_net_list(Net* net_list,Block* bk_list,int N,int B,int T);
void print_net_component(FILE* fp,Block* bk_list,int bk_index);
int cost(Net* net_list,int net_index,int tier_no);
void calculate_gain_list(int** Cost,Gain* gain_list,int* Pre_Cost,int B,int T);
int calculate_block_cost(Block* bk_list,Net* net_list,int bk_index,int tier_no);
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



