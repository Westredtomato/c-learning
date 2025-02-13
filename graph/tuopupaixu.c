//topologicalSort
//拓扑排序，概念：将AOV中的顶点排成一个线性序列，如果vi->vj有弧，那么vi必然在vj前面
//拓扑排序一般用来看图里有没有环，是否包含所有节点
//有向无环图DAG
//AOV：图的节点表示任务，图的弧表示先后依赖关系
//拓扑排序流程：找出没有前驱的点，输出它，减掉以它为出发点的所有边。所有点的入度随之调整。
//重复第一步，直到图中不存在没有前驱的点
//基于无向图

#include<stdio.h>
#include<stdlib.h>

typedef struct Node{ //建立一个栈
    int data; //节点的数据
    struct Node *next;  //指向下一个节点的指针
} Node;

Node* initStack(){ //初始化栈
    Node *stack = (Node *)malloc(sizeof(Node));
    stack->data = 0;
    stack->next = NULL;
    return stack;
}

int isEmpty(Node* stack){
    if(stack->next==NULL){ //没有顶点只有头节点那就是空的
        return 1;
    }
    else{
        return 0;
    }
}

void push(Node* stack,int data){
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = stack->next;
    stack->next = node;
    stack->data++;
}

int pop(Node* stack){
    if(!isEmpty(stack)){
        Node *node = stack->next;
        stack->next = node->next;
        return node->data;
    }
    else{
        return -1;
    }
}

typedef struct Graph {
	char* vexs; //顶点数组 
	int** arcs; //边数组 
	//二级指针
	int vexNum; //顶点数量 
	int arcNum; //边数量 
}Graph;

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

int* findInDegrees(Graph* G){ // 找每个点的入度
    int *inDegrees = (int *)malloc(sizeof(int) * G->vexNum);
    for (int i = 0; i < G->vexNum; i++){
        inDegrees[i] = 0;   // 初始化所有节点的入度为 0
    }
    for (int i = 0; i < G->vexNum; i++){
        for (int j = 0; j < G->vexNum; j++){
            if(G->arcs[i][j])  // 如果有从 i 到 j 的边
                inDegrees[j] = inDegrees[j] + 1; //有边，则该点入度加一
        }
    }
    return inDegrees;
}

void topologicalSort(Graph* G){
    int index = 0; //索引用来记录拓扑进行到哪里了,用于存放拓扑排序的顺序
    int *top = (int *)malloc(sizeof(int) * G->vexNum); // 存储拓扑排序结果
    int *inDegrees = findInDegrees(G); // 获取每个节点的入度
    Node *stack = initStack();
    for (int i = 0; i < G->vexNum; i++){
        if (inDegrees[i] == 0){ //把入度为零的点入栈
            push(stack, i);
        }
    }
    while(!isEmpty(stack)){
        int vex = pop(stack); // 弹出栈顶元素
        top[index++] = vex;  // 记录拓扑排序的结果
        for (int i = 0; i < G->vexNum; i++){
            if(G->arcs[vex][i]){  // 如果从顶点 vex 到 i 有边
                inDegrees[i] = inDegrees[i] - 1; // i 的入度减 1
                if (inDegrees[i] == 0){ // 如果 i 的入度为 0，入栈
                    push(stack, i);
                }
            }
        }
    }
    // 输出拓扑排序结果
    for (int i = 0; i < index; i++){
        printf("%c ", G->vexs[top[i]]);
    }
    // 如果拓扑排序结果的节点数少于图中的顶点数，说明图中有环
    if(index<G->vexNum){
        printf("没有包含所有顶点，图中有环\n");
    }
    printf("\n");
}

int main() {
	Graph* G = initGraph(6);
	int* visited = (int*)malloc(sizeof(int) * G->vexNum);
	for(int i=0; i<G->vexNum; i++){
		 visited[i] = 0;  // 初始化为0，表示未访问
	}
	int arcs[6][6] = {
		//初始化 arcs 数组时，应该用，来分隔每一行的元素
		0,1,1,1,0,0,
        0,0,0,0,0,0,
        0,1,0,0,1,0,
        0,0,0,0,1,0,
        0,0,0,0,0,0,
        0,0,0,1,1,0
	};
	createGraph(G, "123456", (int*)arcs);

	printf("DFS遍历结果: \n");
	DFS(G, visited, 0); // 从节点0开始深度优先遍历
	printf("\n");

    topologicalSort(G);

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