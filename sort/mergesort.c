//2-路归并排序
//将两个或两个以上的有序子序列归并成一个有序序列
//分治法
//通过递归将数组分成两部分，对每一部分进行排序，然后合并成一个有序数组
//归并排序的时间复杂度是 O(n log n)，空间复杂度是 O(n)
//步骤：分解：将数组递归地分割成两部分，直到每个子数组的大小为1（即只有一个元素）
//合并：将两个有序的子数组合并成一个有序的数组

#include <stdio.h>
#include<stdlib.h> // 分配内存

// 打印数组
void printArray(int arr[], int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 合并
void merge(int arr[], int tempArr[], int left, int mid, int right){

    //标记左半区第一个未排序的元素
    int l_pos = left;
    //标记右半区第一个未排序的元素
    int r_pos = mid + 1;
    //临时数组元素的下标
    int pos = left;

    //合并
    while (l_pos <= mid && r_pos <= right){
        if (arr[l_pos] < arr[r_pos]) // 左半区第一个剩余元素更小
            tempArr[pos++] = arr[l_pos++];
        else                       //右半区第一个剩余元素更小
            tempArr[pos++] = arr[r_pos++];
    }

//只有一种情况发生，要么左半区域剩余要么右半区域剩余
    //合并左半区域剩余的元素
    while (l_pos <= mid)
        tempArr[pos++] = arr[l_pos++];

    //合并右半区域剩余的元素
    while (r_pos <= right)
        tempArr[pos++] = arr[r_pos++];

    //把临时数组中合并后的元素复制回原来的数组
    while (left <= right){
        arr[left] = tempArr[left];
        left++;
    }
}

//归并排序
void msort(int arr[], int tempArr[], int left, int right){
    //如果只有一个元素，那么不需要划分
    //只有一个元素的区域本身就是有序的,只需要被归并即可
    if (left < right){
        //找中间点
        int mid = (right - left) / 2 + left;
        //递归划分左半区域
        msort(arr, tempArr, left, mid);
        //递归划分右半区域
        msort(arr, tempArr, mid + 1, right);
        //合并已经排序的部分
        merge(arr, tempArr, left, mid, right);
    }
}

// 归并排序入口
void mergeSort(int arr[], int length){

    // 分配一个辅助数组
    int *tempArr = (int *)malloc(length * sizeof(int));
    if(tempArr){ // 辅助数组分配成功
        msort(arr, tempArr, 0, length - 1);
        free(tempArr);
    }
    else{
        printf("error:failed to allocate memory.\n");
    }
}

int main(){
    int arr[] = {29, 18, 25, 47, 58, 12, 51, 10};
    int length = 8;

    printf("Original array: \n");
    printArray(arr, length);

    // 执行归并排序
    mergeSort(arr, length);

    printf("\nSorted array: \n");
    printArray(arr, length);

    return 0;
}
