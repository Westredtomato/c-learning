#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义常量
#define MAX_USERS 100
#define MAX_ADMIN 1
#define MAX_SESSIONS 50
#define MAX_HALLS 20
#define MAX_ORDERS 100

// 用户结构体
typedef struct {
    char userId[20];
    char phone[20];
    char password[20];
} User;

// 管理员结构体
typedef struct {
    char adminId[20];
    char cinemaName[50];
    char password[20];
} Admin;

typedef struct {
    char sessionId[20];  // 场次编号
    char cinemaName[50]; // 所属影院
    char hallType[10];   // 影厅类型
    char movieName[100]; // 电影名称
    char startTime[6];   // 放映开始时间
    char endTime[6];     // 放映结束时间
    char language[10];   // 放映类型（英语、中文）
    float price;         // 票价
    int remainingSeats;  // 余票数
    char hallId[20];     // 影厅编号

    int totalSeats;      // 影厅总座位数
    char seats[100][5];  // 每个座位的编号，例如 "A1", "A2", "B1" 等
} MovieSession;

typedef struct {
    char hallId[20]; // 影厅编号
    char hallType[10]; // 类型（2D, 3D, IMAX）
    int totalSeats;   // 影厅总座位数
} CinemaHall;

// 订单结构体
typedef struct {
    char orderId[20];
    char userId[20];
    char sessionId[20];
    char seatNumbers[3][5]; // 购买的座位号
    int seatCount; // 购买的座位数
    float total_price;
    int status; // 0: 未付款, 1: 已付款, 2: 已取消
    char startTime[20];       // 场次开始时间
    char endTime[20];         // 场次结束时间
} Order;

// 全局数据存储
User users[MAX_USERS];
Admin admins[MAX_ADMIN];
MovieSession sessions[MAX_SESSIONS];
CinemaHall halls[MAX_HALLS];
Order orders[MAX_ORDERS];

int userCount = 0;
int adminCount = 0; // 初始化为0，稍后将手动添加管理员信息
int sessionCount = 0;
int hallCount = 0;
int orderCount = 0;

// 函数声明
void login();
void userMenu(int userIndex);
void adminMenu();
void querySessions(int userIndex);
void purchaseTicket(int userIndex);
void cancelOrder(int userIndex);
void viewOrders(int userIndex);
void manageHalls();
void manageSessions();
void statistics();
void modifyUserInfo(int userIndex);
void initCinema(); // 初始化影院信息
int checkSeatConflict(MovieSession *session, char seatNumber[5]);
int loginUser(User users[], int userCount, char *userId, char *password);
int loginAdmin(Admin admins[], int adminCount, char *adminId, char *password);
// 提前声明 findSessionIndex 函数
int findSessionIndex(const char *sessionId);

// 初始化影院信息
void initCinema() {
    // 初始化影厅信息
    CinemaHall hall;
    strcpy(hall.hallId, "01");
    strcpy(hall.hallType, "3D");
    hall.totalSeats = 48;
    halls[hallCount++] = hall;

    // 初始化电影场次信息
    MovieSession session1 = {"0101", "West Tomato", "2D", "惊魂记", "0900", "1100", "英语", 28.8, 48, "01"};
    sessions[sessionCount++] = session1;

    MovieSession session2 = {"0102", "West Tomato", "3D", "好东西", "1000", "1200", "中文", 34.9, 48, "01"};
    sessions[sessionCount++] = session2;

    MovieSession session3 = {"0103", "West Tomato", "IMAX", "艋舺", "1100", "1300", "中文", 35.0, 48, "01"};
    sessions[sessionCount++] = session3;
    
    MovieSession session4 = {"0104", "West Tomato", "3D", "熔炉", "1200", "1400", "韩语", 41.0, 48, "01"};
    sessions[sessionCount++] = session4;
    
    MovieSession session5 = {"0105", "West Tomato", "3D", "寄生虫", "1200", "1400", "韩语", 38.0, 48, "01"};
    sessions[sessionCount++] = session5;
    
    MovieSession session6 = {"0106", "West Tomato", "3D", "活着", "1330", "1530", "中文", 36.8, 48, "01"};
    sessions[sessionCount++] = session6;
    
    MovieSession session7 = {"0107", "West Tomato", "IMAX", "色戒", "1400", "1600", "中文", 38.8, 48, "01"};
    sessions[sessionCount++] = session7;
    
    MovieSession session8 = {"0108", "West Tomato", "2D", "狮子王", "1400", "1600", "英语", 26.8, 48, "01"};
    sessions[sessionCount++] = session8;
}

// 初始化一个 MovieSession，并为每个座位设置一个编号
void initMovieSession(MovieSession *session, const char *sessionId, const char *movieName, const char *hallId, int totalSeats) {
    strcpy(session->sessionId, sessionId);
    strcpy(session->movieName, movieName);
    strcpy(session->hallId, hallId);
    session->totalSeats = totalSeats;
    session->remainingSeats = totalSeats;

    // 初始化所有座位（假设没有座位被预定）
    for (int i = 0; i < totalSeats; i++) {
        sprintf(session->seats[i], "S%d", i + 1);  // 为每个座位命名（例如 S1, S2, S3）
    }
}

// 用户注册函数
void userRegister() {
    if (userCount >= MAX_USERS) {
        printf("用户数量已达上限。\n");
        return;
    }

    char userId[20];
    char phone[20];
    char password[20];
    printf("请输入用户名：");
    scanf("%19s", userId); // 限制输入长度为19，防止溢出
    printf("请输入手机号：");
    scanf("%19s", phone); // 限制输入长度为19，防止溢出
    printf("请输入密码：");
    scanf("%19s", password); // 限制输入长度为19，防止溢出

    User newUser;
    strcpy(newUser.userId, userId);
    strcpy(newUser.phone, phone);
    strcpy(newUser.password, password);

    users[userCount] = newUser;
    userCount++;
    printf("用户 %s 注册成功！\n", userId);
}

// 用户登录
int loginUser(User users[], int userCount, char *userId, char *password) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].userId, userId) == 0 && strcmp(users[i].password, password) == 0) {
            return i;  // 返回用户在数组中的位置
        }
    }
    return -1; // 登录失败
}

// 用户登录函数
void login() {
    char userId[20];
    char password[20];
    printf("请输入用户名：");
    scanf("%19s", userId); // 限制输入长度为19，防止溢出
    printf("请输入密码：");
    scanf("%19s", password); // 限制输入长度为19，防止溢出

    int userIndex = loginUser(users, userCount, userId, password);
    if (userIndex != -1) {
        printf("用户 %s 登录成功！\n", userId);
        userMenu(userIndex); // 用户登录后进入用户菜单
    } else {
        printf("用户名或密码错误。\n");
    }
}

// 管理员登录
int loginAdmin(Admin admins[], int adminCount, char *adminId, char *password) {
    for (int i = 0; i < adminCount; i++) {
        if (strcmp(admins[i].adminId, adminId) == 0 && strcmp(admins[i].password, password) == 0) {
            return i;  // 返回管理员在数组中的位置
        }
    }
    return -1; // 登录失败
}

// 1.查看所有电影场次
void querySessions(int userIndex) {
    int choice;
    char searchName[100];
    char searchLanguage[10];

    do {
        printf("\n");
        printf("West Tomato Cinema--2.查看电影场次"); 
        printf("\n请选择一个查询选项:\n");
        printf("1. 查看所有电影场次\n");
        printf("2. 通过电影名称查询电影\n");
        printf("3. 通过放映语言查询电影\n");
        printf("4. 查询有剩余座位的电影\n");
        printf("0. 返回上一菜单\n");
        printf("请输入选项(0-4): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: { // 查看所有电影场次
                int found = 0; // 标志是否找到电影
                // found变量放到每个case块内, 避免跨越case语句初始化
                printf("欢迎来到 West Tomato Cinema ！ \n");
                printf("类型\t场次\t电影\t\t语言\t开始\t结束\t票价\t余票\n");
                for (int i = 0; i < sessionCount; i++) {
                    printf("%s\t%s\t%s\t\t%s\t%s\t%s\t%.2f\t%d\n",
                        sessions[i].hallType, sessions[i].sessionId,
                        sessions[i].movieName, sessions[i].language, sessions[i].startTime,
                        sessions[i].endTime, sessions[i].price, sessions[i].remainingSeats);
                    found = 1; // 找到至少一场电影
                }
                if (!found) {
                    printf("没有找到任何电影场次。\n");
                }
                printf("\n");
                break;
            }

            case 2: { // 通过电影名称查询电影
                int found = 0; // 标志是否找到电影
                printf("请输入电影名称: ");
                scanf("%99s", searchName);
                searchName[strcspn(searchName, "\n")] = 0; // 去除换行符

                printf("类型\t场次\t电影\t\t语言\t开始\t结束\t票价\t余票\n");
                for (int i = 0; i < sessionCount; i++) {
                    if (strlen(searchName) == 0 || strstr(sessions[i].movieName, searchName) != NULL) {
                        printf("%s\t%s\t%s\t\t%s\t%s\t%s\t%.2f\t%d\n",
                            sessions[i].hallType, sessions[i].sessionId,
                            sessions[i].movieName, sessions[i].language, sessions[i].startTime,
                            sessions[i].endTime, sessions[i].price, sessions[i].remainingSeats);
                        found = 1; // 找到符合条件的电影
                    }
                }
                if (!found) {
                    printf("没有找到符合条件的电影。\n");
                }
                printf("\n");
                break;
            }

            case 3: { // 通过放映语言查询电影
                int found = 0; // 标志是否找到电影
                printf("请输入放映语言（英语/中文/韩语）: ");
                scanf("%9s", searchLanguage);
                searchLanguage[strcspn(searchLanguage, "\n")] = 0;

                printf("类型\t场次\t电影\t\t语言\t开始\t结束\t票价\t余票\n");
                for (int i = 0; i < sessionCount; i++) {
                    if (strlen(searchLanguage) == 0 || strcmp(sessions[i].language, searchLanguage) == 0) {
                        printf("%s\t%s\t%s\t\t%s\t%s\t%s\t%.2f\t%d\n",
                            sessions[i].hallType, sessions[i].sessionId,
                            sessions[i].movieName, sessions[i].language, sessions[i].startTime,
                            sessions[i].endTime, sessions[i].price, sessions[i].remainingSeats);
                        found = 1; // 找到符合条件的电影
                    }
                }
                if (!found) {
                    printf("没有找到符合条件的电影。\n");
                }
                printf("\n");
                break;
            }

            case 4: { // 查询有剩余座位的电影
                int found = 0; // 标志是否找到电影
                printf("类型\t场次\t电影\t\t语言\t开始\t结束\t票价\t余票\n");
                for (int i = 0; i < sessionCount; i++) {
                    if (sessions[i].remainingSeats > 0) {
                        printf("%s\t%s\t%s\t\t%s\t%s\t%s\t%.2f\t%d\n",
                            sessions[i].hallType, sessions[i].sessionId,
                            sessions[i].movieName, sessions[i].language, sessions[i].startTime,
                            sessions[i].endTime, sessions[i].price, sessions[i].remainingSeats);
                        found = 1; // 找到有剩余座位的电影
                    }
                }
                if (!found) {
                    printf("没有找到有剩余座位的电影。\n");
                }
                printf("\n");
                break;
            }

            case 0:
                printf("返回上一菜单。\n");
                printf("\n");
                break;

            default:
                printf("无效的选项，请重新选择！\n");
                printf("\n");
                break;
        }

    } while (choice != 0);
}

// 检查时间冲突，返回 1 表示有冲突，0 表示无冲突
//时间相邻时也冲突 
int isTimeConflict(const char *startTime1, const char *endTime1, const char *startTime2, const char *endTime2) {
    // 检查两个时间段是否重叠
    int start1 = atoi(startTime1);
    int end1 = atoi(endTime1);
    int start2 = atoi(startTime2);
    int end2 = atoi(endTime2);
    return (start1 < end2 && end1 > start2);  // 时间重叠则返回1
}

// 检查座位冲突：返回 1 表示冲突，0 表示没有冲突
int checkSeatConflict(MovieSession *session, char seatNumber[5]) {
    for (int i = 0; i < session->totalSeats; i++) {
        if (strcmp(session->seats[i], seatNumber) == 0) {
            return 1;  // 座位已被占用
        }
    }
    return 0;  // 座位未被占用
}

// 2. 预定电影票功能
void purchaseTicket(int userIndex) {
    char sessionId[20];
    printf("\n请输入场次编号：");
    scanf("%s", sessionId);

    // 查找电影场次
    MovieSession *session = NULL;
    for (int i = 0; i < sessionCount; i++) {
        if (strcmp(sessions[i].sessionId, sessionId) == 0) {
            session = &sessions[i];
            break;
        }
    }
    if (session == NULL) {
        printf("场次不存在！\n");
        return;
    }

    // 检查时间冲突和重复购买
    for (int i = 0; i < orderCount; i++) {
        if (strcmp(orders[i].userId, users[userIndex].userId) == 0 && orders[i].status == 0) {
            if (strcmp(orders[i].sessionId, sessionId) == 0) {
                printf("您已经购买了同一影片的票。\n");
                return;
            }
            // 检查时间冲突
            if (isTimeConflict(session->startTime, session->endTime, orders[i].startTime, orders[i].endTime)) {
                printf("您购买的票时间冲突。\n");
                return;
            }
        }
    }

    // 输入座位号
    char seatNumbers[3][5];  // 记录最多三个座位
    int seatCount = 0;
    printf("请输入您要购买的座位号（格式为 0304，表示第三排第四个座位），输入 0 停止预定：\n");

    // 临时变量来记录座位冲突
    int isSeatConflict = 0;

    while (seatCount < 3) {
        scanf("%s", seatNumbers[seatCount]);

        if (strcmp(seatNumbers[seatCount], "0") == 0) {
            // 用户输入0则停止预定
            break;
        }

        // 检查是否已预定
        if (checkSeatConflict(session, seatNumbers[seatCount])) {
            printf("座位 %s 已被预定，请选择其他座位。\n", seatNumbers[seatCount]);
            isSeatConflict = 1;  // 标记有座位冲突
            break;  // 发现冲突后立即退出循环，不再让用户输入其他座位
        }

        // 检查重复输入的座位号
        for (int j = 0; j < seatCount; j++) {
            if (strcmp(seatNumbers[seatCount], seatNumbers[j]) == 0) {
                printf("座位 %s 您已经选择过了，请选择其他座位。\n", seatNumbers[seatCount]);
                isSeatConflict = 1;  // 标记有座位冲突
                break;  // 发现冲突后立即退出循环
            }
        }

        if (isSeatConflict) {
            break;  // 如果有座位冲突，退出输入循环
        }

        seatCount++;
        if (seatCount < 3) {
            printf("请输入下一个座位号: ");
        }
    }

    // 如果发生座位冲突，直接返回，避免计算票数
    if (isSeatConflict) {
        printf("购票过程已终止，请重新选择。\n");
        return;
    }

    // 检查是否超过影厅座位数
    if (seatCount > session->remainingSeats) {
        printf("该场次剩余座位不足，最多可购买 %d 个座位。\n", session->remainingSeats);
        return;
    }

    // 创建订单
    Order newOrder;
    strcpy(newOrder.userId, users[userIndex].userId);
    strcpy(newOrder.sessionId, sessionId);
    newOrder.seatCount = seatCount;
    newOrder.total_price = seatCount * session->price;
    newOrder.status = 0;  // 未付款

    // 保存场次时间
    strcpy(newOrder.startTime, session->startTime);
    strcpy(newOrder.endTime, session->endTime);

    // 将用户选择的座位保存到订单中
    for (int i = 0; i < seatCount; i++) {
        strcpy(newOrder.seatNumbers[i], seatNumbers[i]);
    }

    // 添加订单到订单数组
    orders[orderCount++] = newOrder;

    // 更新场次的剩余座位数
    session->remainingSeats -= seatCount;

    printf("预定成功！您已预定 %d 个座位，总金额：%.2f 元。\n", seatCount, newOrder.total_price);
}


// 查找电影场次的索引
// 将 findSessionIndex 提前声明或者定义，这样 viewOrders 函数在使用时可以找到该函数
int findSessionIndex(const char *sessionId) {
    for (int i = 0; i < sessionCount; i++) {
        if (strcmp(sessions[i].sessionId, sessionId) == 0) {
            return i;
        }
    }
    return -1;  // 未找到
}

// 取消订单的函数
void cancelOrder(int orderIndex) {
    if (orderIndex >= 0 && orderIndex < orderCount) {
        // 标记订单为取消状态
        orders[orderIndex].status = -1;  // -1 表示订单已取消
        printf("订单已取消。\n");

        // 更新剩余座位数
        MovieSession *session = NULL;
        for (int i = 0; i < sessionCount; i++) {
            if (strcmp(sessions[i].sessionId, orders[orderIndex].sessionId) == 0) {
                session = &sessions[i];
                break;
            }
        }
        if (session) {
            session->remainingSeats += orders[orderIndex].seatCount;  // 恢复剩余座位
        }
    } else {
        printf("无效的订单索引。\n");
    }
}


// 3.查询订单
void viewOrders(int userIndex) {
    int found = 0;
    int choice;
    
    printf("\n");
    printf("请选择查询选项：\n");
    printf("1. 查看已付款订单\n");
    printf("2. 查看未付款订单\n");
    printf("3. 返回上一级\n");
    printf("请输入选项(1-3): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:  // 查看已付款订单
            found = 0;
            printf("\n");
            printf("您的已付订单：\n");
            for (int i = 0; i < orderCount; i++) {
                if (strcmp(orders[i].userId, users[userIndex].userId) == 0 && orders[i].status == 1) {
                    found = 1;
                    printf("订单号：%02d\n", i + 1); // 按01, 02, 03递增显示订单号
                    printf("场次编号：%s\n", orders[i].sessionId);
                    printf("电影名称：%s\n", sessions[findSessionIndex(orders[i].sessionId)].movieName);  // 获取电影名称
                    printf("座位数：%d\n", orders[i].seatCount);
                    printf("座位号：");
                    for (int j = 0; j < orders[i].seatCount; j++) {
                        printf("%s ", orders[i].seatNumbers[j]);
                    }
                    printf("\n");
                    printf("总价：%.2f 元\n", orders[i].total_price);
                    printf("订单状态：已付款\n\n");
                }
            }
            if (!found) {
                printf("您没有已付款的订单。\n");
            }
            break;

        case 2:  // 查看未付款订单
            found = 0;
            printf("\n");
            printf("您的未付款订单：\n");
            for (int i = 0; i < orderCount; i++) {
                if (strcmp(orders[i].userId, users[userIndex].userId) == 0 && orders[i].status == 0) {
                    found = 1;
                    printf("订单号：%02d\n", i + 1);  // 按01, 02, 03递增显示订单号
                    printf("场次编号：%s\n", orders[i].sessionId);
                    printf("电影名称：%s\n", sessions[findSessionIndex(orders[i].sessionId)].movieName);  // 获取电影名称
                    printf("座位数：%d\n", orders[i].seatCount);
                    printf("座位号：");
                    for (int j = 0; j < orders[i].seatCount; j++) {
                        printf("%s ", orders[i].seatNumbers[j]);
                    }
                    printf("\n");
                    printf("总价：%.2f 元\n", orders[i].total_price);
                    printf("订单状态：未付款\n");

                    // 提供操作：支付或取消
                    printf("\n");
                    printf("请选择操作：\n");
                    printf("1. 支付订单\n");
                    printf("2. 取消订单\n");
                    printf("3. 返回\n");
                    printf("请输入选项(1-3): ");
                    int action;
                    scanf("%d", &action);
                    switch (action) {
                        case 1:
                            // 支付订单
                            orders[i].status = 1;  // 标记为已付款
                            printf("订单支付成功！祝您观影愉快~\n");
                            break;
                        case 2:
                            // 取消订单
                            cancelOrder(i); // 取消订单
                            break;
                        default:
                            break;
                    }
                    printf("\n");
                }
            }
            if (!found) {
                printf("您没有未付款的订单。\n");
            }
            break;

        case 3:
            // 返回上一级
            return;

        default:
            printf("无效选项，返回上一级。\n");
            break;
    }
}

// 4.修改用户信息
void modifyUserInfo(int userIndex) {
    char newPhone[20], newPassword[20], newUserId[20];
    printf("请输入新的用户ID：");
    scanf("%19s", newUserId);
    printf("请输入新的手机号：");
    scanf("%19s", newPhone);  
    printf("请输入新的密码：");
    scanf("%19s", newPassword);

    strcpy(users[userIndex].userId, newUserId);
    strcpy(users[userIndex].phone, newPhone);
    strcpy(users[userIndex].password, newPassword);

    printf("个人信息修改成功。\n");
}




// 1.查看所有影厅信息
void viewCinemaHalls() {
    if (hallCount == 0) {
        printf("当前没有影厅信息。\n");
        return;
    }

    printf("影厅编号 类型 总座位数\n");
    for (int i = 0; i < hallCount; i++) {
        printf("%s\t%s\t%d\n", halls[i].hallId, halls[i].hallType, halls[i].totalSeats);
    }
}

// 2.添加影厅
void addCinemaHall() {
    if (hallCount >= MAX_HALLS) {
        printf("影厅数量已达上限。\n");
        return;
    }

    CinemaHall hall;
    printf("请输入影厅编号：");
    scanf("%19s", hall.hallId);

    // 检查影厅编号是否重复
    for (int i = 0; i < hallCount; i++) {
        if (strcmp(halls[i].hallId, hall.hallId) == 0) {
            printf("该影厅编号已存在！\n");
            return;
        }
    }

    printf("请输入影厅类型（2D/3D/IMAX）：");
    scanf("%9s", hall.hallType);

    // 限制影厅类型输入
    if (strcmp(hall.hallType, "2D") != 0 && strcmp(hall.hallType, "3D") != 0 && strcmp(hall.hallType, "IMAX") != 0) {
        printf("无效的影厅类型！\n");
        return;
    }

    printf("请输入影厅总座位数：");
    scanf("%d", &hall.totalSeats);

    halls[hallCount] = hall;
    hallCount++;
    printf("影厅添加成功！\n");
}

// 3.修改影厅信息
void modifyCinemaHall() {
    char hallId[20];
    printf("请输入要修改的影厅编号：");
    scanf("%s", hallId);

    int found = 0;
    for (int i = 0; i < hallCount; i++) {
        if (strcmp(halls[i].hallId, hallId) == 0) {
            found = 1;
            printf("找到影厅：%s\n", halls[i].hallId);

            // 修改影厅类型
            printf("请输入新的影厅类型（2D/3D/IMAX）：");
            scanf("%s", halls[i].hallType);

            // 验证影厅类型合法性
            if (strcmp(halls[i].hallType, "2D") != 0 && strcmp(halls[i].hallType, "3D") != 0 && strcmp(halls[i].hallType, "IMAX") != 0) {
                printf("无效的影厅类型！\n");
                return;
            }

            // 修改总座位数
            printf("请输入新的总座位数：");
            scanf("%d", &halls[i].totalSeats);
            if (halls[i].totalSeats <= 0) {
                printf("总座位数无效！\n");
                return;
            }

            printf("影厅信息已修改！\n");
            break;
        }
    }
    if (!found) {
        printf("未找到该影厅！\n");
    }
}

// 4.查看所有电影场次
void viewMovieSessions() {
    if (sessionCount == 0) {
        printf("当前没有电影场次。\n");
        return;
    }
    
    printf("影院\t\t场次\t电影\t\t类型\t语言\t开始\t结束\t票价\t余票\n");
    for (int i = 0; i < sessionCount; i++) {
        printf("%s\t%s\t%s\t\t%s\t%s\t%s\t%s\t%.2f\t%d\n",
               sessions[i].cinemaName,
               sessions[i].sessionId,
               sessions[i].movieName,
               sessions[i].hallType,
               sessions[i].language,
               sessions[i].startTime,
               sessions[i].endTime,
               sessions[i].price,
               sessions[i].remainingSeats);
    }
}

// 5.添加电影场次功能
void addMovieSession() {
    if (sessionCount >= MAX_SESSIONS) {
        printf("场次数量已达上限。\n");
        return;
    }

    MovieSession session;
    printf("请输入场次编号：");
    scanf("%19s", session.sessionId);
    printf("请输入影厅编号：");
    scanf("%19s", session.hallId);
    
    // 检查影厅是否存在
    int hallExists = 0;
    int hallIndex = -1;  // 用于记录影厅的索引
    for (int i = 0; i < hallCount; i++) {
        if (strcmp(halls[i].hallId, session.hallId) == 0) {
            hallExists = 1;
            hallIndex = i; // 找到影厅时记录索引
            break;
        }
    }
    if (!hallExists) {
        printf("该影厅编号不存在！\n");
        return;
    }
    
    // 获取影厅的座位数
    int totalSeats = halls[hallIndex].totalSeats;

    printf("请输入电影名字：");
    scanf("%99s", session.movieName);
    printf("请输入电影类型：");
    scanf("%5s", session.hallType);
    printf("请输入电影开始时间（格式HHMM）：");
    scanf("%5s", session.startTime);
    printf("请输入电影结束时间（格式HHMM）：");
    scanf("%5s", session.endTime);
    printf("请输入语言类型（英语/中文）：");
    scanf("%9s", session.language);
    printf("请输入票价：");
    scanf("%f", &session.price);
    
    session.cinemaName[0] = '\0'; // 初始化为空字符串
    strcpy(session.cinemaName, "West Tomato"); // 设置固定的电影院名称
    
    // 初始化余票数
    session.remainingSeats = totalSeats; // 设置余票数与影厅总座位数一致

    sessions[sessionCount] = session;
    sessionCount++;
    printf("电影场次添加成功！\n");
}

// 6.修改电影场次信息
void modifyMovieSession(int adminIndex) {
    char sessionId[20];
    printf("请输入要修改的场次编号：");
    scanf("%s", sessionId);
    
    //检查电影场次 
    int found = 0;
    for (int i = 0; i < sessionCount; i++) {
        if (strcmp(sessions[i].sessionId, sessionId) == 0) {
            found = 1;
            printf("找到场次：%s %s\n", sessions[i].sessionId,sessions[i].movieName);

            // 修改影厅类型
            printf("请输入新的影厅类型（2D/3D/IMAX）：");
            scanf("%s", sessions[i].hallType);

            // 验证影厅类型合法性
            if (strcmp(sessions[i].hallType, "2D") != 0 && strcmp(sessions[i].hallType, "3D") != 0 && strcmp(sessions[i].hallType, "IMAX") != 0) {
                printf("无效的影厅类型！\n");
                return;
            }

            // 修改票价
            printf("请输入新的票价：");
            scanf("%f", &sessions[i].price);
            if (sessions[i].price < 0) {
                printf("票价不能为负数！\n");
                return;
            }

            // 修改语言类型
            printf("请输入新的语言类型（英语/中文）：");
            scanf("%s", sessions[i].language);
            if (strcmp(sessions[i].language, "英语") != 0 && strcmp(sessions[i].language, "中文") != 0) {
                printf("无效的语言类型！\n");
                return;
            }

            // 修改剩余座位数
            printf("请输入新的剩余座位数：");
            scanf("%d", &sessions[i].remainingSeats);
            if (sessions[i].remainingSeats < 0 || sessions[i].remainingSeats > halls[0].totalSeats) {
                printf("剩余座位数无效！请确保剩余座位数在 0 到 %d 之间。\n", halls[0].totalSeats);
                return;
            }

            printf("电影场次信息已修改！\n");
            break;
        }
    }
    if (!found) {
        printf("未找到该场次！\n");
    }
}

// 7.查看收入统计
void viewRevenueStatistics(int adminIndex) {
    float totalRevenue = 0;
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].status == 1) { // 仅统计已付款订单
            totalRevenue += orders[i].total_price;
        }
    }
    printf("总收入：%.2f元\n", totalRevenue);
}

// 8.查看已预订座位
void viewBookedSeats(int adminIndex) {
    // 确保管理员存在且有效
    if (adminIndex < 0 || adminIndex >= adminCount) {
        printf("无效的管理员索引。\n");
        return;
    }

    for (int i = 0; i < sessionCount; i++) {
        int bookedSeats = 0;

        // 遍历所有订单，计算已支付的座位数
        for (int j = 0; j < orderCount; j++) {
            if (strcmp(orders[j].sessionId, sessions[i].sessionId) == 0 && orders[j].status == 1) {
                bookedSeats += orders[j].seatCount; // 累加已支付的座位数
            }
        }

        // 输出场次信息，只显示已预订的座位数
        printf("场次：%s, 影厅：%s, 已预订座位：%d\n",
               sessions[i].sessionId,
               sessions[i].hallType,
               bookedSeats);
    }
}


// 管理员功能菜单
void adminMenu(int adminIndex, Admin admins[], int adminCount) {
    int choice;

    // 确保管理员索引有效
    if (adminIndex < 0 || adminIndex >= adminCount) {
        printf("无效的管理员索引，无法访问管理员菜单。\n");
        return;
    }

    do {
        printf("\n-- 管理员菜单 --\n");
        printf("1. 查看影厅\n");
        printf("2. 添加影厅\n");
        printf("3. 修改影厅\n");
        printf("4. 查看电影场次\n");
        printf("5. 添加电影场次\n");
        printf("6. 修改电影场次\n");
        printf("7. 查看收入统计\n");
        printf("8. 查看已预定座位\n");
        printf("0. 退出，返回主菜单\n");
        printf("请选择操作: ");
        scanf("%d", &choice);

        switch (choice) {
        	case 1: viewCinemaHalls(); break;
            case 2: addCinemaHall(); break;
            case 3: modifyCinemaHall(); break;
            case 4: viewMovieSessions(); break; // 确保没有传递多余的参数
            case 5: addMovieSession(); break;
            case 6: modifyMovieSession(adminIndex); break;
            case 7: viewRevenueStatistics(adminIndex); break;  // 确保传递管理员索引
            case 8: viewBookedSeats(adminIndex); break;  // 确保传递管理员索引
            case 0: printf("退出管理员菜单。\n"); break;
            default: printf("无效的选择，请重新输入。\n"); break;
        }
    } while (choice != 0);
}

// 用户功能菜单
void userMenu(int userIndex) {
    // 确保用户索引有效
    if (userIndex < 0 || userIndex >= userCount) {
        printf("无效的用户索引。\n");
        return;
    }

    int choice;
    do {
        printf("\n-- 用户菜单 --\n");
        printf("1. 查看电影场次\n");
        printf("2. 预定电影票\n");
        printf("3. 查看订单\n");
        printf("4. 修改用户信息\n");
        printf("0. 退出，返回主菜单\n");
        printf("请选择操作: ");
        if (scanf("%d", &choice) != 1) {
            printf("输入无效，请输入数字。\n");
            while (getchar() != '\n'); // 清空输入缓冲区
            continue;
        }

        switch (choice) {
            case 1: querySessions(userIndex); break;
            case 2: purchaseTicket(userIndex); break;
            case 3: viewOrders(userIndex); break;
            case 4: modifyUserInfo(userIndex); break;
            case 0: printf("退出用户菜单。\n"); break;
            default: printf("无效的选择，请重新输入。\n"); break;
        }
    } while (choice != 0);
}

// 主菜单（用户与管理员共用）
void mainMenu() {
    int choice;

    do {
        printf("\n-- 主菜单 --\n");
        printf("1. 用户登录\n");
        printf("2. 用户注册\n");
        printf("3. 管理员登录\n");
        printf("4. 退出\n");
        printf("请选择操作: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("输入无效，请输入数字。\n");
            while (getchar() != '\n'); // 清空输入缓冲区
            continue;
        }

        switch (choice) {
            case 1:
                login(); // 调用登录函数
                break;
            case 2:
                userRegister(); // 调用注册函数
                break;
            case 3: {
                char adminId[20];
                char password[20];
                printf("请输入管理员ID：");
                if (scanf("%19s", adminId) != 1) {
                    printf("输入无效。\n");
                    continue;
                }
                printf("请输入密码：");
                if (scanf("%19s", password) != 1) {
                    printf("输入无效。\n");
                    continue;
                }

                int adminIndex = loginAdmin(admins, adminCount, adminId, password);
                if (adminIndex >= 0) {
                    adminMenu(adminIndex, admins, adminCount);  // 调用 adminMenu 并传递有效的 adminIndex
                } else {
                    printf("登录失败！\n");
                }
                break;
            }
            case 4:
                printf("退出系统。\n");
                return; // 退出主菜单
            default:
                printf("无效的选择，请重新输入。\n");
                break;
        }
    } while (choice != 4);
}

// 初始化管理员数据
void initAdmins() {
    Admin admin;
    strcpy(admin.adminId, "晴天祝");
    strcpy(admin.cinemaName, "West Tomato");
    strcpy(admin.password, "cxy");
    admins[adminCount++] = admin;
}

int main() {
	// 初始化影院信息
    initCinema();
    
    // 初始化管理员数据
    initAdmins();

    // 启动登录流程
    mainMenu();
    return 0;
}
