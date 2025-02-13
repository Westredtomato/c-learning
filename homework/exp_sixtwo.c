#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 15  // 哈希表的大小，可以适当调整

typedef struct Person {
    char name[20];  // 姓名
    char phone[20]; // 电话号码
} Person;

typedef struct HashList {
    Person* data[NUM];  // 存储电话簿的哈希表
} HashList;

// 哈希函数，使用字符串的简单哈希值
int hash(char* name) {
    int hashValue = 0;   // 初始化哈希值为0
    for (int i = 0; name[i] != '\0'; i++) {   // 循环遍历字符串中的每个字符
        hashValue = (hashValue + name[i]) % NUM;   // 更新哈希值
    }
    return hashValue;   // 返回计算出来的哈希值
}

// 初始化哈希表
HashList* initHashTable() {
    HashList* table = (HashList*)malloc(sizeof(HashList));
    for (int i = 0; i < NUM; i++) {
        table->data[i] = NULL; // 初始化所有位置为空
    }
    return table;
}

// 释放哈希表的内存
void freeHashTable(HashList* table) {
    for (int i = 0; i < NUM; i++) {
        if (table->data[i] != NULL) {
            free(table->data[i]);  // 释放每个非空条目的内存
        }
    }
    free(table);  // 释放哈希表的内存
}

// 插入电话号码
void insertPhone(HashList* table, char* name, char* phone) {
    int index = hash(name);  // 计算姓名对应的哈希值，得到应该插入的位置
    int originalIndex = index;  // 保存原始的哈希值，用于判断哈希表是否已满
    int probeCount = 0;  // 探测计数器，用于防止进入死循环 
    while (table->data[index] != NULL) { // 算出的哈希值的表位置已经有数据
        if (strcmp(table->data[index]->name, name) == 0) {
            // 如果已经有这个名字，更新电话号码
            strcpy(table->data[index]->phone, phone);
            return;
        }
        // 如果当前位置有数据且不是目标名字，即发生冲突，使用线性探测
        index = (index + 1) % NUM;  // 线性探测：尝试下一个位置
        probeCount++;  // 增加探测次数
        // 如果探测次数等于哈希表的大小，说明已经尝试了所有槽位
        if (probeCount == NUM) {
            printf("哈希表已满，无法插入新元素\n");
            return;
        }
        // 如果探测回到原始位置，说明循环一遍哈希表，也可以停止插入
        if (index == originalIndex) {
            // 如果又回到原始位置，说明哈希表已满，不能再插入
            printf("哈希表已满\n");
            return;
        }
    }
    // 如果找到空位，插入新数据
    table->data[index] = (Person*)malloc(sizeof(Person));
    strcpy(table->data[index]->name, name);
    strcpy(table->data[index]->phone, phone);
}

// 查找电话号码
char* searchPhone(HashList* table, char* name) {
    int index = hash(name);
    int originalIndex = index;
    while (table->data[index] != NULL) { // 该格有数据
        if (strcmp(table->data[index]->name, name) == 0) {
            return table->data[index]->phone;
        }
        index = (index + 1) % NUM; // 线性探测：尝试下一个位置
        if (index == originalIndex) {
            break;
        }
    }
    return NULL; // 如果没有找到
}

// 打印哈希表
void printHashTable(HashList* table) {
    printf("当前电话簿：\n");
    for (int i = 0; i < NUM; i++) {
        if (table->data[i] != NULL) {
            printf("位置 %d: %s -> %s\n", i, table->data[i]->name, table->data[i]->phone);
        }
    }
}

int main() {
    int choice;
    char nameToSearch[20];
    HashList* table = initHashTable();

    // 插入一些示例数据
    insertPhone(table, "xiaoxiexie", "80102");
    insertPhone(table, "xiaochenchen", "80104");
    insertPhone(table, "xiaofengfeng", "80105");
    insertPhone(table, "choupidan", "01234");
    insertPhone(table, "qiudou", "56789");
    insertPhone(table, "bike", "10086");
    insertPhone(table, "renjitian", "11001");
    insertPhone(table, "hetianyou", "44011");
    insertPhone(table, "lukasi", "55599");
    insertPhone(table, "jicunxi", "97797");
    insertPhone(table, "chenguilin", "00777");
    insertPhone(table, "lanyu", "52334");

    // 打印哈希表内容
    printHashTable(table);

    // 主循环，允许用户多次进行查找操作
    do{
        printf("1.查找电话号码\n2.退出查询\n");
        scanf("%d", &choice);

        switch(choice){
            case 1:
                // 允许用户输入需要查询的同学名字
                printf("请输入要查询的同学名字: ");
                scanf("%s", nameToSearch);  // 从键盘读取名字

                // 查找电话号码
                char* phone = searchPhone(table, nameToSearch);
                if (phone != NULL) {
                    printf("%s的电话号码是: %s\n", nameToSearch, phone);
                } else {
                    printf("没有找到%s的电话号码\n", nameToSearch);
                }
                break;
            case 2:
                printf("退出程序。\n");
                break;
            default:
                printf("无效的选择，请重新输入。\n");
        }
    } while (choice != 2);

    // 释放哈希表内存
    freeHashTable(table);

    return 0;
}
