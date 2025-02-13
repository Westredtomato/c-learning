//校园导航系统
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30000 // 用一个很大的数表示不可达的边

// 图的结构体
typedef struct Graph {
    char* vexs; // 顶点数组 (景点名称代号)
    int** arcs; // 邻接矩阵 (路径长度)
    int vexNum; // 顶点数量
    int arcNum; // 边数量
} Graph;

// 景点信息结构体
typedef struct Information {
    char* name;  // 景点名称
    char* description; // 景点简介
} Information;

// 初始化图
Graph* initGraph(int vexNum) {
    Graph* G = (Graph*)malloc(sizeof(Graph));
    G->vexs = (char*)malloc(sizeof(char) * vexNum);
    G->arcs = (int**)malloc(sizeof(int*) * vexNum);
    for (int i = 0; i < vexNum; i++) {
        G->arcs[i] = (int*)malloc(sizeof(int) * vexNum);
    }
    G->vexNum = vexNum;
    G->arcNum = 0; // 初始化边数量
    return G;
}

// 创建图的邻接矩阵
void createGraph(Graph* G, char* vexs, int* arcs) {
    for (int i = 0; i < G->vexNum; i++) {
        G->vexs[i] = vexs[i];  // 设置顶点名称
        for (int j = 0; j < G->vexNum; j++) {
            G->arcs[i][j] = *(arcs + i * G->vexNum + j);
            if (G->arcs[i][j] != 0 && G->arcs[i][j] != MAX) {  
                G->arcNum++; // 如果存在边，则增加边的数量
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
        if (!s[i] && d[i] < min) { // 找到未确定最短路径且距离最小的节点
            min = d[i];
            index = i;
        }
    }
    return index;
}

// Dijkstra算法计算从index节点到其它节点的最短路径
void dijkstra(Graph *G, int index, int *d, int *p) {
    // index即起点
	// 准备辅助数组
	int *s = (int *)malloc(sizeof(int) * G->vexNum); // 标记节点是否已确定最短路径

    // 初始化距离、前驱和标记数组
    for (int i = 0; i < G->vexNum; i++) {
        if (G->arcs[index][i] != MAX && G->arcs[index][i] != 0) {  // 代表有路径，即有权值
            p[i] = index;  // 设置前驱节点
            d[i] = G->arcs[index][i];  // 设置距离
        } else {
            p[i] = -1;  // 无前驱节点
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
            if (!s[v] && d[u] + G->arcs[u][v] < d[v] && d[u] != MAX) {  // 更新最短路径
                d[v] = d[u] + G->arcs[u][v];
                p[v] = u;
            }
        }
    }

    free(s);  // 释放标记数组的内存
}

// 输出从起点到终点的最短路径
void printPath(int* p, int index, int destination, Graph* G, Information* info) { //int* p: 前驱节点数组

    //如果当前节点 destination 就是起点 index，则直接输出起点的信息并返回。递归终止。
    if (destination == index) {
        printf("%d %s", index + 1, info[index].name); // 输出起点
        return;
    } 

    //如果当前节点 destination 不是起点，则递归调用 printPath 函数，传入当前节点的前驱节点 p[destination]。一直回溯到起点。
    printPath(p, index, p[destination], G, info); // 递归输出路径

    //在递归调用返回后，输出当前节点的信息。确保路径是从起点到终点的顺序输出。
    printf(" -> %d %s", destination + 1, info[destination].name); // 输出当前节点
}

// 查询景点的信息
void askInformation(Information* info, int vexNum) {
    int choice;
    printf("请输入你想查询的景点编号 (1 - %d): ", vexNum);
    scanf("%d", &choice);
    if (choice >= 1 && choice <= vexNum) {
        printf("景点: %s\n", info[choice-1].name);
        printf("简介: %s\n", info[choice-1].description);
    } else {
        printf("无效的景点编号\n");
    }
}

// 查询任意两个景点的最短路径
void askShortestPath(Graph* G, int* d, int* p, Information* info) {
    int start, end;
    printf("景点编号和名称:\n");
    for (int i = 0; i < G->vexNum; i++) {
        printf("%d %s\n", i + 1, info[i].name); // 打印所有景点的编号和名称
    }

    printf("请输入起始景点编号 (1 - %d): ", G->vexNum);
    scanf("%d", &start);
    printf("请输入目标景点编号 (1 - %d): ", G->vexNum);
    scanf("%d", &end);

    if (start >= 1 && start <= G->vexNum && end >= 1 && end <= G->vexNum) {
        dijkstra(G, start - 1, d, p); // 从指定的起点开始计算最短路径
        printf("最短路径从景点 %d %s 到景点 %d %s: ", start, info[start-1].name, end, info[end-1].name);
        printPath(p, start-1, end-1, G, info); // 输出最短路径
        printf("\n路径长度: %d\n", d[end-1]); // 输出路径长度
    } else {
        printf("无效的景点编号\n");
    }
}

int main() {
    // 创建图，6个节点
    Graph* G = initGraph(6);
    Information info[6] = {
        {"南门", "欢迎来到暨南大学，这里是南门，圆拱之下是梦（噩梦）开始的地方。"},
        {"体育馆", "全称为霍英东体育馆，上有题字“把中华文化传播到五洲四海”，你可以在这里享受运动的乐趣。"},
        {"恒大楼", "即教学楼，同学们在这里上课，在知识的海洋里遨游洗礼，注意不要溺水，不要钓鱼。"},
        {"裕华堂", "三层大食堂，有各色美食，满足你的口腹之欲。如有不适，与食堂无关😉。"},
        {"宿舍T3栋", "您休息的地方, 地理位置优越, 往前走是图书馆, 往后走是兴安超市, 住在T3你就偷着乐吧!(排电梯的时候可以哭)"},
        {"图书馆", "又称冷冻库，馆内气温低，注意穿好外套保暖，否则会被冷死。十点关门，学习也要注意时间哦~"}
    };

    // 定义6个节点的邻接矩阵
    int arcs[6][6] = {
        {0, 15, MAX, MAX, 28, 21},
        {15, 0, 23, MAX, 30, MAX},
        {MAX, 23, 0, 7, MAX, 9},
        {MAX, MAX, 7, 0, 18, MAX},
        {28, 30, MAX, 18, 0, 10},
        {21, MAX, 9, MAX, 10, 0}
    };

    createGraph(G, "123456", (int*)arcs);  // 创建图的邻接矩阵

    // 存储最短路径和前驱节点
    int d[6], p[6]; // 表达式必须要有常量值，不能int d[G->vexNum], p[G->vexNum]

    int choice;
    while (1) {
        printf("\n1. 查询景点信息\n2. 查询最短路径\n3. 退出\n请选择服务: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                askInformation(info, 6);  // 查询景点信息
                break;
            case 2:
                askShortestPath(G, d, p, info);  // 查询最短路径
                break;
            case 3:
                printf("感谢使用校园导游程序！再见。\n");
                goto END; // 结束程序
            default:
                printf("无效的选择，请重新输入。\n");
        }
    }

END:
    // 释放资源
    for (int i = 0; i < G->vexNum; i++) {
        free(G->arcs[i]);
    }
    free(G->arcs);
    free(G->vexs);
    free(G);
    return 0;
}
