//最短路径floyd算法
//P数组，记录了目标顶点到其他顶点的最短路径的前驱节点 没有前驱用-1
//D数组，记录了目标顶点到其他顶点的最短路径的长度, MAX表示无路径
//核心：试探法，通过加入不同的点进行中转，选择出最优解
//又称 3 for 算法， n*n*n

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

void floyd(Graph* G){
    int d[4][4];  //定义数组应该用常量值，不能 int d[G->vexNum][G->vexNum]
    int p[4][4];
    //初始化
    for (int i = 0; i < G->vexNum; i++){
        for (int j = 0; j < G->vexNum; j++){
                d[i][j] = G->arcs[i][j];
                if (G->arcs[i][j] > 0 && G->arcs[i][j] != MAX){
                    p[i][j] = i; //有弧则起点是自己
                }
                else{
                    p[i][j] = -1;
                }
        }
    }
    
    //三个for循环
    for (int i = 0; i < G->vexNum; i++){ //试探把每个顶点加进来
        for (int j = 0; j < G->vexNum; j++){ //这两个循环是矩阵里的每个点
            for (int k = 0; k < G->vexNum; k++){ //以i为中转
                if (d[j][i] + d[i][k] < d[j][k]){
                    d[j][k] = d[j][i] + d[i][k];
                    p[j][k] = p[i][k];
                }
            }
        }
    }

        // 输出d和p数组
        for (int i = 0; i < G->vexNum; i++)
        {
            for (int j = 0; j < G->vexNum; j++)
            {
                printf("%d ", d[i][j]);
            }
            printf("\n");
        }
    printf("\n");
    for (int i = 0; i < G->vexNum; i++){
        for (int j = 0; j < G->vexNum; j++){
            printf("%d ", p[i][j]);
        }
        printf("\n");
    }
}

int main() {
	Graph* G = initGraph(4);
	int* visited = (int*)malloc(sizeof(int) * G->vexNum);
	for(int i=0; i<G->vexNum; i++){
		 visited[i] = 0;  // 初始化为0，表示未访问
	}
    int arcs[4][4] = {
        // 初始化 arcs 数组时，应该用，来分隔每一行的元素
        0, 1, MAX, 3,
        1, 0, 2, 2,
        MAX, 2, 0, 8,
        3, 2, 8, 0
    };
	createGraph(G, "1234", (int*)arcs);

	printf("DFS遍历结果: \n");
	DFS(G, visited, 0); // 从节点0开始深度优先遍历
	printf("\n");

    floyd(G);

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
