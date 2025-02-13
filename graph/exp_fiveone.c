#include<stdio.h>
#include<stdlib.h>

#define MAX 30000 // 用一个很大的数表示不可达的边

// 图的结构体
typedef struct Graph {
    char* vexs; // 顶点数组 
    int** arcs; // 邻接矩阵
    int vexNum; // 顶点数量
    int arcNum; // 边数量
} Graph;

// 初始化图
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

// 创建图的邻接矩阵
void createGraph(Graph* G, char* vexs, int* arcs) {
    for (int i = 0; i < G->vexNum; i++) {
        G->vexs[i] = vexs[i];
        for (int j = 0; j < G->vexNum; j++) {
            G->arcs[i][j] = *(arcs + i * G->vexNum + j);
            //*取地址，首地址加偏移量，把二维数组一维化，i*G->vexNum相当于跳过了前i行的数，j是第i+1行的偏移量
            if (G->arcs[i][j] != 0 && G->arcs[i][j] != MAX) {  //带权值故应该arcs！=max，即有路径
                G->arcNum++;
            }
        }
    }
    G->arcNum /= 2; // 由于是无向图，只算一条边
}

// 获取当前未确定最短路径的最小值的节点
int getMin(int *d, int *s, Graph *G) {
    int min = MAX;
    int index = -1;
    for (int i = 0; i < G->vexNum; i++) {
        if (!s[i] && d[i] < min) {
            min = d[i];
            index = i;
        }
    }
    return index;
}

// Dijkstra算法计算从index节点到其它节点的最短路径
void dijkstra(Graph *G, int index) {
    // index即起点
	// 准备辅助数组
    int *s = (int *)malloc(sizeof(int) * G->vexNum); // 标记节点是否已确定最短路径
    int *p = (int *)malloc(sizeof(int) * G->vexNum); // 前驱节点
    int *d = (int *)malloc(sizeof(int) * G->vexNum); // 最短路径的长度

    // 初始化距离、前驱和标记数组
    for (int i = 0; i < G->vexNum; i++) {
        if (G->arcs[index][i] != MAX && G->arcs[index][i] != 0) { // 有路径
            p[i] = index;
            d[i] = G->arcs[index][i];
        } else {
            p[i] = -1;
            d[i] = MAX; // 没有路径
        }
        if (i == index) {
            s[i] = 1; // 起点标记为已访问
            d[i] = 0; // 起点到自己的距离为0
        } else {
            s[i] = 0; // 其他节点标记为未访问
        }
    }
    
    // Dijkstra算法的核心部分
    for (int i = 0; i < G->vexNum - 1; i++) {
        int u = getMin(d, s, G); // 获取当前最小的距离节点
        s[u] = 1; // 标记该节点已确定最短路径
        for (int v = 0; v < G->vexNum; v++) {
            if (!s[v] && d[u] + G->arcs[u][v] < d[v] && d[u] != MAX) {
                d[v] = d[u] + G->arcs[u][v];
                p[v] = u;
            }
        }
    }

    // 输出从起点到每个节点的最短路径和路径长度
    printf("从v0到各个顶点的最短路径及路径长度：\n");
    for (int i = 0; i < G->vexNum; i++) {
        if (d[i] == MAX) {
            printf("v0 -> %c, 无路径\n", G->vexs[i]);
        } else {
            printf("v0 -> %c, 最短路径长度: %d\n", G->vexs[i], d[i]);
        }
    }

    free(s);
    free(p);
    free(d);
}

int main() {
    // 创建图，6个节点
    Graph* G = initGraph(6);

    // 定义6个节点的邻接矩阵
    int arcs[6][6] = {
        {0, 10, MAX, MAX, 3, 2},
        {8, 0, 5, MAX, MAX, MAX},
        {MAX, MAX, 0, 15, MAX, 2},
        {MAX, MAX, 9, 0, 5, MAX},
        {2, MAX, MAX, 5, 0, MAX},
        {MAX, MAX, MAX, MAX, 7, 0}
    };

    createGraph(G, "123456", (int*)arcs);

    // 计算从v0到其他节点的最短路径
    dijkstra(G, 0); // 从节点0（即v0）开始

    // 释放内存
    for (int i = 0; i < G->vexNum; i++) {
        free(G->arcs[i]);
    }
    free(G->arcs);
    free(G->vexs);
    free(G);

    return 0;
}
