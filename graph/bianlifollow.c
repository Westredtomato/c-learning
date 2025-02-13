//图的创建（邻接矩阵）和遍历（DFS）（BFS）

#include<stdio.h>
#include<stdlib.h>

#define MAXSIZE 5

typedef struct Graph {
	char* vexs; //顶点数组 
	int** arcs; //边数组 
	//二级指针
	int vexNum; //顶点数量 
	int arcNum; //边数量 
}Graph;

typedef struct Queue{
	int front;
	int rear;
	int data[MAXSIZE];
}Queue;

Queue* initQueue(){
	Queue* Q = (Queue*)malloc(sizeof(Queue));
	Q->front = Q->rear = 0;
	return Q;
}

int isFull(Queue* Q){
	//return (Q->rear + 1) % MAXSIZE == Q->front; 返回是否满
    if((Q->rear + 1) % MAXSIZE == Q->front){
    return 1;
    }
    else{
    return 0;
    }
}

int isEmpty(Queue* Q){
	//return Q->front == Q->rear; 返回是否为空
    if(Q->front==Q->rear){
        return 1;
    }
    else {
        return 0;
    }
}

int enQueue(Queue* Q,int data){
	if(isFull(Q)){
		return 0;//队列满
	}
	else {
		Q->data[Q->rear] = data;
		Q->rear = (Q->rear + 1) % MAXSIZE;//循环队列
		return 1;//入队成功
	}
}

int deQueue(Queue* Q){
	if(isEmpty(Q)){
		return -1; // 队列为空，不能出队
	}
	else{
		int data = Q->data[Q->front];
		Q->front = (Q->front + 1) % MAXSIZE; // 循环队列
		return data;
	}
}

void freeQueue(Queue* Q) {
    free(Q);  // 释放队列的内存
}

Graph* initGraph(int vexNum) {
	Graph* G = (Graph*)malloc(sizeof(Graph));
	G->vexs = (char*)malloc(sizeof(char) * vexNum);
	G->arcs = (int**)malloc(sizeof(int*) * vexNum);
	for (int i = 0; i < vexNum; i++) {
		G->arcs[i] = (int*)malloc(sizeof(int) * vexNum);
	}
	G->vexNum = vexNum;
	G->arcNum = 0;
	return G;
}

void createGraph(Graph* G, char* vexs, int* arcs) {
	for (int i = 0; i < G->vexNum; i++) {
		G->vexs[i] = vexs[i];
		for (int j = 0; j < G->vexNum; j++) {
			G->arcs[i][j] = *(arcs + i * G->vexNum + j);
			//*取地址，首地址加偏移量，把二维数组一维化，i*G->vexNum相当于跳过了前i行的数，j是第i+1行的偏移量
			if (G->arcs[i][j] != 0) {
				G->arcNum++;
			}
		}
		G->arcNum /= 2; //无向图，只能算一条边
	}
}

//深度优先遍历
void DFS(Graph* G, int* visited, int index) {
	printf("%c\t", G->vexs[index]);
	visited[index] = 1;
	for (int i = 0; i < G->vexNum; i++) {
		if (G->arcs[index][i] == 1 && !visited[i]) {
			DFS(G, visited, i);  // 递归遍历
		}
	}
}

//广度优先遍历，用到队列
void BFS(Graph* G,int* visited, int index){
	Queue *Q = initQueue();
	printf("%c\t", G->vexs[index]);
	visited[index] = 1;
	enQueue(Q, index);
	while(!isEmpty(Q)){
		int i = deQueue(Q);  // 出队一个节点
		for (int j = 0; j < G->vexNum;j++){
			if(G->arcs[i][j] == 1 && !visited[j]){  // 访问邻接节点
				printf("%c\t", G->vexs[j]);
				visited[j] = 1;
				enQueue(Q, j); //入队
			}
		}
	}
	freeQueue(Q);  // 释放队列内存
}

int main() {
	Graph* G = initGraph(5);
	int* visited = (int*)malloc(sizeof(int) * G->vexNum);
	for(int i=0; i<G->vexNum; i++){
		 visited[i] = 0;  // 初始化为0，表示未访问
	}
	int arcs[5][5] = {
		//初始化 arcs 数组时，应该用，来分隔每一行的元素
		0, 1, 1, 1, 0,
		1, 0, 1, 1, 1,
		1, 1, 0, 0, 0,
		1, 1, 0, 0, 1,
		0, 1, 0, 1, 0
	};
	createGraph(G, "ABCDE", (int*)arcs);

	printf("DFS遍历结果: \n");
	DFS(G, visited, 0); // 从节点0开始深度优先遍历
	printf("\n");

	// 重置visited数组，准备进行BFS
    for (int i = 0; i < G->vexNum; i++) {
        visited[i] = 0;  // 重新初始化为0
    }

    printf("BFS遍历结果: \n");
	BFS(G, visited, 0); // 从节点0开始广度优先遍历
	printf("\n");

	// 释放内存
    free(visited);
    for (int i = 0; i < G->vexNum; i++) {
        free(G->arcs[i]);
    }
    free(G->arcs);
    free(G->vexs);
    free(G);

	return 0;
}
