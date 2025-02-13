#include <stdio.h>
#include <stdlib.h>

// 打印数组
void printArray(int array[], int length){
    for (int i = 0; i < length; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

// 希尔排序（Shell Sort）实现
void shellSort(int array[], int length){
    int step = length / 2;
    while (step > 0){
        for (int i = 0; i < length; i++){
            for (int j = i + step; j < length; j += step){  //j是在i后面的组员
                for (int k = i; k < j; k += step){  //k是在j后面的组员
                    if(array[j] < array[k]){
                        int temp = array[j];
                        for (int l = j - step; l >= k; l -= step){  //覆盖掉前一个数
                            array[l + step] = array[l];
                        }
                        array[k] = temp;
                    }
                }
            }
        }
        step /= 2; // 缩小步长
    }
}

// 快速排序（Quick Sort）实现
int partition(int array[], int i, int j){
    int x = array[i];  // 选择第一个元素 array[i] 作为枢纽 x
    while (i < j) {  // 当左右指针没有相遇时继续
        while (i < j && array[j] >= x) j--;  // 右侧扫描，找小于枢纽的元素
        if (i < j) {
            array[i] = array[j];  // 将右侧元素放到左侧
            i++;  // 左指针右移
        }
        while (i < j && array[i] < x) i++;  // 左侧扫描，找大于枢纽的元素
        if (i < j) {
            array[j] = array[i];
            j--;
        }
    }
    array[i] = x;  // 将枢纽放到正确位置
    return i;  // 返回枢纽的位置
}

void quickSort(int array[], int i, int j){
    if(i < j){
        int index = partition(array, i, j);  // 分区
        quickSort(array, i, index - 1);  // 排序左部分
        quickSort(array, index + 1, j);  // 排序右部分
    }
}


// 堆排序（Heap Sort）实现
void heapify(int array[], int n, int i){
    int largest = i;  // 假设当前节点是最大节点
    int left = 2 * i + 1;  // 左孩子
    int right = 2 * i + 2; // 右孩子

    // 如果左子树更大
    if (left < n && array[left] > array[largest])
        largest = left;

    // 如果右子树更大
    if (right < n && array[right] > array[largest])
        largest = right;

    // 如果最大值不是根节点，则交换并递归
    if (largest != i) {
        int temp = array[i];
        array[i] = array[largest];
        array[largest] = temp;

        heapify(array, n, largest);
    }
}

void heapSort(int array[], int n){
    // 构建堆（从最后一个非叶子节点开始）
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(array, n, i);
    }

    // 一个个取出元素
    // 交换堆顶与堆尾，重新堆化
    for (int i = n - 1; i > 0; i--) {
        // 将当前根节点（最大值）与末尾元素交换（交换堆顶和堆尾
        int temp = array[0];
        array[0] = array[i];
        array[i] = temp;

        // 重新堆化
        heapify(array, i, 0);
    }
}

int main(){
    int choice, length;
    int *array;
    int cont = 1;  // 控制是否继续

    printf("请输入数组的长度: ");
    scanf("%d", &length);

    // 表达式必须含有常量值，不可直接int array[length];
    // 使用 malloc 动态分配内存
    array = (int*)malloc(length * sizeof(int));
    if (array == NULL) {
        printf("内存分配失败!\n");
        return 1;
    }

    printf("请输入数组元素: ");
    for (int i = 0; i < length; i++){
        scanf("%d", &array[i]);
    }

    while (cont) {

        printf("请选择排序算法:\n");
        printf("1. 快速排序\n");
        printf("2. 希尔排序\n");
        printf("3. 堆排序\n");
        printf("4. 退出\n");
        scanf("%d", &choice);

        // 根据用户选择的排序算法进行排序
        switch(choice) {
            case 1:
                printf("使用快速排序进行排序:\n");
                quickSort(array, 0, length - 1);
                break;
            case 2:
                printf("使用希尔排序进行排序:\n");
                shellSort(array, length);
                break;
            case 3:
                printf("使用堆排序进行排序:\n");
                heapSort(array, length);
                break;
            case 4:
                printf("退出程序。\n");
                free(array);  // 退出时释放内存
                return 0;
            default:
                printf("无效的选择!\n");
                free(array);  // 如果选择无效，释放内存
                return 1;
        }

        printf("排序后的数组:\n");
        printArray(array, length);

        // 总结
        printf("\n排序算法总结:\n");
        switch(choice) {
            case 1:
            printf("快速排序：\n");
                printf("快速排序是一种分治法，首先选择一个枢纽，将数组分成两部分，\n");
                printf("然后递归地对这两部分进行排序，最后得到排序结果。时间复杂度平均为O(nlogn)，\n");
                printf("最坏情况下为O(n^2)，但通常表现较好。\n");
                break;
            case 2:
                printf("希尔排序：\n");
                printf("希尔排序是插入排序的改进版，通过对数组进行分组来优化插入排序的效率，\n");
                printf("其时间复杂度依赖于增量序列的选择。希尔排序不稳定，适用于较大的数据集。\n");
                break;
            case 3:
                printf("堆排序：\n");
                printf("堆排序是一种选择排序，利用堆这种数据结构来排序数组，时间复杂度为O(nlogn)，\n");
                printf("并且堆排序是稳定的，但需要额外的空间来实现堆结构。\n");
                break;
        }
        
        // 询问用户是否继续
        printf("\n是否继续选择排序方式? (1. 是 / 0. 否): ");
        scanf("%d", &cont);

    }
    
    // 释放内存
    free(array);
    return 0;
}
