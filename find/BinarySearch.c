//二分查找,又称折半查找
//时间复杂度为O（log2n）
//要求线性表必须有序，mind=(start+end)/2
//满二叉树h层查找h次，结点n=2^h-1,每一层有2^(h-1),,每一层查找次数2^(h-1)*h
//ASL=2^(h-1)*h*（1/n），（每一层的相加，h从j=1到j=h）,ASL=[(n+1)log2(n+1)]/n-1
//折半查找要求线性表中的结点按关键字值大小有序，并且线性表必须采用顺序存储方式

#include <stdio.h>
#include <stdlib.h>

typedef struct List{
    int *data;
    int length;
    int num;
} List;

List* initList(int length){
    List *list = (List *)malloc(sizeof(List));
    list->data = (int *)malloc(sizeof(int) * length);
    list->length = length;
    list->num = 0;
    return list;
}

void listAdd(int data,List* list){
    list->data[list->num] = data;
    list->num = list->num + 1;
    //可以检查num和length是否超出长度
}

void printList(List* list){
    for (int i = 0; i < list->num; i++){
        printf("%d ", list->data[i]);
    }
    printf("\n");
}

int binarySearch(int key, List *list){ //二分查找
    int start = 0;
    int end = list->num - 1;
    int mid;
    while (start <= end){
        mid = (start + end) / 2;
        if (list->data[mid] < key){ //中间的数小于目标数
            start = mid + 1;  //mid右移
        }
        else if (list->data[mid] > key){
            end = mid - 1;
        }
        else
            return mid;
    }
    return -1;
}

int binarySearchRecursion(int key, List *list, int start, int end){ //递归算法
    if(start == end){ //递归出口
        if (list->data[start] == key){
            return start;
        }
        else{
            return -1;
        }
    }
    int mid = (start + end) / 2;
    if (list->data[mid] < key) {
        return binarySearchRecursion(key, list, mid + 1, end);
    }
    else if(list->data[mid] > key){
        return binarySearchRecursion(key, list, start, mid - 1);
    }
    else
        return mid;
}

int main(){
    List *list = initList(9);

    listAdd(1, list);
    listAdd(2, list);
    listAdd(3, list);
    listAdd(4, list);
    listAdd(5, list);
    listAdd(6, list);
    listAdd(7, list);
    listAdd(8, list);
    listAdd(9, list);

    printList(list);

    printf("data %d in %d\n", 7, binarySearch(7, list));
    printf("data %d in %d\n", 10, binarySearch(10, list));
    printf("data %d in %d\n", 1, binarySearch(1, list));

    printf("data %d in %d\n", 7, binarySearchRecursion(7, list, 0, list->num - 1));
    printf("data %d in %d\n", 10, binarySearchRecursion(10, list, 0, list->num - 1));
    printf("data %d in %d\n", 1, binarySearchRecursion(1, list, 0, list->num - 1));

    return 0;
}