#include<stdio.h>
#include<stdlib.h>
#include"findway.h"


void InitQueue(LinkQueue *Q)
{
    Q->front = Q->rear = (QueuePtr)malloc(sizeof(Qnode));     //指向头节点，不储存信息
    if(!Q->front)    //分配失败
    {
        overflow_box(500, 500);
    }
    Q->front->next = NULL;
}

void DestroyQueue(LinkQueue *Q)     //从队头开始往队尾走，一个个释放
{
    while(Q->front)  //队还不空
    {
        Q->rear = Q->front->next;
        free(Q->front);
        Q->front = Q->rear;
    }
}

int IsEmpty(LinkQueue Q)
{
    if(Q.front == Q.rear)    //队头、队尾重合，就是队空
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void EnQueue(LinkQueue *Q, QElemtype e) //e插入队尾
{
    QueuePtr p;
    p = (QueuePtr)malloc(sizeof(Qnode));
    if(!p)
    {
        overflow_box(500, 500);
    }
    p->data = e;
    p->next = NULL;
    Q->rear->next = p;    //连接到队尾
    Q->rear = Q->rear->next;          //队尾往后移动
}

void DeQueue(LinkQueue *Q, QElemtype *e)
{
    QueuePtr p;
    if(Q->front == Q->rear)
    {
        return ;        //队空，删除失败，退出函数
    }
    p = Q->front->next;
    *e = p->data;        //保存要出队的数据
	Q->front->next = p->next;    //队首后移
	if(Q->rear == p)     //删除之后，队空
    {
        Q->rear = Q->front;
    }
    free(p);    //删除以后就应该释放原结点
}
/*********队的基本操作结束，下面是图*************/

int CreateGraph(Graph G)
{
    int i, j, k = 0;
    LNode *p = NULL, *last = NULL;
    int accessible[16][19]={
                            {1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                            {1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                            {1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
                            {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
                            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                            {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                            {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0},
                            {1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
                           };
/* int accessible[19][16]={
                            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                            {1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1},
                            {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                            {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},       //头不能在墙壁上一格，不然有点怪
                            {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                            {1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0},
                            {1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0},
                            {1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1},
                            {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1},
                            {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1},
                            {1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1},
                            {1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1},
                            {1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                          };
    /*建立邻接链表来储存图的信息*/
    for(i = 0; i < 16; i++) //第一行都是墙壁，肯定不通，不浪费时间去遍历了
    {
        for(j = 1; j < 19; j++)
        {
            if(!accessible[i][j])   //这个点可以通过，则加到图中。不能通过就舍弃
            {
                //给G[k]初始化，k自增
                G[k].x = i;
                G[k].y = j;
                G[k].next = NULL;
                last = NULL;
                //依次判断右边、左边、上面、下面可不可以通行
                if(i < 15 && !accessible[i+1][j])    //右边有方格且可以向右通行
                {
                    p = (LNode *)malloc(sizeof(LNode));
                    p->x = i+1;
                    p->y = j;
                    p->direction = 1;
                    p->next = NULL;
                    if(!last)   //这是第一个结点
                    {
                        G[k].next = p;
                    }
                    else
                    {
                        last->next = p;
                    }
                    last = p;   //不管是不是第一个结点，last都得指向p，因为last指向最后一个
                }

                if(i > 1 && !accessible[i-1][j])    //左边有方格且可以向右通行
                {
                    p = (LNode *)malloc(sizeof(LNode));
                    p = (LNode *)malloc(sizeof(LNode));
                    p->x = i-1;
                    p->y = j;
                    p->direction = 2;
                    p->next = NULL;
                    if(!last)   //这是第一个结点
                    {
                        G[k].next = p;
                    }
                    else
                    {
                        last->next = p;
                    }
                    last = p;   //不管是不是第一个结点，last都得指向p，因为last指向最后一个
                }

                if(j > 1 && !accessible[i][j-1])    //上面
                {
                    p = (LNode *)malloc(sizeof(LNode));
                    p = (LNode *)malloc(sizeof(LNode));
                    p->x = i;
                    p->y = j-1;
                    p->direction = 3;
                    p->next = NULL;
                    if(!last)   //这是第一个结点
                    {
                        G[k].next = p;
                    }
                    else
                    {
                        last->next = p;
                    }
                    last = p;   //不管是不是第一个结点，last都得指向p，因为last指向最后一个
                }

                if(j < 18 && !accessible[i][j+1])    //下面
                {
                    p = (LNode *)malloc(sizeof(LNode));
                    p = (LNode *)malloc(sizeof(LNode));
                    p->x = i;
                    p->y = j+1;
                    p->direction = 4;
                    p->next = NULL;
                    if(!last)   //这是第一个结点
                    {
						G[k].next = p;
                    }
                    else
                    {
                        last->next = p;
                    }
                    last = p;   //不管是不是第一个结点，last都得指向p，因为last指向最后一个
                }

                k++;
            }
        }
    }
    return k;   //返回G中元素的个数
}

int FindAdjVex(Graph G, int n, int k, QElemtype *adjvex) //找V在G中的邻接点，储存在adjvex[4]中，放的是x，y坐标
{    //adjvex中第一个分量是向右的邻接点，第二个向左，以此类推。n是图中点的个数，k是邻接表中的下标
    int i, j;       //i用来表示adjvex的下标，j表示LocateVex的返回值
    LNode *p = NULL;
    Axis V;

    p = G[k].next;
    if(!p)      //没有邻接点
    {
        return 0;       //图里有这个点，但它没邻接点
    }

    while(p)   //p非空，没到末尾
    {
        i = p->direction - 1;    //按方向存
        V.x = p->x;
        V.y = p->y;
        p = p->next;
        j = LocateVex(G, n, V);
      /*if(j == -1)     //G里面没V这个点（其实不可能发生这种情况）
        {
            return 1;   //没找到的特殊处理
        }*/
        adjvex[i] = j;
    }
    return 1;           //找到了邻接点
}

int LocateVex(Graph G, int n, Axis V)
{
    int i;
    for(i = 0; i < n; i++)
    {
        if(G[i].x == V.x && G[i].y == V.y)
        {
            return i;
        }
    }

    return -1;  //没找到
}

/********得到一个path之后，先入栈再出栈，就得到了我们想要的从V0到Vt的路径********/
/***************逐步debug后发现path的构建过程没有任何问题，但最终输出确有问题**********/
int FindWay(Graph G, PathType *path, int n, Axis V0, Axis Vt) //V0是起点，Vt是终点
{
    int i, j, w, *dist, v0, vt;   //v0，vt是V0和Vt在G中的下标，w是中间点的下标
    LinkQueue Q;
    QElemtype k, adjvex[4]; //adjvex里面存的是下标，是要入队出队的，因此属于队的元素类型
    InitQueue(&Q);

    v0 = LocateVex(G, n, V0);
    vt = LocateVex(G, n, Vt);
    if(v0 == -1 || vt == -1)        //起始点、终止点错误，不玩了直接退出函数，啥也不干
    {
        return 0;       //error
    }
    dist = (int *)malloc(n * sizeof(int));
    for(i = 0; i < n; i++)
    {
        dist[i] = -1;
        path[i].former = -1;
    }
    dist[v0] = 0;

    EnQueue(&Q, v0);
    while(!IsEmpty(Q))
    {   //adjvex重新初始化
        adjvex[0] = -1;
        adjvex[1] = -1;
        adjvex[2] = -1;
        adjvex[3] = -1;
        DeQueue(&Q, &k);    //k是队首元素，而队里放的是在图G中的编号
        if(FindAdjVex(G, n, k, adjvex)) //没邻接点的话直接跳掉
        {
            for(j = 0; j < 4; j++)
            {
                if((w = adjvex[j]) != -1)   //说明这条路通，k有w这个邻接点
                {
                    if(dist[w] == -1)       //说明这条路不但是通的，还没被访问过
                    {
                        dist[w] = dist[k] + 1;  //V0到W的最短距离是 V0到K的最短距离+1
                        path[w].former = k;          //w的路径上有k这一点
                        path[w].direction = j + 1;  //记录这是由k代表的点向右走一步来的，还是向左来的，等等
                        EnQueue(&Q, w);
                    }
                    /*********邻接表的建立或者是查找邻接点都是对的，最后没改变到位可能是move函数有问题*******/
                    if(w == vt)         //找到终点了，不浪费时间继续找了
                    {
                        path[v0].former=-1;        //debug结果显示，起点的path居然不是-1了，暂时不知道原因，先强制设为-1
                        DestroyQueue(&Q);   //使用完毕，释放以节约内存
                        free(dist);
                        dist = NULL;
                        return 1;;
                    }
                }
            }
        }
    }
}

/****栈****/
/*只需要把path逆序，不需要其他操作，因此只写四个函数*/
void InitStack(LkStack *S)
{
    S->bottom = (SNode *)malloc(sizeof(SNode));     //申请头结点
    if(!S->bottom)
    {
        overflow_box(500, 500);
    }
    S->top = S->bottom;       //置空
}

void DestroyStack(LkStack *S)
{
    SNode *p;
    while(S->top != S->bottom)  //栈还没空
    {
        p = S->top;
        S->top = p->next;
        free(p);
    }

}

void Push(LkStack *S, SElemtype e)
{
    SNode *p;
    p = (SNode *)malloc(sizeof(SNode));
    p->data = e;
    p->next = S->top;
    S->top = p;
}

void Pop(LkStack *S, SElemtype *e)
{
    SNode *p;
    p = S->top;
    S->top = p->next;
    *e = p->data;
    free(p);
}








