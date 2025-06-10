#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

// 比较函数用于排序（升序）
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// 1. FCFS（先来先服务）
int fcfs(int current_head, int requests[], int n, int *schedule, int *schedule_len) {
    int total_seek = 0;
    *schedule_len = 0;
    schedule[(*schedule_len)++] = current_head; // 记录初始位置
    for (int i = 0; i < n; i++) {
        total_seek += abs(requests[i] - current_head);
        current_head = requests[i];
        schedule[(*schedule_len)++] = current_head; // 记录每次移动后的位置
    }
    return total_seek;
}

// 2. SSTF（最短寻道时间优先）
int sstf(int current_head, int requests[], int n, int *schedule, int *schedule_len) {
    int total_seek = 0;
    int visited[n];
    memset(visited, 0, sizeof(visited));
    *schedule_len = 0;
    schedule[(*schedule_len)++] = current_head; // 记录初始位置

    for (int i = 0; i < n; i++) {
        int min_dist = __INT_MAX__;
        int min_idx = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && abs(requests[j] - current_head) < min_dist) {
                min_dist = abs(requests[j] - current_head);
                min_idx = j;
            }
        }
        total_seek += min_dist;
        current_head = requests[min_idx];
        schedule[(*schedule_len)++] = current_head; // 记录移动后的位置
        visited[min_idx] = 1;
    }
    return total_seek;
}

// 3. SCAN（扫描算法）
int scan(int current_head, int requests[], int n, int *schedule, int *schedule_len, int disk_size) {
    int sorted_req[n];
    memcpy(sorted_req, requests, n * sizeof(int));
    qsort(sorted_req, n, sizeof(int), compare);

    int pos = 0;
    while (pos < n && sorted_req[pos] < current_head) pos++;

    int total_seek = 0;
    *schedule_len = 0;
    schedule[(*schedule_len)++] = current_head; // 记录初始位置

    // 向右处理右侧请求
    for (int i = pos; i < n; i++) {
        total_seek += abs(sorted_req[i] - current_head);
        current_head = sorted_req[i];
        schedule[(*schedule_len)++] = current_head;
    }
    // 移动到磁盘末端
    int end = disk_size - 1;
    total_seek += abs(end - current_head);
    current_head = end;
    schedule[(*schedule_len)++] = current_head; // 记录末端位置

    // 向左处理左侧请求
    for (int i = pos - 1; i >= 0; i--) {
        total_seek += abs(sorted_req[i] - current_head);
        current_head = sorted_req[i];
        schedule[(*schedule_len)++] = current_head;
    }
    return total_seek;
}

// 4. CSCAN（循环扫描算法）
int cscan(int current_head, int requests[], int n, int *schedule, int *schedule_len, int disk_size) {
    int sorted_req[n];
    memcpy(sorted_req, requests, n * sizeof(int));
    qsort(sorted_req, n, sizeof(int), compare);

    int pos = 0;
    while (pos < n && sorted_req[pos] < current_head) pos++;

    int total_seek = 0;
    *schedule_len = 0;
    schedule[(*schedule_len)++] = current_head; // 记录初始位置

    // 向右处理右侧请求
    for (int i = pos; i < n; i++) {
        total_seek += abs(sorted_req[i] - current_head);
        current_head = sorted_req[i];
        schedule[(*schedule_len)++] = current_head;
    }
    // 移动到磁盘末端
    int end = disk_size - 1;
    total_seek += abs(end - current_head);
    current_head = end;
    schedule[(*schedule_len)++] = current_head; // 记录末端位置

    // 跳回起点
    int start = 0;
    total_seek += abs(start - end);
    current_head = start;
    schedule[(*schedule_len)++] = current_head; // 记录起点位置

    // 处理左侧请求
    for (int i = 0; i < pos; i++) {
        total_seek += abs(sorted_req[i] - current_head);
        current_head = sorted_req[i];
        schedule[(*schedule_len)++] = current_head;
    }
    return total_seek;
}

// 5. LOOK（单向扫描优化算法）
int look(int current_head, int requests[], int n, int *schedule, int *schedule_len) {
    int sorted_req[n];
    memcpy(sorted_req, requests, n * sizeof(int));
    qsort(sorted_req, n, sizeof(int), compare);

    int pos = 0;
    while (pos < n && sorted_req[pos] < current_head) pos++;

    int total_seek = 0;
    *schedule_len = 0;
    schedule[(*schedule_len)++] = current_head; // 记录初始位置

    // 向右处理右侧请求（到最大请求）
    if (pos < n) {
        for (int i = pos; i < n; i++) {
            total_seek += abs(sorted_req[i] - current_head);
            current_head = sorted_req[i];
            schedule[(*schedule_len)++] = current_head;
        }
    }
    // 向左处理左侧请求（到最小请求）
    if (pos > 0) {
        for (int i = pos - 1; i >= 0; i--) {
            total_seek += abs(sorted_req[i] - current_head);
            current_head = sorted_req[i];
            schedule[(*schedule_len)++] = current_head;
        }
    }
    return total_seek;
}

// 6. CLOOK（循环单向扫描优化算法）
int clook(int current_head, int requests[], int n, int *schedule, int *schedule_len) {
    int sorted_req[n];
    memcpy(sorted_req, requests, n * sizeof(int));
    qsort(sorted_req, n, sizeof(int), compare);

    int pos = 0;
    while (pos < n && sorted_req[pos] < current_head) pos++;

    int total_seek = 0;
    *schedule_len = 0;
    schedule[(*schedule_len)++] = current_head; // 记录初始位置

    // 向右处理右侧请求（到最大请求）
    if (pos < n) {
        for (int i = pos; i < n; i++) {
            total_seek += abs(sorted_req[i] - current_head);
            current_head = sorted_req[i];
            schedule[(*schedule_len)++] = current_head;
        }
    }
    // 跳回最小请求（循环逻辑）
    if (pos > 0 && n > 0) {
        total_seek += abs(sorted_req[0] - current_head);
        current_head = sorted_req[0];
        schedule[(*schedule_len)++] = current_head; // 记录跳回的最小请求
        // 处理剩余左侧请求
        for (int i = 1; i < pos; i++) {
            total_seek += abs(sorted_req[i] - current_head);
            current_head = sorted_req[i];
            schedule[(*schedule_len)++] = current_head;
        }
    }
    return total_seek;
}

// 打印调度顺序
void print_schedule(int *schedule, int len) {
    printf("调度顺序：");
    for (int i = 0; i < len; i++) {
        printf("%d", schedule[i]);
        if (i < len - 1) printf(" → ");
    }
    printf("\n");
}

// 验证输入的磁盘号是否有效
int validate_track(int track, int disk_size) {
    return track >= 0 && track < disk_size;
}

int main() {
    int n, current_head, disk_size, choice, seed;
    int *requests;
    int schedule[100]; // 存储调度顺序，足够大即可
    int schedule_len;

    // 设置随机数种子
    srand(time(NULL)); // 基于当前时间生成随机种子

    // 选择输入方式
    printf("请选择请求序列输入方式：\n");
    printf("1. 手动输入\n");
    printf("2. 随机生成\n");
    printf("选择：");
    if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        printf("输入无效！程序退出\n");
        return 1;
    }

    // 输入磁盘最大空间
    printf("请输入磁盘最大空间（磁道从0开始）：");
    if (scanf("%d", &disk_size) != 1 || disk_size <= 0) {
        printf("输入无效！磁盘最大空间必须为正整数\n");
        return 1;
    }

    // 输入请求数量
    printf("请输入磁盘请求数量：");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("输入无效！请求数量必须为正整数\n");
        return 1;
    }

    // 动态分配请求数组
    requests = (int *)malloc(n * sizeof(int));
    if (requests == NULL) {
        printf("内存分配失败\n");
        return 1;
    }

    // 手动输入或随机生成请求序列
    if (choice == 1) {
        // 手动输入初始磁头位置
        printf("请输入初始磁头位置（0-%d）：", disk_size - 1);
        if (scanf("%d", &current_head) != 1 || !validate_track(current_head, disk_size)) {
            printf("输入无效！磁头位置必须在0-%d之间\n", disk_size - 1);
            free(requests);
            return 1;
        }

        // 手动输入磁盘请求序列
        printf("请输入%d个磁盘请求磁道号（0-%d），用空格分隔：\n", n, disk_size - 1);
        for (int i = 0; i < n; i++) {
            if (scanf("%d", &requests[i]) != 1 || !validate_track(requests[i], disk_size)) {
                printf("输入无效！第%d个磁道号必须在0-%d之间\n", i + 1, disk_size - 1);
                free(requests);
                return 1;
            }
        }
    } else {
        // 随机生成初始磁头位置
        current_head = rand() % disk_size;
        printf("随机生成初始磁头位置：%d\n", current_head);

        // 随机生成请求序列
        printf("随机生成%d个磁盘请求磁道号（0-%d）：\n", n, disk_size - 1);
        for (int i = 0; i < n; i++) {
            requests[i] = rand() % disk_size;
            printf("%d ", requests[i]);
        }
        printf("\n");
    }

    printf("\n磁盘调度算法测试（初始磁头位置：%d，磁盘大小：%d）\n", current_head, disk_size);
    printf("请求序列：");
    for (int i = 0; i < n; i++) printf("%d ", requests[i]);
    printf("\n\n");

    // 1. FCFS
    int fcfs_seek = fcfs(current_head, requests, n, schedule, &schedule_len);
    printf("1. FCFS（先来先服务）\n");
    print_schedule(schedule, schedule_len);
    printf("总寻道长度：%d\n\n", fcfs_seek);

    // 2. SSTF
    int sstf_seek = sstf(current_head, requests, n, schedule, &schedule_len);
    printf("2. SSTF（最短寻道时间优先）\n");
    print_schedule(schedule, schedule_len);
    printf("总寻道长度：%d\n\n", sstf_seek);

    // 3. SCAN
    int scan_seek = scan(current_head, requests, n, schedule, &schedule_len, disk_size);
    printf("3. SCAN（扫描算法）\n");
    print_schedule(schedule, schedule_len);
    printf("总寻道长度：%d\n\n", scan_seek);

    // 4. CSCAN
    int cscan_seek = cscan(current_head, requests, n, schedule, &schedule_len, disk_size);
    printf("4. CSCAN（循环扫描算法）\n");
    print_schedule(schedule, schedule_len);
    printf("总寻道长度：%d\n\n", cscan_seek);

    // 5. LOOK
    int look_seek = look(current_head, requests, n, schedule, &schedule_len);
    printf("5. LOOK（单向扫描优化算法）\n");
    print_schedule(schedule, schedule_len);
    printf("总寻道长度：%d\n\n", look_seek);

    // 6. CLOOK
    int clook_seek = clook(current_head, requests, n, schedule, &schedule_len);
    printf("6. CLOOK（循环单向扫描优化算法）\n");
    print_schedule(schedule, schedule_len);
    printf("总寻道长度：%d\n\n", clook_seek);

    // 释放内存
    free(requests);
    return 0;
}
