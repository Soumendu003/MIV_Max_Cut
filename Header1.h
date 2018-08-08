#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<limits.h>
#include<time.h>
#include<math.h>
typedef struct net_com Net_Component;
struct net_com{
    int net_index;
    Net_Component* right;
};
typedef struct gain Gain;
struct gain{
    int bk_index;
    int tier_index;
    int gain_value;
    int current_index;
};
typedef struct bk_com Block_Component;
struct bk_com{
    int bk_index;
    Block_Component* right;
};
typedef struct block Block;
struct block{
    char name[10];
    int index;
    long area;
    int length;
    int width;
    int tier;
    bool lock;
    int no_of_adj_bk;
    int bk_degree;
    int Current_Cost;
    Block_Component* adj_bk_ptr;
    Net_Component* net_ptr;
};

typedef struct terminal Terminal;
struct terminal{
    char name[10];
    int index;
    int tier;
};

typedef struct tr_com Terminal_Component;
struct tr_com{
    Terminal* Add;
    Terminal* Next;
};
typedef struct net_tier_com Net_Tier_Component;
struct net_tier_com{
    int tier_index;
    int bk_count;
    Block_Component* bk_ptr;
};
typedef struct net Net;
struct net{
    int index;
    int degree;
    Net_Tier_Component top_tier;
    Net_Tier_Component low_tier;
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
    double area_consumed;
    double max_area;
    double min_area;
    int tot_bk;
    Block_Component* bk_com;
};
typedef struct wait Wait_List;
struct wait{
    int wait_bk_cnt;
    Gain* gain_component;
};

void Read_Nets(FILE* fp1,Block* bk_list,int B);
void Read_Blocks(FILE* fp1);
int search_block(Block* bk_list,int lwr,int uppr,char* name);
void insert_bk_component(Net* bk_list,int index,int bk_index);
void print_bk_component(FILE* fp,Net* net_list,int j);
double Calculate_Total_Area(Block* bk_list,int B);
void Initial_Partition(FILE* fp1,Block* bk_list,Net* net_list,int B,int N,int T,float relax);
void default_blocks_placement(Block* bk_list,int B);
void initialize_net_list(Net* net_list,int N);
void update_net_list(Net* net_list,Block* bk_list,int net_index,int bk_index,int tier_cnt,int pre_tier);
int place_block(Tier* tier_list,Block* bk_list,int bk_index,int tier_cnt,int prev_tier);
int claculate_MIV(Net* net_list,int N,int T);
void custom_update_net_list(Net* net_list,Block* bk_list,int N,int B,int T);
void print_net_component(FILE* fp,Block* bk_list,int bk_index);
int cost(Net* net_list,int net_index,int bk_index,int tier_no);
void calculate_gain_list(int** Cost,Gain* gain_list,Block* bk_list,int B,int T);
int calculate_block_cost(Block* bk_list,Net* net_list,int bk_index,int tier_no);
void Secondary_Partition(FILE* fp1,Net* net_list,Block* bk_list,Tier* tr_list,int N,int B,int T);
void Max_Heapify_Gain(Gain* gain_list,Block* bk_list,int ele_index,int last_index);
void build_gain_heap(Gain* gain_list,Block* bk_list,int last_index);
void free_block_components(Net ele);
void free_block_component(Block_Component* ele);
void insert_net_tier_block_components(Net_Tier_Component ele,int bk_index);
void insert_net_component(Block* bk_list,int bk_index,int net_index);
void create_and_link_gain_list(Gain* gain_list,Block* bk_list,int tier_size,int block_cnt);
int Extract_Heap(Gain* gain_list,Block* bk_list,int* heap_size);
void Compromized_FM(FILE* fp1,int** Cost,Gain* gain_list,Block* bk_list,Net* net_list,Tier* tier_list,int B,int N,int T);
void insert_adj_bk_component(Block* bk_list,Net* net_list,int net_index,int bk_index);
void print_adj_bk_component(FILE* fp,Block* bk_list,int bk_index);
void insert_adj_component(Block* bk_list,int bk_index,int adj_bk_index);
void free_net_component(Net_Component* ele);
void free_net_components(Block ele);
void place_simulated_initial_block(Block* bk_list,Net* net_list,int bk_index,int tier_no);
void Simulated_Annealtion(FILE* fp1,Block* bk_list,Net* net_list,int B,int T,float Initial_Temp,int no_of_steps);
void Simulated_Initialize(FILE* fp1,Block* bk_list,Net* net_list,int B,int N);
int calculate_simulated_block_current_cost(Block* bk_list,Net* net_list,int bk_index);
void place_simulated_block(Block* bk_list,Net* net_list,int bk_index,int tier_no);
void Min_area_coverage(Block* bk_list,Net* net_list,Tier* tier_list,int B,int N,int T);
void calculate_tier_gain_list(int* Cost,Gain* gain_list,Block* bk_list,int B);
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



