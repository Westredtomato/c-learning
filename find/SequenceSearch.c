//顺序查找
//ASL平均查找长度
//O（n）
//顺序查找则线性表的存储方式没有特定要求,既可以是顺序存储，也可以是链式存储。

#include <stdio.h>
#include <stdlib.h>

typedef struct List{
    int *data;
    int length;
    int num; //代表有多少个元素
} List;

List* initList(int length){ //线性表
    List *list = (List *)malloc(sizeof(List));
    list->length = length;
    list->data = (int *)malloc(sizeof(int) * length);
    list->num = 0;
    return list;
}

void listAdd(List *list, int data){
    list->data[list->num] = data;
    list->num = (list->num) + 1;
}

void printList(List* list){
    for (int i = 0; i < list->num; i++){
        printf("%d -> ", list->data[i]);
    }
    printf("NULL\n");
}

int search(List *list, int key){
    for (int i = 0; i < list->num; i++){
        if (list->data[i] == key)
            return i;  //返回索引
    }
    return -1;
}

int main(){
    List *list = initList(5);
    listAdd(list, 1);
    listAdd(list, 2);
    listAdd(list, 3);
    listAdd(list, 4);

    printList(list);

    printf("%d\n", search(list, 2));

    return 0;
}