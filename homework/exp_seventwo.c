#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 学生结构体，包含姓名和成绩
typedef struct {
    char name[50];
    int score;
} Student;

// 打印学生成绩和名次
void printStudentRanks(Student arr[], int length) {
    int rank = 1;  // 初始名次
    for (int i = 0; i < length; i++) {
        // 对于成绩相同的学生，分配相同的名次
        if (i > 0 && arr[i].score != arr[i-1].score) {
            rank = i + 1;  // 如果成绩不同，更新名次
            //i 是从 0 开始的，而名次是从 1 开始的，所以需要加 1
        }
        printf("Rank %d: %s - %d\n", rank, arr[i].name, arr[i].score);
    }
}

// 合并函数：将两个有序的子数组合并成一个有序数组
void merge(Student arr[], Student tempArr[], int left, int mid, int right) {
    int l_pos = left;    // 左半区起始位置
    int r_pos = mid + 1; // 右半区起始位置
    int pos = left;      // 临时数组的起始位置

    // 合并两个子数组
    while (l_pos <= mid && r_pos <= right) {
        if (arr[l_pos].score > arr[r_pos].score) { // 左半区第一个剩余元素更小
            tempArr[pos++] = arr[l_pos++];
        } else { // 右半区第一个剩余元素更小
            tempArr[pos++] = arr[r_pos++];
        }
    }

    // 将左半区剩余的元素添加到临时数组
    while (l_pos <= mid) {
        tempArr[pos++] = arr[l_pos++];
    }

    // 将右半区剩余的元素添加到临时数组
    while (r_pos <= right) {
        tempArr[pos++] = arr[r_pos++];
    }

    // 将排序后的临时数组复制回原数组
    for (int i = left; i <= right; i++) {
        arr[i] = tempArr[i];
    }
}

// 递归分治排序函数
void msort(Student arr[], Student tempArr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        msort(arr, tempArr, left, mid);         // 递归排序左半部分
        msort(arr, tempArr, mid + 1, right);    // 递归排序右半部分
        merge(arr, tempArr, left, mid, right);  // 合并排序结果
    }
}

// 归并排序的入口函数
void mergeSort(Student arr[], int length) {
    Student* tempArr = (Student*)malloc(length * sizeof(Student));
    if (tempArr) {
        msort(arr, tempArr, 0, length - 1);
        free(tempArr);  // 释放临时数组
    } else {
        printf("Memory allocation failed.\n");
    }
}

int main() {
    int n;

    // 输入学生数量
    printf("Enter the number of students: ");
    scanf("%d", &n);

    // 动态分配学生数组
    Student* students = (Student*)malloc(n * sizeof(Student));
    if (students == NULL) {
        printf("Memory allocation failed.\n");
        return -1;
    }

    // 输入学生姓名和成绩
    for (int i = 0; i < n; i++) {
        printf("Enter the name and score of student %d: ", i + 1);
        scanf("%s %d", students[i].name, &students[i].score);
    }

    // 排序学生数组
    mergeSort(students, n);

    // 输出排序结果和学生名次
    printf("\nSorted students by score:\n");
    printStudentRanks(students, n);

    // 释放动态分配的内存
    free(students);

    return 0;
}
