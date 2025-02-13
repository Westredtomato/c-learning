//冒泡排序，两两比较
//两两比较，找出最大的放最后，然后到第二大，...
//序列长度为n,需要进行n-1次冒泡（外层循环），内层n-1-i
//交换排序

#include<stdio.h>

void printArray(int array[], int length){
    for (int i = 0; i < length; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

void bubbleSort(int array[], int length){
    //外层循环
    for (int i = 0; i < length - 1; i++){
        //内层
        for (int j = 0; j < length - 1 - i; j++){
            if (array[j] > array[j + 1]){ // 后面的数大于前面的
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
        printArray(array, length);
    }
}

int main(){
    int array[8] = {49, 38, 65, 97, 76, 13, 27, 49};
    bubbleSort(array, 8);
    return 0;
}