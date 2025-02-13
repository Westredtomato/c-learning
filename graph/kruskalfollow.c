//克鲁斯卡尔，找最小生成树 MST ，找边法，对所有边排序，选权值最小的边，但不能形成一个回路（连通图),应包含所有顶点
//维护一个边的数组并排序，判断图是否连通（需要一个辅助数组记录当前索引的结点属于哪个连通分量）
//微错，运行结果有误

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
    int start;
    int end;
    int weight;
} Edge;

Edge* initEdge(Graph* G){
    int index = 0;
    Edge *edge = (Edge *)malloc(sizeof(Edge) * G->arcNum);
    for (int i = 0; i < G->vexNum; i++){
        for (int j = i + 1; j < G->vexNum; j++){
            if(G->arcs[i][j] != MAX){
                edge[index].start = i;
                edge[index].end = j;
                edge[index].weight = G->arcs[i][j];
                index++;
            }
        }
    }
    return edge;
}

void sortEdge(Edge* edge,Graph* G){
    Edge temp;
    for (int i = 0; i < G->arcNum - 1; i++){
        for (int j = 0; j < G->arcNum - i - 1; j++){
            if(edge[j].weight > edge[j+1].weight){
                    temp = edge[j];
                    edge[j] = edge[j + 1];
                    edge[j + 1] = temp;
            }
        }
    }
}

void kruskal(Graph* G){
    int *connected = (int *)malloc(sizeof(int) * G->vexNum);
    for (int i = 0; i < G->vexNum; i++){
        connected[i] = i;
    }
    Edge *edge = initEdge(G);
    sortEdge(edge, G);
    for (int i = 0; i < G->arcNum; i++){
        int start = connected[edge[i].start];
        int end = connected[edge[i].end];
        if(start != end){
            printf("v%c --> v%c weight = %d\n", G->vexs[edge[i].start], G->vexs[edge[i].end], edge[i].weight);
            for (int j = 0; j < G->vexNum; j++){
                if(connected[j] == end){
                    connected[j] = start;
                }
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

    kruskal(G);

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
