//希尔排序，缩小增量排序
//核心是对数据进行分组，根据不同的步长（相隔距离）分组，比如第1个数，第1+n个数，第1+2n个数，...为一组
//分别对这几组进行插入排序，接下对形成的序列重新缩小步长来排序
//1.从以k为步长开始，对序列进行分组，对组内的数字进行插入排序
//2.缩小步长k，重复第一步，直到k=1
//跳跃式取值，不稳定
//插入排序

#include<stdio.h>

void printArray(int array[], int length){
    for (int i = 0; i < length; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

void shellSort(int array[], int length, int step){
    for (int i = 0; i < length; i++){
        for (int j = i + step; j < length; j += step){ //j是在i后面的组员
            for (int k = i; k < j; k += step){ //k是在j后面的组员
                if(array[j] < array[k]){
                    int temp = array[j];
                    for (int l = j - step; l >= k; l -= step){ //覆盖掉前一个数
                        array[l + step] = array[l];
                    }
                    array[k] = temp;
                }
            }
        }
    }
}

int main(){
    int array[10] = {12, 18, 25, 3, 6, 11, 32, 9, 27, 8};
    int steps[3] = {5, 3, 1};
    for (int i = 0; i < 3; i++){
        shellSort(array, 10, steps[i]);
        printArray(array, 10);
    }
    return 0;
}
