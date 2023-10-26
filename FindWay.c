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
	int** data;  //�����ά����
};

typedef struct Maze* PMaze;
typedef struct Maze* LinkStack;
typedef struct MAZE_STRU MAZE;//����洢�Թ�������

LinkStack SetNullStack_link()
{
	LinkStack top = (LinkStack)malloc(sizeof(struct Maze));
	if (top != NULL)
		top->next = NULL;
	else
		printf("Alloc failure"); //�ڴ����ʧ��
	return top;

}
 
int IsNullStack_link(LinkStack top)  //�жϿ՗C
{
	if (top->next == NULL)
		return 1;
	else
		return 0;
}

void Push_link(LinkStack top, PMaze p)//��ջ�� PMAZE p��ʾ����ջ���½ڵ�
{
	p->next = top->next;
	top->next = p;
}

void POP(LinkStack top, PMaze precur)  
{//��ǰλ�ó�ջ������һ����¼��openlist�е�min��ջ��
	PMaze pre, p;
	pre = top;
	p = top;
	if (p->next != NULL)
	{
		while (p != NULL)
		{
			if (p == precur)  //Ҫ�Ƴ��Ľڵ�precur
			{
				pre->next = p->next;  //�����ýڵ�
				//return 0;
			}
			pre = p;
			p = p->next;
		}
	}
	else {
		printf("openlist�ѿ�!\n");
	}
}

LinkStack Top_link(LinkStack sstack)//ȡջ��Ԫ��
{
	if (IsNullStack_link(sstack))
	{
		printf("it is empty");
		return 0;
	}
	else
		return sstack->next;
}

PMaze InputCoordinate(int x, int y, int g, int h, int f)//������,�ɱ�����ָ��ǰλ�õĽڵ�
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
		if ((q->x == p->x) && (q->y == p->y)) return 1;  //���Ҹõ��Ƿ����ĳ����
		q = q->next;
	}
	return 0;
}

PMaze min_pmaze(LinkStack top)  //�ڶ�ջ�в�����Сfֵ��Ԫ��
{
	PMaze q, p = top->next;
	q = p;
	while (p != NULL)
	{
		if (q->f > p->f)
		{
			q = p;
		}
		if (q->f == p->f)   //���f��ͬ����ȡ��С��hֵ�������ڸ����ٵ�����
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
	//A*�㷨̽���Թ�
	//�����˰˸�����
	int direction[8][2] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} };
	int posX = entryX, posY = entryY;//��ʱ�������������꣨x,y��
	int preposX = entryX, preposY = entryY;//���ڴ洢ǰһ��λ�õ����꣨x,y��

	int mov;//�ƶ��ķ���
	int g, h, f;//��ʱ�洢g,h

	PMaze cur;//ָ��ǰλ��
	PMaze precur;//�洢ǰһ��λ��
	PMaze min;//���ڴ洢������С��f��λ��

	LinkStack openlist = SetNullStack_link();//���忪���б�
	LinkStack closelist = SetNullStack_link();//�������б�

	g = abs(posX - preposX) + abs(posY - preposY);
	h = abs(exitX - posX) + abs(exitY - posY);   //�����پ���
	f = g + h;
	cur = InputCoordinate(entryX, entryY, g, h, f);
	cur->father = NULL;
	Push_link(openlist, cur);
	min = cur;//�����ʼλ��Ϊ���λ��

	while (!IsNullStack_link(openlist))
	{
		precur = min;
		//printf("((%d %d)g:%d h:%d f:%d)\n", min->x, min->y,min->g,min->h,min->f);
		//��һ��ע�������ڵ���ʱ���鿴ÿһ����С�ɱ���λ�õľ�����Ϣ

		// ��ӡopen������
		////printf("Open��: ");
		//PMaze tempOpen = Top_link(openlist);
		//while (tempOpen != NULL) {
		//	//printf("(%d,%d)��", tempOpen->x, tempOpen->y );
		//	tempOpen = tempOpen->next;
		//}
		//printf("\n");

		//// ��ӡclose������
		////printf("Close��: ");
		//PMaze tempClose = closelist->next;
		//while (tempClose != NULL) {
		//	//printf("(%d ,%d)��", tempClose->x, tempClose->y);
		//	tempClose = tempClose->next;
		//}
		//printf("\n");
		//printf("\n");

		preposX = precur->x;//��¼��ǰλ�õ�X���꣨��һ��λ�õ�ǰһ��λ�ã�
		preposY = precur->y;//��¼��ǰλ�õ�Y���꣨��һ��λ�õ�ǰһ��λ�ã�
		POP(openlist, precur);//��ջ,��closelist
		Push_link(closelist, precur);
		mov = 0;//���ó�ʼ̽������
		while (mov < 8)
		{
			posX = preposX + direction[mov][0];
			posY = preposY + direction[mov][1];

			if (posX == exitX && posY == exitY)//openlist���г��ڣ����ʾ�����յ�
			{
				g = abs(posX - preposX) + abs(posY - preposY);
				h = abs(exitX - posX) + abs(exitY - posY);
				f = g + h;
				cur = InputCoordinate(exitX, exitY, g, h, f);
				cur->father = precur;
				Push_link(openlist, cur);//����openlist
				printf("\nA*�㷨�����������Ž�Ϊ:\n");
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
			if (!(MAZE->data[posX][posY]) && !Exist(closelist, cur)) //�ж��Ƿ���ǽ��,�Ƿ���closelist��
			{
				if (!Exist(openlist, cur))//�����򽫵�ǰλ�ô���openlist
				{
					cur->father = precur;
					Push_link(openlist, cur);//��ջ
				}
				else
				{//����ǰ�㵽min��λ�ö��ڵ�ԭ���׽ڵ��λ�õľ��룬�򽫵�ǰλ�õĸ��ڵ����Ϊmin
					if (cur->g < precur->g)
					{
						cur->father = min;
					}
				}//����ʲôҲ����
			}
			mov++;
		}
		min = min_pmaze(openlist);// ÿ�α�����f��С����min
	}
	
	return 0;
}
/*    ���崴���Թ��ĺ���    */
MAZE* InitMaze(int size)//�����Թ�
{
	int i;
	MAZE* maze = (MAZE*)malloc(sizeof(struct MAZE_STRU));
	maze->size = size;
	//���Թ�����ռ�
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
//	printf("���þ������ʽ�����Թ��Ľṹ��\n");
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
			maze->data[i][j] = 1; // ����Χȫ������Ϊǽ��
		}
	}

	// ��������ڲ�ͨ��
	for (i = 1; i < maze->size - 1; i++) {
		for (j = 1; j < maze->size - 1; j++) {
			//maze->data[i][j] = rand() % 2; // �������0��1����ʾͨ����ǽ��
			int randomValue = rand() % 3;
			if (randomValue == 0) {
				maze->data[i][j] = 1; // ǽ�ڸ���1/3
			}
			else {
				maze->data[i][j] = 0; // ͨ��
			}
		}
	}

	maze->data[1][1] = 2; // �������Ϊͨ��
	maze->data[maze->size - 2][maze->size - 2] = 2; // �����յ�Ϊͨ��
}

void WriteMaze(MAZE* maze)
{
	int i, j;
	printf("\n�Թ��Ľṹ���£�\n");
	for (i = 0; i < maze->size; i++)
	{
		for (j = 0; j < maze->size; j++)
		{
			if (maze->data[i][j] == 1)
				printf("��");
			if (maze->data[i][j] == 0)
				printf("  ");
			if (maze->data[i][j] == 2)
				printf("\x1B[31m��\x1B[0m");
		}
		printf("\n");
	}
}

int main()
{
	srand((unsigned)time(NULL)); // ��ʼ�������������
	MAZE* maze;
	int A;//���ڴ洢�������÷���ֵ��������ж��Թ��Ƿ���·��ͨ���յ�
	int mazeSize;
	int entryX, entryY, exitX, exitY;
	
	printf("�������Թ���С��\n");
	scanf_s("%d", &mazeSize);
	maze = InitMaze(mazeSize);
	//ReadMaze(maze);
	CreateRandomMaze(maze);
	/*printf("��������ڵ�����ڵ�����꣺\n");
	scanf_s("%d%d%d%d", &entryX, &entryY, &exitX, &exitY);*/
	
	WriteMaze(maze);//��ӡ�Թ�
	A = maze_A(1, 1, mazeSize - 2, mazeSize - 2, maze);//����A*
	WriteMaze(maze);//��ӡ·��
	if (A == 0) printf("\n���Թ�û��·��ͨ���յ㣡\n");
	return 0;
}
