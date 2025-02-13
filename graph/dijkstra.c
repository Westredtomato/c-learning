//最短路径dijkstra算法
//S数组，记录了目标顶点到其他顶点的最短路径是否求得
//P数组，记录了目标顶点到其他顶点的最短路径的前驱节点
//D数组，记录了目标顶点到其他顶点的最短路径的长度

#include<stdio.h>
#include<stdlib.h>

#define MAX 30000

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
			if (G->arcs[i][j] != 0 && G->arcs[i][j] != MAX) { //带权值故应该arcs！=max，即有路径
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
		if (G->arcs[index][i] > 0 && G->arcs[index][i] != MAX && !visited[i]) {
			DFS(G, visited, i);  // 递归遍历
		}
	}
}

int getMin(int *d, int *s, Graph *G)
{
	int min = MAX;
	int index;
	for (int i = 0; i < G->vexNum; i++){
		if(!s[i] && d[i]<min ){
			min = d[i];
			index = i;
		}
	}
	return index;
}

void dijkstra(Graph *G, int index){ 
	// index即起点
	// 准备辅助数组
	int *s = (int *)malloc(sizeof(int) * G->vexNum); // 是否已确定最短路径
    int *p = (int *)malloc(sizeof(int) * G->vexNum); // 前驱节点
    int *d = (int *)malloc(sizeof(int) * G->vexNum); // 最短路径长度
    // 初始化辅助数组
    for (int i = 0; i < G->vexNum; i++){
        if (G->arcs[index][i] > 0 && G->arcs[index][i] != MAX) { // 代表有路径，即有权值
            p[i] = index;
			d[i] = G->arcs[index][i];
		}
        else{
			p[i] = -1;
			d[i] = MAX; //没弧
		}
		if(i==index){
			s[i] = 1;
			d[i] = 0;
		}
		else
			s[i] = 0;
	}
	for (int i = 0; i < G->vexNum - 1; i++){
		int index = getMin(d, s, G);
		s[index] = 1;
		for (int j = 0; j < G->vexNum; j++){
			if (!s[j] && d[index] + G->arcs[index][j] < d[j]){
				d[j] = d[index] + G->arcs[index][j];
				p[j] = index;
			}
		}
	}
	for (int i = 0; i < G->vexNum; i++){
		printf("%d %d %d\n", s[i], p[i], d[i]);
	}
}

int main() {
	Graph* G = initGraph(7);
	int* visited = (int*)malloc(sizeof(int) * G->vexNum);
	for(int i=0; i<G->vexNum; i++){
		 visited[i] = 0;  // 初始化为0，表示未访问
	}
	int arcs[7][7] = {
		//初始化 arcs 数组时，应该用，来分隔每一行的元素
		0,12,MAX,MAX,MAX,16,14,
        12,0,10,MAX,MAX,7,MAX,
        MAX,10,0,3,5,6,MAX,
        MAX,MAX,3,0,4,MAX,MAX,
        MAX,MAX,5,4,0,2,8,
        16,7,6,MAX,2,0,9,
        14,MAX,MAX,MAX,8,9,0
	};
	createGraph(G, "1234567", (int*)arcs);

	printf("DFS遍历结果: \n");
	DFS(G, visited, 0); // 从节点0开始深度优先遍历
	printf("\n");

    dijkstra(G, 0);

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
