//插入排序
//一个个比较然后把数字插到合适的地方
//最好的情况（顺序有序）：比较n-1次，移动0次
//最坏的情况（逆序有序）：比较(n+2)(n-1)/2，移动(n+4)(n-1)/2
//平均情况：O(n^2)
//稳定

#include<stdio.h>

void printArray(int array[], int length){
    for (int i = 0; i < length; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

void insertSort(int array[], int length){
    //外层循环，从第二个元素开始比较，比较从零到他们前一个元素之间有没有合适位置插入
    for (int i = 1; i < length; i++){
        for (int j = 0; j < i; j++){
            if (array[i] < array[j]){
                int temp = array[i];
                for (int k = i - 1; k >= j; k--){
                    array[k + 1] = array[k];  //从后往前移该元素
                }
                array[j] = temp;
            }
        }
        printArray(array, length);
    }
}

int main(){
    int array[10] = {10, 18, 4, 3, 6, 12, 1, 9, 18, 8};
    printArray(array, 10);
    insertSort(array, 10);
    return 0;
}