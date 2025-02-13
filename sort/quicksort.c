//快速排序
//在数组中选择一个数字作为枢纽，将其他数与它进行比较，可以确定枢纽的相对位置
//再进行第二趟排序，对左子表，选择一个枢纽，排序，再对左子表的左子表，右子表等等
//指针指向头和尾，
//指针在右边的时候从右往左一直扫，直到找到比轴点小的，交换，然后左指针往右移
//指针重合的时候就是枢纽的位置
//跳跃式取值和交换，不稳定
//队链表无法快速排序，对顺序表可以
//属于交换排序
//最好情况(每次总选到中间值做枢纽):O(nlog2n)
//最坏情况(每次总选到最大或最小值做枢纽):O(n^2)

//交换排序：
//最好情况：比较n-1，移动0
//最坏情况：比较n(n-1)/2，移动n(n-1)/2

#include<stdio.h>

void printArray(int array[], int length){
    for (int i = 0; i < length; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

int partition(int array[], int i, int j){
    //在数组中选择一个枢纽，将<枢纽的元素放左边，将>=枢纽的元素放右边
    int x = array[i];//选择第一个元素 array[i] 作为枢纽 x
    while (i < j){ // 当左右指针没有相遇时继续
        while (i < j && array[j] >= x){ // 右侧扫描，找小于枢纽的元素
            j--;
        }
        if (i < j){
            array[i] = array[j]; // 将右侧元素放到左侧
            i++; // 左指针右移
        }
        while (i < j && array[i] < x){ // 左侧扫描，找大于枢纽的元素
            i++;
        }
        if (i < j){
            array[j] = array[i]; // 将左侧元素放到右侧
            j--; // 右指针左移
        }
    }
    array[i] = x; // 将枢纽放到正确位置
    return i; // 返回枢纽的最终位置
}

void quickSort(int array[], int i, int j){
    //不断调用 partition 将数组分成两个部分，分别对两部分排序
    //递归出口，退出的条件
    if(i < j){
        int index = partition(array, i, j);
        printArray(array, 10);
        quickSort(array, i, index - 1);
        quickSort(array, index + 1, j);
    }
}

int main(){
    int array[10] = {36,30,60,40,68,25,57,100,333,13};
    printf("原始序列：\n");
    printArray(array, 10);
    printf("排序过程：\n");
    quickSort(array, 0, 9);
    printf("排序后：\n");
    printArray(array, 10);
    return 0;
}