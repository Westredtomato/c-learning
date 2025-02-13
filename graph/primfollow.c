//普里姆算法，找最小生成树 MST ，找点法，从集合里找边最小的加上这个点
//记录当前u集合状态，选择最小边的顶和点加入u集合

#include<stdio.h>
#include<stdlib.h>

//图顶点之间不通，那么邻接矩阵值为MAX
//如果顶点是自己本身，那么值为零

#define MAX 30000 // 无穷大，表示不相连

typedef struct Graph {
	char* vexs; //顶点数组 
	int** arcs; //边数组 
	//二级指针
	int vexNum; //顶点数量 
	int arcNum; //边数量 
}Graph;

typedef struct Edge {
    char vex;   //记录u集合到最小边的起始结点
    int weight; //记录最小边
}Edge;

//当 edge.weight = 0 时，代表顶点加入到u集合中
Edge* initEdge(Graph* G,int index){
    Edge *edge = (Edge *)malloc(sizeof(Edge) * G->vexNum);
    for (int i = 0; i < G->vexNum;i++){
        edge[i].vex = G->vexs[index];
        edge[i].weight = G->arcs[index][i];
    }
    return edge;
}

int getMinEdge(Edge* edge,Graph* G){
    int index;
    int min = MAX;
    for (int i = 0; i < G->vexNum; i++){
        if(edge[i].weight != 0 && min > edge[i].weight){
            min = edge[i].weight;
            index = i;
        }
    }
    return index;
}

void prim(Graph* G,int index){
    int min;
    Edge *edge = initEdge(G, index);
    for (int i = 0; i < G->vexNum - 1; i++){ //找最小的点，找n-1次
        min = getMinEdge(edge, G);
        printf("v%c --> v%c, weight=%d\n", edge[min].vex, G->vexs[min], edge[min].weight);
        edge[min].weight = 0;
        for (int j = 0; j < G->vexNum;j++){ // 更新最小边
            if(G->arcs[min][j] < edge[j].weight){
                edge[j].weight = G->arcs[min][j];
                edge[j].vex = G->vexs[min];
            }
        } 
    }
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
			if (G->arcs[i][j] != 0 && G-> arcs[i][j] != MAX) {
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
		if (G->arcs[index][i] > 0 && G-> arcs[index][i] != MAX && !visited[i]) {
			//判断是否存在一条边，并且该节点还没有被访问
            //G->arcs[index][i] == 0 表示没有边,G->arcs[index][i] == MAX 表示两点不相连
            DFS(G, visited, i);  // 递归遍历
		}
	}
}

int main() {
	Graph* G = initGraph(6);
	int* visited = (int*)malloc(sizeof(int) * G->vexNum);
	for(int i=0; i<G->vexNum; i++){
		 visited[i] = 0;  // 初始化为0，表示未访问
	}
    int arcs[6][6] = {
        // 初始化 arcs 数组时，应该用，来分隔每一行的元素
        0, 6, 1, 5, MAX, MAX,
        6, 0, 5, MAX, 3, MAX,
        1, 5, 0, 5, 6, 4,
        5, MAX, 5, 0, MAX, 2,
        MAX, 3, 6, MAX, 0, 6,
        MAX, MAX, 4, 2, 6, 0
    };
	createGraph(G, "123456", (int*)arcs);

	printf("DFS遍历结果: \n");
	DFS(G, visited, 0); // 从节点0开始深度优先遍历
	printf("\n");

    prim(G, 0);

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
