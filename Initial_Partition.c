#include"Header1.h"
void Initial_Partition(Block* bk_list,Net* net_list,int B,int N)
{
    int i,j,T,cnt,flag,tier_cnt;
    printf("\n Enter Number of Tiers:");
    scanf("%d",&T);
    double extra_area,avg_area,tot_area=Calculate_Total_Area(bk_list,B);
    avg_area=tot_area/T;
    float relax;
    printf("\n Enter area relaxation percentage:");
    scanf("%f",&relax);
    tot_area=tot_area*(1+relax);
    Tier* tier_list=(Tier*)calloc(T,sizeof(Tier));
    //Initializes Tier Components
    for(i=0;i<T;i++)
    {
        tier_list[i].rem_area=tot_area;
        tier_list[i].tot_bk=0;
    }
    default_blocks_placement(bk_list,B);
    cnt=0;
    tier_cnt=0;
    flag=0;
    for(i=0;i<N;i++)
    {
        Block_Component* tem=net_list[i].bk_ptr;
        while(tem!=NULL)
        {
            int bk_index=tem->bk_index;
            if(bk_list[bk_index].tier==-1)
            {
                int lock=0;
                while(!place_block(bk_list,bk_index,tier_cnt))   //place_blocks returns 1 if it successfully places the bk at tier_cnt
                {
                    lock++;
                    tier_cnt++;
                    tier_cnt=(tier_cnt%T);
                    if(lock>=T)
                    {
                        printf("\n The Block Index=%d is locked",bk_index);
                        flag=1;
                        break;
                    }
                }
                cnt++;
                if(cnt>=B)
                {
                    flag=1;               //indicates that all the blocks are placed
                    break;
                }
            }
            tem=tem->right;
        }
        if(flag!=0)
        {
            break;
        }
    }

}