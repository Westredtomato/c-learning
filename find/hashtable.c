//哈希表 hash table
//对线性表从0开始编号，尽可能让哈希表分散
//不同的数据利用哈希函数算出相同的值的时候，进行二次哈希，
//1.开放地址法（二次哈希）。2.链式存储法。
//1.新哈希值 b = f（f(data)+a）
//a可以从1开始递增（线性），a也可以按 1^2，-1^2，2^2，-2^2，……（二次），也可以（随机）
//2.封装成一个链式结点，谁有冲突就直接连在后面
//O（1）
//二次探测再散列处理冲突：h(49)=49%11=5,冲突，h1=(h(49)+1^2)%11=6,冲突，h2=(h(49)+2^2)%11=9，冲突解决
//要确定增量序列，如1^1,-1^1,2^2,-2^2

#include <stdio.h>
#include <stdlib.h>

#define NUM 5  // 要整除的数

typedef struct HashList{
    int num;     // 哈希表中的元素数量
    char *data;  // 存储哈希表的内容
} HashList;

// 初始化哈希表
HashList* initList(){
    HashList *list = (HashList *)malloc(sizeof(HashList));
    list->num = 0;
    list->data = (char *)malloc(sizeof(char) * NUM);
    for (int i = 0; i < NUM; i++){
        list->data[i] = 0;
    }
    return list;
}

// hash是一个简单的哈希函数，将传入的数据取模 NUM并返回结果
int hash(int data){
    return data % NUM;  // 计算数据的哈希值（即数据对 NUM 取余）
}

// 线性方法
void put(HashList *list, char data){
    int index = hash(data);      // 计算数据的哈希值
    if (list->data[index] != 0){
        // 如果目标位置已经被占用（发生冲突）
        int counut = 1;  // 冲突计数器
        while (list->data[index] != 0){  // 继续查找直到找到一个空的位置
            index = hash(hash(data) + counut);  // 使用二次哈希计算新的索引
            counut++;
            //如果仍然冲突，counut变成2，3，……
        }
    } 
    // 跳出循环就开始赋值
    list->data[index] = data;
    list -> num ++;  // 增加元素计数
}

int main(){
    HashList *list = initList();
    put(list, 'A');
    put(list, 'F');
    printf("%c\n", list->data[0]);
    printf("%c\n", list->data[1]);
    return 0;
}