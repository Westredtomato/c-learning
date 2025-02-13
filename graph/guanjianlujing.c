//AOE网，activity on edge，有向无环图，事件（顶点），活动（弧）
//求解关键路径，1：拓扑排序
//2：计算指标，事件指标vi，最早early（选最大的路径），最晚开始时间late（选最小的）、用vi（early）-weight）
//活动指标li，最早=Vstart(e)，最晚开始时间=Vend(late)-weight
//3：找出关键活动， l(l)-l(e)=0 时为关键活动,求时间余量

//此代码有误

#include <stdio.h>
#include <stdlib.h>

#define MAX 30000  //宏变量，最大值

typedef struct Node { //建立一个栈
    int data; //节点的数据
    struct Node* next;  //指向下一个节点的指针
} Node;

Node* initStack() { //初始化栈
    Node* stack = (Node*)malloc(sizeof(Node));
    stack->data = 0;
    stack->next = NULL;
    return stack;
}

int isEmpty(Node* stack) {
    if (stack->next == NULL) { //没有顶点只有头节点那就是空的
        return 1;
    }
    else {
        return 0;
    }
}

void push(Node* stack, int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->next = stack->next;
    stack->next = node;
    stack->data++;
}

int pop(Node* stack) {
    if (!isEmpty(stack)) {
        Node* node = stack->next;
        stack->next = node->next;
        return node->data;
    }
    else {
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
            if (G->arcs[i][j] > 0 && G->arcs[i][j] != MAX) {
                G->arcNum++;
            }
        }
        //不能用G->arcNum /= 2; 有向图，两条边都算
    }
}

int* findInDegrees(Graph* G) { // 找每个点的入度
    int* inDegrees = (int*)malloc(sizeof(int) * G->vexNum);
    for (int i = 0; i < G->vexNum; i++) {
        inDegrees[i] = 0;   // 初始化所有节点的入度为 0
    }
    for (int i = 0; i < G->vexNum; i++) {
        for (int j = 0; j < G->vexNum; j++) {
            if (G->arcs[i][j] > 0 && G->arcs[i][j] != MAX) // 如果有从 i 到 j 的路径
                inDegrees[j] = inDegrees[j] + 1; //有边，则该点入度加一
        }
    }
    return inDegrees;
}

int* topologicalSort(Graph* G) { //要返回返回值，故从void变成int*
    int index = 0; //索引用来记录拓扑进行到哪里了,用于存放拓扑排序的顺序
    int* top = (int*)malloc(sizeof(int) * G->vexNum); // 存储拓扑排序结果
    int* inDegrees = findInDegrees(G); // 获取每个节点的入度
    Node* stack = initStack();
    for (int i = 0; i < G->vexNum; i++) {
        if (inDegrees[i] == 0) { //把入度为零的点入栈
            push(stack, i);
        }
    }
    while (!isEmpty(stack)) {
        int vex = pop(stack); // 弹出栈顶元素
        top[index++] = vex;  // 记录拓扑排序的结果
        for (int i = 0; i < G->vexNum; i++) {
            if (G->arcs[vex][i] > 0 && G->arcs[vex][i] != MAX) { // 如果从顶点 vex 到 i 有路径
                inDegrees[i] = inDegrees[i] - 1; // i 的入度减 1
                if (inDegrees[i] == 0) { // 如果 i 的入度为 0，入栈
                    push(stack, i);
                }
            }
        }
    }
    // 输出拓扑排序结果
    for (int i = 0; i < index; i++) {
        printf("%c ", G->vexs[top[i]]);
    }
    // 如果拓扑排序结果的节点数少于图中的顶点数，说明图中有环
    if (index < G->vexNum) {
        printf("没有包含所有顶点，图中有环\n");
    }
    printf("\n");
    return top;
}

//深度优先遍历
void DFS(Graph* G, int* visited, int index) {
    printf("%c ", G->vexs[index]);
    visited[index] = 1;
    for (int i = 0; i < G->vexNum; i++) {
        if (G->arcs[index][i] > 0 && G->arcs[index][i] != MAX && !visited[i]) {
            DFS(G, visited, i);  // 递归遍历
        }
    }
}

int getIndex(int* top, Graph* G, int i) {
    int j;
    for (j = 0; j < G->vexNum; j++) {
        if (top[j] == i) {
            break;
        }
    }
    return j;
}

void criticalPath(Graph* G) {
    int* top = topologicalSort(G);
    int* early = (int*)malloc(sizeof(int) * G->vexNum);
    int* late = (int*)malloc(sizeof(int) * G->vexNum);
    for (int i = 0; i < G->vexNum; i++) {  //对这两个数组进行赋值
        early[i] = 0;   // 初始化最早发生时间为0
        late[i] = 0;    // 初始化最晚发生时间为一个很大的值
    }

    // 计算最早发生时间，对拓扑序列的每一个点进行循环
    for (int i = 0; i < G->vexNum; i++) {  //要拿拓扑序列里面的index
        int max = 0;  //从前到后，看前面的边到目标顶点的最大值是多少
        for (int j = 0; j < G->vexNum; j++) {
            if (G->arcs[j][top[i]] > 0 && G->arcs[j][top[i]] != MAX) {
                int index = getIndex(top, G, j);
                if (early[index] + G->arcs[j][top[i]] > max)
                    max = early[index] + G->arcs[j][top[i]];
            }
        }
        early[i] = max;
    }
    printf("最早发生时间：\n");
    for (int i = 0; i < G->vexNum; i++) {
        printf("%d ", early[i]);
    }
    printf("\n");
    late[(G->vexNum) - 1] = early[(G->vexNum) - 1];

    // 计算最晚发生时间
    for (int i = (G->vexNum) - 2; i >= 0; i--) {
        int min = MAX;
        for (int j = 0; j < G->vexNum; j++) {
            if (G->arcs[top[i]][j] > 0 && G->arcs[top[i]][j] != MAX) {  //代表有边
                int index = getIndex(top, G, j);
                if (late[index] - G->arcs[top[i]][j] < min)
                    min = late[index] - G->arcs[top[i]][j];
            }
        }
        late[i] = min;
    }
    printf("最晚发生时间：\n");
    for (int i = 0; i < G->vexNum; i++) {
        printf("%d ", late[i]);
    }
    printf("\n");

    for (int i = 0; i < G->vexNum; i++) {
        for (int j = 0; j < G->vexNum; j++) {
            if (G->arcs[i][j] > 0 && G->arcs[i][j] != MAX) {
                int start = getIndex(top, G, i);
                int end = getIndex(top, G, j);
                if ((late[end] - G->arcs[i][j]) - early[start] == 0) {
                    printf("start = %d end = %d\n", i, j);
                }
            }
        }
    }
}

int main() {
    Graph* G = initGraph(9);
    int* visited = (int*)malloc(sizeof(int) * G->vexNum);
    for (int i = 0; i < G->vexNum; i++) {
        visited[i] = 0;  // 初始化为0，表示未访问
    }
    int arcs[9][9] = {
        //初始化 arcs 数组时，应该用，来分隔每一行的元素
        0,6,4,5,MAX,MAX,MAX,MAX,MAX,
        MAX,0,MAX,MAX,1,MAX,MAX,MAX,MAX,
        MAX,MAX,0,MAX,1,MAX,MAX,MAX,MAX,
        MAX,MAX,MAX,0,MAX,2,MAX,MAX,MAX,
        MAX,MAX,MAX,MAX,0,MAX,9,7,MAX,
        MAX,MAX,MAX,MAX,MAX,0,MAX,4,MAX,
        MAX,MAX,MAX,MAX,MAX,MAX,0,MAX,2,
        MAX,MAX,MAX,MAX,MAX,MAX,MAX,0,4,
        MAX,MAX,MAX,MAX,MAX,MAX,MAX,MAX,0
    };
    createGraph(G, "012345678", (int*)arcs);

    printf("DFS遍历结果: \n");
    DFS(G, visited, 0); // 从节点0开始深度优先遍历
    printf("\n");

    criticalPath(G);

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