//插冒归很稳定（直接插入、冒泡排序、归并排序）
//时间复杂度是O（n^2）:直接选择，冒泡排序，直接插入
//选择排序

//R2i是Ri的左孩子，R2i+1是Ri的右孩子（从0开始数
//R2i+1是Ri的左孩子，R2i+2是Ri的右孩子（从1开始数
//R(i-1)/2是Ri的父节点（整数除法
//将无序序列建成一个堆，得到关键字最大（最小）的纪录
//输出堆顶最大/最小值，使得剩下n-1个元素的序列重新堆，得到n个元素的次小值
//重新执行，可得到一个有序序列
//构建最大堆的时间复杂度是 O(n),每次从堆顶开始调整堆的时间复杂度是 O(log n)
//进行 n-1 次调整，因此堆排序的时间复杂度为 O(n log n)
//不稳定排序

#include <stdio.h>

void printArray(int array[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

// 调整堆，使其符合最大堆的性质
void heapify(int array[], int length, int i) {
    int largest = i; // 初始化最大元素为根节点
    int left = 2 * i + 1; // 左子节点索引
    int right = 2 * i + 2; // 右子节点索引

    // 如果左子节点大于根节点
    if (left < length && array[left] > array[largest]) {
        largest = left;
    }

    // 如果右子节点大于根节点
    if (right < length && array[right] > array[largest]) {
        largest = right;
    }

    // 如果最大值不是根节点，交换并递归堆化
    if (largest != i) {
        int temp = array[i];
        array[i] = array[largest];
        array[largest] = temp;

        // 递归堆化子树
        heapify(array, length, largest);
    }
}

// 堆排序主函数
void heapSort(int array[], int length) {
    // 构建最大堆
    for (int i = length / 2 - 1; i >= 0; i--) { 
        // 从数组的最后一个非叶子节点自底向上开始堆化，（非叶子节点才需要堆化
        heapify(array, length, i);
    }

    // 一个个取出元素，从堆中取出最大元素
    for (int i = length - 1; i >= 1; i--) {
        // 将当前根节点（最大元素）与堆的最后一个元素交换
        int temp = array[0];
        array[0] = array[i];
        array[i] = temp;

        // 调整剩余的堆
        heapify(array, i, 0);
    }
}

int main() {
    int array[8] = {29, 18, 25, 47, 58, 12, 51, 10};
    printArray(array, 8);  // 输出排序前的数组
    heapSort(array, 8);  // 调用堆排序函数
    printArray(array, 8);  // 输出排序后的数组
    return 0;
}
