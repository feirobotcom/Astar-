#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct Maze
{
	int x;
	int y;
	int g;
	int h;
	int f;  //f=g+h
	struct Maze* next;
	struct Maze* father;
};

struct MAZE_STRU
{
	int size;
	int** data;  //定义二维数组
};

typedef struct Maze* PMaze;
typedef struct Maze* LinkStack;
typedef struct MAZE_STRU MAZE;//定义存储迷宫的数组

LinkStack SetNullStack_link()
{
	LinkStack top = (LinkStack)malloc(sizeof(struct Maze));
	if (top != NULL)
		top->next = NULL;
	else
		printf("Alloc failure"); //内存分配失败
	return top;

}
 
int IsNullStack_link(LinkStack top)  //判断空桟
{
	if (top->next == NULL)
		return 1;
	else
		return 0;
}

void Push_link(LinkStack top, PMaze p)//入栈， PMAZE p表示待入栈的新节点
{
	p->next = top->next;
	top->next = p;
}

void POP(LinkStack top, PMaze precur)  
{//当前位置出栈（将上一个记录在openlist中的min出栈）
	PMaze pre, p;
	pre = top;
	p = top;
	if (p->next != NULL)
	{
		while (p != NULL)
		{
			if (p == precur)  //要移除的节点precur
			{
				pre->next = p->next;  //跳过该节点
				//return 0;
			}
			pre = p;
			p = p->next;
		}
	}
	else {
		printf("openlist已空!\n");
	}
}

LinkStack Top_link(LinkStack sstack)//取栈顶元素
{
	if (IsNullStack_link(sstack))
	{
		printf("it is empty");
		return 0;
	}
	else
		return sstack->next;
}

PMaze InputCoordinate(int x, int y, int g, int h, int f)//将坐标,成本存入指向当前位置的节点
{
	LinkStack top = (LinkStack)malloc(sizeof(struct Maze));
	if (top != NULL)
	{
		top->x = x;
		top->y = y;
		top->g = g;
		top->h = h;
		top->f = f;
		top->next = NULL;
	}
	else
		printf("Alloc failure");
	return top;
}

int Exist(LinkStack head, PMaze p)
{
	PMaze q = head->next;
	while (q != NULL)
	{
		if ((q->x == p->x) && (q->y == p->y)) return 1;  //查找该点是否存在某表中
		q = q->next;
	}
	return 0;
}

PMaze min_pmaze(LinkStack top)  //在堆栈中查找最小f值的元素
{
	PMaze q, p = top->next;
	q = p;
	while (p != NULL)
	{
		if (q->f > p->f)
		{
			q = p;
		}
		if (q->f == p->f)   //如果f相同，则取较小的h值，有利于更快速的搜索
		{
			if (q->h > p->h)
			{
				q = p;
			}
		}
		p = p->next;
	}
	return q;
}


int maze_A(int entryX, int entryY, int exitX, int exitY, struct MAZE_STRU* MAZE)
{
	//A*算法探索迷宫
	//定义了八个方向
	int direction[8][2] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} };
	int posX = entryX, posY = entryY;//临时变量，储存坐标（x,y）
	int preposX = entryX, preposY = entryY;//用于存储前一个位置的坐标（x,y）

	int mov;//移动的方向
	int g, h, f;//临时存储g,h

	PMaze cur;//指向当前位置
	PMaze precur;//存储前一个位置
	PMaze min;//用于存储具有最小的f的位置

	LinkStack openlist = SetNullStack_link();//定义开放列表
	LinkStack closelist = SetNullStack_link();//定义封闭列表

	g = abs(posX - preposX) + abs(posY - preposY);
	h = abs(exitX - posX) + abs(exitY - posY);   //曼哈顿距离
	f = g + h;
	cur = InputCoordinate(entryX, entryY, g, h, f);
	cur->father = NULL;
	Push_link(openlist, cur);
	min = cur;//赋予初始位置为起点位置

	while (!IsNullStack_link(openlist))
	{
		precur = min;
		//printf("((%d %d)g:%d h:%d f:%d)\n", min->x, min->y,min->g,min->h,min->f);
		//上一条注释是用于调试时来查看每一个最小成本的位置的具体信息

		// 打印open表内容
		////printf("Open表: ");
		//PMaze tempOpen = Top_link(openlist);
		//while (tempOpen != NULL) {
		//	//printf("(%d,%d)、", tempOpen->x, tempOpen->y );
		//	tempOpen = tempOpen->next;
		//}
		//printf("\n");

		//// 打印close表内容
		////printf("Close表: ");
		//PMaze tempClose = closelist->next;
		//while (tempClose != NULL) {
		//	//printf("(%d ,%d)、", tempClose->x, tempClose->y);
		//	tempClose = tempClose->next;
		//}
		//printf("\n");
		//printf("\n");

		preposX = precur->x;//记录当前位置的X坐标（下一个位置的前一个位置）
		preposY = precur->y;//记录当前位置的Y坐标（下一个位置的前一个位置）
		POP(openlist, precur);//出栈,入closelist
		Push_link(closelist, precur);
		mov = 0;//设置初始探索方向
		while (mov < 8)
		{
			posX = preposX + direction[mov][0];
			posY = preposY + direction[mov][1];

			if (posX == exitX && posY == exitY)//openlist中有出口，则表示到达终点
			{
				g = abs(posX - preposX) + abs(posY - preposY);
				h = abs(exitX - posX) + abs(exitY - posY);
				f = g + h;
				cur = InputCoordinate(exitX, exitY, g, h, f);
				cur->father = precur;
				Push_link(openlist, cur);//进入openlist
				printf("\nA*算法搜索出的最优解为:\n");
				PMaze temp = Top_link(openlist);
				while (temp->father != NULL)
				{
					
					printf("(%d, %d)<- ", temp->x, temp->y);
					MAZE->data[temp->x][temp->y] = 2;
					
					temp = temp->father;
				}

				

				printf("(%d, %d) ", entryX, entryY);
				printf("\n");
				return 1;
			}
			g = abs(posX - preposX) + abs(posY - preposY);
			h = abs(exitX - posX) + abs(exitY - posY);
			f = g + h;
			cur = InputCoordinate(posX, posY, g, h, f);
			if (!(MAZE->data[posX][posY]) && !Exist(closelist, cur)) //判断是否是墙体,是否在closelist中
			{
				if (!Exist(openlist, cur))//不在则将当前位置存入openlist
				{
					cur->father = precur;
					Push_link(openlist, cur);//进栈
				}
				else
				{//若当前点到min的位置短于到原父亲节点的位置的距离，则将当前位置的父节点更改为min
					if (cur->g < precur->g)
					{
						cur->father = min;
					}
				}//否则什么也不做
			}
			mov++;
		}
		min = min_pmaze(openlist);// 每次遍历找f最小存入min
	}
	
	return 0;
}
/*    定义创建迷宫的函数    */
MAZE* InitMaze(int size)//创建迷宫
{
	int i;
	MAZE* maze = (MAZE*)malloc(sizeof(struct MAZE_STRU));
	maze->size = size;
	//给迷宫分配空间
	maze->data = (int**)malloc(sizeof(int*) * maze->size);
	for (i = 0; i < maze->size; i++)
	{
		maze->data[i] = (int*)malloc(sizeof(int) * maze->size);
	}
	return maze;
}

//void ReadMaze(MAZE* maze)
//{
//	int i, j;
//	printf("请用矩阵的形式输入迷宫的结构：\n");
//	for (i = 0; i < maze->size; i++)
//	{
//		for (j = 0; j < maze->size; j++)
//			scanf_s("%d", &maze->data[i][j]);
//	}
//}

void CreateRandomMaze(MAZE* maze)
{
	int i, j;
	for (i = 0; i < maze->size; i++) {
		for (j = 0; j < maze->size; j++) {
			maze->data[i][j] = 1; // 将外围全部设置为墙壁
		}
	}

	// 随机设置内部通道
	for (i = 1; i < maze->size - 1; i++) {
		for (j = 1; j < maze->size - 1; j++) {
			//maze->data[i][j] = rand() % 2; // 随机设置0或1来表示通道或墙壁
			int randomValue = rand() % 3;
			if (randomValue == 0) {
				maze->data[i][j] = 1; // 墙壁概率1/3
			}
			else {
				maze->data[i][j] = 0; // 通道
			}
		}
	}

	maze->data[1][1] = 2; // 设置起点为通道
	maze->data[maze->size - 2][maze->size - 2] = 2; // 设置终点为通道
}

void WriteMaze(MAZE* maze)
{
	int i, j;
	printf("\n迷宫的结构如下：\n");
	for (i = 0; i < maze->size; i++)
	{
		for (j = 0; j < maze->size; j++)
		{
			if (maze->data[i][j] == 1)
				printf("■");
			if (maze->data[i][j] == 0)
				printf("  ");
			if (maze->data[i][j] == 2)
				printf("\x1B[31m●\x1B[0m");
		}
		printf("\n");
	}
}

int main()
{
	srand((unsigned)time(NULL)); // 初始化随机数生成器
	MAZE* maze;
	int A;//用于存储函数调用返回值，借此来判断迷宫是否有路径通向终点
	int mazeSize;
	int entryX, entryY, exitX, exitY;
	
	printf("请输入迷宫大小：\n");
	scanf_s("%d", &mazeSize);
	maze = InitMaze(mazeSize);
	//ReadMaze(maze);
	CreateRandomMaze(maze);
	/*printf("请输入入口点与出口点的坐标：\n");
	scanf_s("%d%d%d%d", &entryX, &entryY, &exitX, &exitY);*/
	
	WriteMaze(maze);//打印迷宫
	A = maze_A(1, 1, mazeSize - 2, mazeSize - 2, maze);//调用A*
	WriteMaze(maze);//打印路径
	if (A == 0) printf("\n该迷宫没有路径通向终点！\n");
	return 0;
}
