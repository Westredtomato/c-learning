#include <stdio.h>
#include <stdlib.h>

// 定义线性表结构体
typedef struct List {
    int *data;       // 存储数据的数组
    int length;      // 数组的最大长度
    int num;         // 当前元素数量
} List;

// 初始化线性表
List* initList(int length) {
    List *list = (List *)malloc(sizeof(List));
    list->data = (int *)malloc(sizeof(int) * length);
    list->length = length;
    list->num = 0;
    return list;
}

// 向线性表添加数据
void listAdd(int data, List* list) {
    if (list->num < list->length) {
        list->data[list->num++] = data;
    } else {
        printf("列表已满，无法添加更多元素。\n");
    }
}

// 打印线性表
void printList(List* list) {
    for (int i = 0; i < list->num; i++) {
        printf("%d ", list->data[i]);
    }
    printf("\n");
}

// 顺序查找
int sequentialSearch(List *list, int key) {
    for (int i = 0; i < list->num; i++) {
        if (list->data[i] == key) {
            return i;  // 返回找到的索引
        }
    }
    return -1;  // 没有找到返回-1
}

// 折半查找
int binarySearch(int key, List *list) {
    int start = 0;
    int end = list->num - 1;
    while (start <= end) {
        int mid = (start + end) / 2;
        if (list->data[mid] < key) {
            start = mid + 1;  // 中间值小于目标值，搜索右半部分
        } else if (list->data[mid] > key) {
            end = mid - 1;  // 中间值大于目标值，搜索左半部分
        } else {
            return mid;  // 找到目标值，返回索引
        }
    }
    return -1;  // 没有找到返回-1
}

// 辅助函数，用于qsort排序
int compareIntegers(const void *a, const void *b) {//比较函数，用于告诉 qsort 如何比较两个元素
    return (*(int*)a - *(int*)b);//如果 *a 小于 *b，返回负数，等于返回 0，大于返回正数
}

int main() {
    int length, key, choice;
    char ch;

    printf("请输入数组长度: ");
    scanf("%d", &length);

    // 初始化列表
    List *list = initList(length);

    // 输入数组元素
    printf("请输入 %d 个整数:\n", length);
    for (int i = 0; i < length; i++) {
        int value;
        scanf("%d", &value);
        listAdd(value, list);
    }

    // 打印输入的数组
    printf("输入的数组: ");
    printList(list);

    // 主循环，允许用户多次进行查找操作
    do {
        printf("请选择查找方式:\n1. 顺序查找\n2. 折半查找\n0. 退出\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:  // 顺序查找
                printf("请输入要查找的记录: ");
                scanf("%d", &key);
                int indexSeq = sequentialSearch(list, key);
                if (indexSeq != -1) {
                    printf("顺序查找结果: 记录 %d 在位置 %d\n", key, indexSeq);
                } else {
                    printf("顺序查找结果: 记录 %d 不在数组中\n", key);
                }
                break;
            case 2:  // 折半查找
                // 排序数组以便进行折半查找
                qsort(list->data, list->num, sizeof(int), compareIntegers);
                //要排序的数组，数组中的元素数量，每个元素的大小，比较函数，用于确定排序顺序

                // 打印排序后的数组
                printf("排序后的数组: ");
                printList(list);

                printf("请输入要进行折半查找的记录: ");
                scanf("%d", &key);

                //进行折半查找
                int indexBin = binarySearch(key, list);
                if (indexBin != -1) {
                    printf("折半查找结果: 记录 %d 在位置 %d\n", key, indexBin);
                } else {
                    printf("折半查找结果: 记录 %d 不在数组中\n", key);
                }
                break;
            case 0:  // 退出
                printf("退出程序。\n");
                break;
            default:
                printf("无效的选择，请重新输入。\n");
        }
    } while (choice != 0);

    // 释放内存
    free(list->data);
    free(list);

    return 0;
}