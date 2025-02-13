

//图的数组（邻接矩阵）存储表示
#define INFINITY INT_MAX //最大值无穷
#define MAX_VERTEX_NUM 20 //最大顶点个数
typedef enum {DG, DN, UDG, UDN} GraphKind; //{有向图，有向网，无向图，无向网}
typedef struct ArcCell {
	VRType adj; //VRType是顶点关系类型。对无权图，用1或0
	           //表示相邻否；对带权图，则为权值类型
	InfoType * info; //该弧相关信息的指针
}ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
typedef struct {
	VertexType vexs[MAX_VERTEX_NUM]; //顶点向量
	AdjMatrix arcs; //邻接矩阵
	int vexnum, arcnum; //图的当前顶点数和弧数
	GraphKind kind; //图的种类标志
}MGraph;

Status CreateGraph(MGraph &G) {
	//采用数组（邻接矩阵）表示法，构造图G
	scanf(&G, kind);
	switch (G, kind) {
		case DG:return CreateDG(G);
		case DN:return CreateDN(G);
		case UDG:return CreateUDG(G);
		case UDN:return CreateUDN(G);
		default:return ERROR;
	}
}//CreateGraph

Status CreateUDN(MGraph &G) {
	//采用数组（邻接矩阵）表示法，构造无向网G
	scanf(&G.vexnum, &G.arcnum, &IncInfo); //IncInfo为0则各弧不含任何消息
	for (i = 0; i < G.vexnum; ++i)
		scanf(&G.vexs[i]); //构造顶点向量
	for (i = 0; i < G.vexnum; ++i) //初始化邻接矩阵
		for (j = 0; j < G.vexnum; ++j)
			G.arcs[i][j] = { INFINITY,NULL }; //{adj， info}
	for (k = 0; k < G.arcnum; ++k) { //构造邻接矩阵
		scanf(&v1, &v2, &w); //输入一条边依附的顶点及权值
		i = LocateVex(G, v1);
		j = LocateVex(G, v2); //确定v1和v2在G中的位置
		G.arcs[i][j].adj = w; //弧<v1, v2>的权值
		if (IncInfo)
			Input(*G.acrs[i][j].info); //若弧含有相关信息，则输入
		G.arcs[j][i] = G.arcs[i][j]; //置<v1, v2>的对称弧<v2, v1>
	}
	return OK;
}//CreateUDN

//图的邻接表存储表示
#define MAX_VERTEX_NUM 20
typedef struct ArcNode {
	int adjvex;  //该弧所指向的顶点的位置
	struct ArcNode* nextarc;  //指向下一条弧的指针
	InfoType* info;  //该弧相关信息的指针
}ArcNode;
typedef struct VNode {
	VertexType data;  //顶点信息
	ArcNode* firstarc;  //指向第一条依附该顶点的弧的指针
}VNode,AdjList[MAX_VERTEX_NUM];
typedef struct {
	AdjList vertices;
	int vexnum, arcnum;  //图的当前结点数和弧数
	int kind;  //图的种类标志
}ALGraph;

//有向图的十字链表存储表示
#define MAX_VERTEX_NUM 20
typedef struct ArcBox {
	int tailvex, headvex;  //该弧的尾和头顶点的位置
	struct ArcBox* hlink, * tlink;  //分别为弧头相同和弧尾相同的弧的链域
	InfoType* info;  /；该弧相关信息的指针
}ArcBox;
typedef struct VexNode {
	VertexType data;
	ArcBox* firstin, * firstout;  //分别指向该顶点第一条入弧和出弧
}VexNode;
typedef struct {
	VexNode xlist[MAX_VERTEX_NUM];  //表头向量
	int vexnum, arcnum;  //有向图的当前顶点数和弧数
}OLGraph;

Status CreateDG(OLGraph& G) {
	//采用十字链表存储表示，构造有向图G（G.kind=DG）
	scanf(&G.vexnum, &G.arcnum, &IncInfo);  //IncInfo为0则各弧不含其他信息
	for (i = 0; i < G.vexnum; ++i) {  //构造表头向量
		scanf(&G.xlist[i].data);  //输入顶点值
		G.xlist[i].firstin = NULL;
		G.xlist[i].firstout = NULL;  //初始化指针
	}
	for (k = 0; k < G.arcnum; ++k) {  //输入各弧并构造十字链表
		scanf(&v1, &v2);  //输入一条弧的始点和终点
		int i = LocateVex(G, v1);
		int j = LocateVex(G, v2);  //确定v1和v2在G中的位置
 		p = (ArcBox* )malloc(sizeof(ArcBox));  //假定有足够空间
		*p = { i,j,G.xlist[j].firstin,G.xlist[i].firstout,NULL };  //对弧结点赋值
		// { tailvex，headvex，hlink，tlink，info }
		G.xlist[j].firstin = G.xlist[i].firstout = p;  //完成在入弧和出弧链头的插入
		if (IncInfo)
			Input(*p->info);  //若弧含有相关信息，则输入
	}
}//CreateDG

//无向图的邻接多重表存储表示
#define MAX_VERTEX_NUM 20
typedef emnu{ unvisited,visited } VisitIf;
typedef struct EBox {
	VisitIf mark;  //访问标记
	int ivex.jvex;  //该边依附的两个顶点的位置
	struct EBox* ilink, * jlink;  //分别指向依附这两个顶点的下一条边
	InfoType* info;  //该边信息指针
}EBox;
typedef struct VexBox {
	VertexType data;
	EBox* firstedge;  //指向第一条依附该顶点的边
}VexBox;
typedef struct {
	VexBox adjmulist[ MAX_VERTEX_NUM ];
	int vexnum, edgenum;  //无向图的当前顶点数和边数
}AMLGraph;

//接下来两个算法使用的全局变量
Boolean visited[MAX];  //
Status(*VisitFunc)(int v);

void DFSTraverse(Graph G, Status(*Visit)(int v)) {  //对图G作深度优先遍历
	VisitFunc = Visit;  //使用全局变量VisitFunc，使DFS不必设函数指针参数
	for (v = 0; v < G.vexnum; ++v)
		visited[v] = FALSE;  //访问标志数组初始化
	for (v = 0; v < G.vexnum; ++v)
		if (!visited[v])
			DFS(G, v);  //对尚未访问的顶点调用DFS
}

void DFS(Graph G, int v) {
	//从第v个顶点出发递归地深度优先遍历图G
	visited[v] = TRUE;  VisitFunc(v);  //访问第v个顶点
	for (w = FirstAdjVex(G, v); w >= 0; w = NextAdjVex(G, v, w))
		if (!visited[w])
			DFS(G, w);  //对v的尚未访问的邻接顶点w递归调用DFS
}

//广度优先遍历算法
void BFSTraverse(Graph G, Status(*Visit)(int v)) {
	//按广度优先非递归遍历图G。使用辅助队列Q和访问标志数组visited
	for (v = 0; v < G.vexnum; ++v)
		visited[v] = FALSE;
	InitQueue(Q);  //置空的辅助队列Q
	for(v=0;v<G.vexnum;++v)
		if (!visited[v]) {  //v尚未访问
			visited[v] = TRUE; Visit(v);
			EnQueue(Q, v);  //v入队列
			while (!QueueEmpty(Q)) {
				DeQueue(Q, u);  //队头元素出队并置为u
				for(w = FirstAdjVex(G, u); w >= 0; w = NextAdjVex(G, u, w))
					if (!Visited[w]) {  //w为u的尚未访问的邻接顶点
						Visited[w] = TRUE; Visit(w);
						EnQueue(Q, w);
					}//if
			}//while
		}//if
}//BFSTreverse

void DFSForest(Graph G, CSTree& T) {
	//
	//
	T = NULL;
	for (v = 0; v < G.vexnum; ++v)
		visited[v] = FALSE;
	for(v=0;v<G.vexnum;++v)
		if (!visited[v]) {
			p = (CSTree)malloc(sizeof(CSNode));
			*p = { GetVex(G,v),NULL,NULL };
			if (!T) T = p;
			else q->nextsibling = p;
			q = p;
			DFSTree(G, v, p);
		}
}//DFSForest


