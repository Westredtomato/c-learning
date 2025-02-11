//顺序查找线性表，增加哨兵版
//ASL平均查找长度,ASL=n*(n+1)/2 *1/n=(1+n)/2
//O（n）

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
    list->num = 1;  //从第二个索引开始存
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
    int i;
    list->data[0] = key;
    for (i = (list -> num) - 1; list -> data[i] != key; i--) {
        printf("i = %d\n", i);
    }
    return i;
}

int main(){
    List *list = initList(5);
    listAdd(list, 4);
    listAdd(list, 5);
    listAdd(list, 6);
    listAdd(list, 7);

    printList(list);

    printf("%d\n", search(list, 4));

    return 0;
}