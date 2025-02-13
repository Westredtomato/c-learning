//借助快速排序的思想，在n个元素的序列中，求出第k个最小的元素

#include <stdio.h>
#include <stdlib.h>

// 用于交换数组中的两个元素
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 分区操作，将数组分为两部分，小于基准值的部分在左边，大于基准值的部分在右边
int partition(int array[], int left, int right) {
    // 选择最后一个元素作为基准元素
    int pivot = array[right];
    int i = left - 1;

    // 遍历数组，将小于基准值的元素移动到左边
    for (int j = left; j < right; j++) {
        if (array[j] < pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    
    // 将基准元素放到它最终的位置
    swap(&array[i + 1], &array[right]);

    // 返回基准元素的位置
    return i + 1;
}

// 快速选择算法，查找第k个最小的元素
int quickSelect(int array[], int left, int right, int k) {
    if (left == right) {
        return array[left];
    }

    // 分区操作，基准元素的最终位置
    int pivotIndex = partition(array, left, right);

    // 如果基准元素的位置正好是k，返回该元素
    if (pivotIndex == k) {
        return array[pivotIndex];
    } 
    // 如果k小于基准元素的位置，继续在左半部分查找
    else if (k < pivotIndex) {
        return quickSelect(array, left, pivotIndex - 1, k);
    } 
    // 如果k大于基准元素的位置，继续在右半部分查找
    else {
        return quickSelect(array, pivotIndex + 1, right, k);
    }
}

int main() {
    int array[] = {7, 10, 4, 3, 20, 15};
    int n = sizeof(array) / sizeof(array[0]);
    int k;  // 求第k个最小的元素

    // 从键盘输入k
    printf("Enter the value of k (1-based index): ");
    scanf("%d", &k);

    if (k < 1 || k > n) {
        printf("Invalid value of k. It should be between 1 and %d.\n", n);
        return -1;
    }

    // 调用快速选择算法
    int result = quickSelect(array, 0, n - 1, k - 1);  // k-1 是为了转换为0-based索引

    printf("The %dth smallest element is: %d\n", k, result);
    return 0;
}
