//简单选择排序
//先扫，再找，放入最前
//选择排序：简单选择排序，堆排序

//第一个元素与后面的所有进行比较，小的就交换放前面，然后第二个元素跟后面的比，小的放前
//外层循环判断循环次数n-1，内层循环判断
//每一轮选择最小（或最大）元素并与当前位置交换
//比较次数n(n-1)/2

//树形选择排序，时间复杂度O(nlog2n)

#include<stdio.h>

void printArray(int array[], int length){
    for (int i = 0; i < length; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

void xuanzeSort(int array[], int length){
    //外层循环
    for (int i = 0; i < length - 1; i++){
        int minIndex = i; // 假设当前位置为最小元素的位置
        //内层循环，找到从i到末尾最小的元素
        for (int j = i + 1; j < length; j++){
            if (array[j] < array[minIndex]){
                minIndex = j; // 更新最小元素的位置
            }
        }
        // 如果最小元素的位置不是当前位置，进行交换
        if (minIndex != i){
            int temp = array[i];
            array[i] = array[minIndex];
            array[minIndex] = temp;
        }
        printArray(array, length);
    }
}

int main(){
    int array[8] = {49, 38, 65, 97, 76, 13, 27, 49};
    xuanzeSort(array, 8);
    return 0;
}