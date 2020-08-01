#include<fcntl.h>
#include<io.h>
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<bios.h>
#include<math.h>
#include"advance.h"
#include"basicgf.h"
#include"bricks.h"
#include"color1.h"
#include"findway.h"
#include"frnture.h"
#include"hzxs.h"
#include"input.h"
#include"manbody.h"
#include"module_b.h"
#include"mouse.h"
#include"myhouse.h"
#include"rbtbody.h"
#include"rbtmove.h"
#include"svgahead.h"
#include"wall.h"
/*每一次移动的最小单位函数*/
void dmove(CASE *case_state,int *mousex,int *mousey, int casetype)    //要改变状态，所以应该传指针，casetype为0是机器人，1是人
{
	int button=0;
	int x0 = case_state->xpixel, y0 = case_state->ypixel;

	//判断方向并移动相应距离（2个像素点）
	if(!casetype)   //是机器人
	{
	    switch(case_state->direction)
        {
            case 1:         //向右走
            {
                if(case_state->catch_th == WITHOUT_THING)   //手上没拿东西
                {
                    robot_right(*case_state);       //画出向右走的图像
                }
                else if(case_state->catch_th == WITH_THING)    //手上有东西
                {
                    right_hold(*case_state);
                }
                x0 = x0 + 2;
                break;
            }
            case 2:         //向左
            {
                if(case_state->catch_th == WITHOUT_THING)   //手上没拿东西
                {
                    robot_left(*case_state);       //画出向左走的图像
                }
                else if(case_state->catch_th == WITH_THING)    //手上有东西
                {
                    left_hold(*case_state);
                }
                x0 = x0 - 2;
                break;
            }
            case 3:         //向上（指屏幕上方），不管拿不拿东西，图像都一样
            {
                if(case_state->catch_th == WITHOUT_THING)   //手上没拿东西
                {
                    backbodyhead(*case_state);
                }
                else if(case_state->catch_th == WITH_THING)
                {
                    back_hold(*case_state);
                }
                y0 = y0-2;
                break;
            }
            case 4:         //向下
            {
                if(case_state->catch_th == WITHOUT_THING)
                {
                    forebodyhead(*case_state);
                }
                else if(case_state->catch_th == WITH_THING)
                {
                    front_hold(*case_state);
                }
                y0 = y0 + 2;
                break;
            }
        }
	}
    else        //是人
    {
        switch(case_state->direction)
        {
            case 1:         //向右走
            {
                man_rightbody(*case_state);
                x0 = x0 + 2;
                break;
            }
            case 2:         //向左
            {
                man_leftbody(*case_state);
                x0 = x0 - 2;
                break;
            }
            case 3:         //向上（指屏幕上方），不管拿不拿东西，图像都一样
            {
                man_backbody(*case_state);
                break;
            }
            case 4:         //向下
            {
                man_forebody(*case_state);
                y0 = y0 + 2;
                break;
            }
        }
	}

	/*保持图像*/
	delay0(50);
	/*****newxy的作用还不是很清楚，put_image和get_image的像素点应该是要改的********/
	newxy(mousex,mousey,&button);     //在新位置处画鼠标
	put_image(case_state->xpixel-35,case_state->ypixel-2,case_state->xpixel+35,case_state->ypixel+80,case_state->cat);
	case_state->xpixel = x0;  //改变位置
	case_state->ypixel = y0;
	case_state->x = case_state->xpixel/40;
	case_state->y = case_state->ypixel/40;
	get_image(case_state->xpixel-35,case_state->ypixel-2,case_state->xpixel+35,case_state->ypixel+80,case_state->cat);
}


/*普通的移动函数,每次移动一格*/
void move0(CASE *case_state,int *mousex,int *mousey, int casetype)
{
	int i;
	put_image(case_state->xpixel-35,case_state->ypixel-2,case_state->xpixel+35,case_state->ypixel+80,case_state->cat);

	/*按一定的间隔改变机器人脚的位置*/
	for(i=0;i<20;i++)
	{
		if(case_state->leg)
		{
			case_state->leg_right=0;
			case_state->leg_left--;
		}
		else
		{
			case_state->leg_left=0;
			case_state->leg_right--;
		}
		if(case_state->leg_left<=-5||case_state->leg_right<=-5)
		{
			case_state->leg = !case_state->leg;
		}
		/*改变手的角度*/
		if(case_state->hand)
		{
			case_state->hand_left++;
			case_state->hand_right--;
		}
		else//初始化时为0，即动右手
		{
			case_state->hand_right++;
			case_state->hand_left--;
		}
		if((case_state->hand_right>=10)||(case_state->hand_left>=10)||(case_state->hand_right<=-10)||(case_state->hand_left<=-10))
		{
			case_state->hand=!case_state->hand;
		}
		//前面这些部分是在改变动作，相当于做一个走路的图画
		//后面调用dmove就是真的在移动了
/*可以这样理解，走路的动画是：“在原地改变动作（即这段注释上面的部分）+不在意动作，给位置平移2个像素点”的叠加*/
		/*调用最小移动单元*/
		dmove(case_state,mousex,mousey, casetype);
	}
	/*更新位置*/
	case_state->x = case_state->xpixel/40;//因为大小关系，给x一个偏移量20
	case_state->y = case_state->ypixel/40;//偏移量50
	case_state->leg = 0;
	case_state->leg_left = 0;
	case_state->leg_right = 0;
    forebodyhead(*case_state);

	//走一步少一格电           //写完充电函数以后会改，至少走十步少一格电
	/*ChangeHouse(25,-1);
	if(CheckHouse(25)<50&&(!CheckHouse(26))){
		charge_main(case_state,x,y);
	}*/
}


/*端水移动函数，put_image和get_image的范围是要挖掉的范围。理论上越大越保险，但是时间开销大*/
/*这个只有机器人有，人没有，不新增参数了*/
void dmove2(CASE *case_state,int *x,int *y,unsigned int *withbottle)
{
	int button=0;
	int x0 = case_state->xpixel,y0 = case_state->ypixel;

	/*判断方向*/
	switch(case_state->direction)
	{
		case 1:
		{
			right_hold(*case_state);
			x0 = x0+2;
			delay0(50);
	        newxy(x,y,&button);
	        put_image(case_state->xpixel-48,case_state->ypixel-80,case_state->xpixel+48,case_state->ypixel-3,withbottle);
	        case_state->xpixel = x0;
	        case_state->ypixel = y0;
	        get_image(case_state->xpixel-48,case_state->ypixel-80,case_state->xpixel+48,case_state->ypixel-3,withbottle);
			break;
		}
		case 2:
		{
			left_hold(*case_state);
			x0 = x0-2;
			delay0(50);
	        newxy(x,y,&button);
	        put_image(case_state->xpixel-48,case_state->ypixel-80,case_state->xpixel+48,case_state->ypixel-3,withbottle);
	        case_state->xpixel = x0;
	        case_state->ypixel = y0;
	        get_image(case_state->xpixel-48,case_state->ypixel-80,case_state->xpixel+48,case_state->ypixel-3,withbottle);
			break;
		}
		case 3:
		{
			back_hold(*case_state);
			y0 = y0-2;
			delay0(50);
	        newxy(x,y,&button);
	        put_image(case_state->xpixel-48,case_state->ypixel-80,case_state->xpixel+48,case_state->ypixel-3,withbottle);
	        case_state->xpixel = x0;
	        case_state->ypixel = y0;
	        get_image(case_state->xpixel-48,case_state->ypixel-80,case_state->xpixel+48,case_state->ypixel-3,withbottle);
			break;
		}
		case 4:
		{
			front_hold(*case_state);
			y0 = y0+2;
			delay0(50);
	        newxy(x,y,&button);
	        put_image(case_state->xpixel-48,case_state->ypixel-80,case_state->xpixel+48,case_state->ypixel-3,withbottle);
	        case_state->xpixel = x0;
	        case_state->ypixel = y0;
	        get_image(case_state->xpixel-48,case_state->ypixel-80,case_state->xpixel+48,case_state->ypixel-3,withbottle);
			break;
		}
	}
}

void move2(CASE *case_state,int *x,int *y,unsigned int *withbottle)//搬动椅子的移动   //改成水杯或者衣服
{
	int i;
	/*改变机器人的脚的位置*/
	for(i=0;i<20;i++)
	{
		if(case_state->leg)
		{
			case_state->leg_right=0;
			case_state->leg_left--;
		}
		else
		{
			case_state->leg_left=0;
			case_state->leg_right--;
		}
		if(case_state->leg_left<=-5||case_state->leg_right<=-5)
		{
			case_state->leg = !case_state->leg;
		}

		if(case_state->hand)
		{
			case_state->hand_left++;
			case_state->hand_right--;
		}
		else//初始化时为0，即动右手
		{
			case_state->hand_right++;
			case_state->hand_left--;
		}
		if((case_state->hand_right>=10)||(case_state->hand_left>=10)||(case_state->hand_right<=-10)||(case_state->hand_left<=-10))
		{
			case_state->hand=!case_state->hand;
		}

		dmove2(case_state,x,y,withbottle);
	}
	case_state->x =   (case_state->xpixel-20)/40;//因为大小关系，给x一个偏移量20
	case_state->y = (case_state->ypixel-4-50)/40;//偏移量50
	case_state->leg = 0;
	case_state->leg_left = 0;
	case_state->leg_right = 0;
}



/*寻路函数，给定起止点自动寻路并到达*/
/********************************************************************
case_state的x、y坐标会到move0，然后再进到demove0，然后才被改变
demove0中，根据方向，移动两个像素点，改变case_state
*********************************************************************/


int aimmove(CASE *case_state,int x0,int y0,int xt,int yt,int *mousex,int *mousey, Graph G, int n, int casetype)//以(0,0)为开始，大像素点
{
	Axis V0, Vt;        //传给findway
	/*path传给findway，然后用入栈S，再把S弹出放回path*/
	PathType *path = NULL, *t = NULL;      //t是入栈出栈时用的暂存变量
	LkStack S;
	int i;

	V0.x = x0, V0.y = y0;
	Vt.x = xt, Vt.y = yt;
	InitStack(&S);
    path = (PathType *)malloc(n*sizeof(PathType));
	if(!path)
	{
		overflow_box(500,500);
		getch();
		exit(1);
	}
	t = (PathType *)malloc(sizeof(PathType));
	if(!t)
	{
		overflow_box(500,500);
		getch();
		exit(1);
	}

	case_state->x = x0;
	case_state->y = y0;

	/*如果起点等于终点*/
	if(x0==xt && y0==yt)
	{
		return 0;
	}

	if(!FindWay(G, path, n, V0, Vt))//坐标计算以(0,0)开始
	{
		//寻路出错
		FindWay_error(500,500);
		getch();
		exit(1);
	}
	else
	{
		/*将path中的元素弹出，压入R中*/
		for(i = LocateVex(G, n, Vt); i != -1; i = path[i].former)     //从终点开始向前回溯，i!=-1说明到达起始点后还要把起始点入栈
		{
		    Push(&S, path[i]);
		}
		/*弹出R中元素，并单步移动*/   /**************出了大问题***********/
		while(S.top != S.bottom)    //栈非空
		{
            Pop(&S, t);
            case_state->direction = t->direction;     //记方向，等下给move0的时候就知道方向，可以做动画，具体坐标不重要
            move0(case_state, mousex, mousey,casetype);
		}
	}
	/*释放堆*/
	free(t);
	DestroyStack(&S);
    free(path);
	/*将指针置为空*/
	path = NULL;
	t = NULL;
	//递归调用，直到到达目的地
	if(case_state->x != xt || case_state->y!= yt)       /*********debug显示，结束一次后，case_state的x、y分别为8、8，这就是出错的原因*******/
	{
		aimmove(case_state, case_state->x, case_state->y, xt, yt, mousex, mousey, G, n, casetype);
	}
}

