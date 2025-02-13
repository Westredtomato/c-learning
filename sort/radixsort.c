//基数排序
//非比较型的整数排序算法
//通过将数字分解成不同的位（通常是从最低位开始）来排序
//每一位的排序一般使用稳定的排序算法（如计数排序）
/*步骤：
选择基数：基数通常是数位系统的进制数。处理十进制数，基数为10。处理二进制数，基数为2
按每一位排序：首先按数字的最低位进行排序，然后再按次低位、次次低位等依次排序，直到处理完所有位
稳定性要求：排序过程中必须使用稳定的排序算法。稳定排序保证相等的元素相对顺序不变
*/

//最高位优先法MSD，最低位优先法LSD

#include <stdio.h>
#include <stdlib.h>

// 打印数组
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 计数排序：按某一位进行排序
void countingSort(int arr[], int n, int exp) {
    // 使用 malloc 动态分配内存
    int* output = (int*)malloc(n * sizeof(int));  // 分配大小为 n 的整数数组
    int count[10] = {0};  // 用于存储每个数字出现的次数

    // 计算每个数字在当前位上的出现次数
    for (int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
    }

    // 计算累计次数
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // 从后向前填充结果数组，保证稳定性
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    // 将结果数组复制回原数组
    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    // 使用 free 释放动态分配的内存
    free(output);
}

// 基数排序主函数
void radixSort(int arr[], int n) {
    // 找到数组中最大的数，以确定排序的位数
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    // 按每一位进行排序
    for (int exp = 1; max / exp > 0; exp *= 10) {
        countingSort(arr, n, exp);
        printf("After sorting by digit %d: \n", exp);
        printArray(arr, n);  // 打印当前数组状态
    }
}

int main() {
    int arr[] = {268,855,413,507,320};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: \n");
    printArray(arr, n);

    // 执行基数排序
    radixSort(arr, n);

    printf("Sorted array: \n");
    printArray(arr, n);

    return 0;
}
