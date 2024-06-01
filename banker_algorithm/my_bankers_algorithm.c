// 银行家算法的多线程程序
#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<Windows.h>

int nResources, // 资源数量 
nProcesses; // 进程数量
int* resources; // 当前可用资源
int** allocated;    //分配矩阵
int** maxRequired;  // 最大需求矩阵
int** need;         // 剩余需求矩阵
int* safeSeq;   // 安全序列
int nProcessRan = 0;  // 运行的进程数

// 获取安全序列，如果存在则返回true，否则返回false
bool getSafeSeq();

// 如果存在安全序列则模拟多线程执行
void processCode(int);

int main(int argc, char* argv) {
    srand(time(NULL));
    printf("进程数量? ");
    scanf("%d", &nProcesses);
    printf("资源数量? ");
    scanf("%d", &nResources);
    // 分配资源数组内存
    resources = (int*)malloc(nResources * sizeof(*resources));
    // 输入当前可用资源
    printf("\n当前可用资源(R1 R2 ...)? ");
    for (int i = 0; i < nResources; ++i)
        scanf("%d", &resources[i]);

    // 分配分配矩阵内存
    allocated = (int**)malloc(nProcesses * sizeof(*allocated));
    for (int i = 0; i < nProcesses; ++i)
        allocated[i] = (int*)malloc(nResources * sizeof(**allocated));

    // 分配最大需求矩阵内存
    maxRequired = (int**)malloc(nProcesses * sizeof(*maxRequired));
    for (int i = 0; i < nProcesses; ++i)
        maxRequired[i] = (int*)malloc(nResources * sizeof(**maxRequired));

    // 输入每个进程已分配资源
    printf("\n");
    for (int i = 0; i < nProcesses; ++i) {
        printf("进程 %d 被分配的资源   (R1 R2 ...)? ", i + 1);
        for (int j = 0; j < nResources; ++j)
            scanf("%d", &allocated[i][j]);
    }
    // 输入每个进程的每个资源的最大需求
    printf("\n");
    for (int i = 0; i < nProcesses; ++i) {
        printf("进程 %d 所需的最大资源 (R1 R2 ...)? ", i + 1);
        for (int j = 0; j < nResources; ++j)
            scanf("%d", &maxRequired[i][j]);
    }
    printf("\n");

    // 分配剩余需求矩阵内存
    need = (int**)malloc(nProcesses * sizeof(*need));
    for (int i = 0; i < nProcesses; ++i)
        need[i] = (int*)malloc(nResources * sizeof(**need));

    // 计算剩余需求矩阵
    for (int i = 0; i < nProcesses; ++i) {
        for (int j = 0; j < nResources; ++j)
            need[i][j] = maxRequired[i][j] - allocated[i][j];
    }
    // 获取安全序列
    safeSeq = (int*)malloc(nProcesses * sizeof(*safeSeq));   // 分配安全序列内存
    for (int i = 0; i < nProcesses; ++i)safeSeq[i] = -1;

    if (!getSafeSeq()) {
        printf("\n不安全状态!这些进程导致系统进入不安全状态。\n\n");
        exit(-1);
    }
    printf("\n安全序列建立：");
    for (int i = 0; i < nProcesses; i++) {
        printf("%-3d", safeSeq[i] + 1);
    }
    printf("\n\n");
    Sleep(1000);

    // ...run thread
    printf("正在执行进程...\n");
    int i = 0;
    while (nProcessRan < nProcesses) {
        processCode(i);
        i = (i + 1) % nProcesses;
    }

    printf("\n所有进程都已完成!\n");

    // 释放
    for (int i = 0; i < nResources; ++i) {
        free(allocated[i]);
        free(maxRequired[i]);
        free(need[i]);
    }
    free(resources);
    free(allocated);
    free(maxRequired);
    free(need);
    free(safeSeq);

    return 0;
}

// 获取安全序列的函数
bool getSafeSeq() {
    int* tempRes = (int*)malloc(nResources * sizeof(int));  // 为临时资源分配内存
    for (int i = 0; i < nResources; i++)tempRes[i] = resources[i];  // 将资源复制到临时资源数组

    bool* finished = (bool*)malloc(nResources * sizeof(bool));  // 为进程完成状态分配内存
    for (int i = 0; i < nProcesses; i++)finished[i] = false;  // 初始化所有进程为未完成状态
    int nfinished = 0;  // 完成的进程数量初始化为0
    while (nfinished < nProcesses) {  // 当还有未完成的进程时循环
        bool safe = false;  // 初始化安全状态为假

        for (int i = 0; i < nProcesses; ++i) {  // 遍历所有进程
            if (!finished[i]) {  // 如果当前进程未完成
                bool possible = true;  // 初始化能否运行状态为真
                for (int j = 0; j < nResources; ++j)  // 遍历所有资源
                    if (need[i][j] > tempRes[j]) {  // 如果需要的资源大于临时资源
                        possible = false;  // 设置为不能运行
                        break;
                    }
                if (possible) {  // 如果当前剩余资源满足当前进程需求
                    for (int j = 0; j < nResources; ++j)  // 更新临时资源
                        tempRes[j] += allocated[i][j];
                    safeSeq[nfinished] = i;  // 添加当前进程到安全序列
                    finished[i] = true;  // 设置当前进程为完成状态
                    ++nfinished;  // 完成的进程数量加1
                    safe = true;  // 找到至少一个安全序列的节点，暂时处于安全
                }
            }
        }

        if (!safe) {  // 如果未找到安全序列
            for (int i = 0; i < nProcesses; i++)safeSeq[i] = -1;  // 将安全序列全部置为-1
            return false;  // 返回失败
        }
    }
    return true;  // 返回成功
}

void processCode(int processIndex) {
    int p = processIndex; // 获取线程标号
    if (p != safeSeq[nProcessRan])
        return;

    printf("\n--> 进程 %d", p + 1);
    printf("\n    已分配 : ");
    for (int i = 0; i < nResources; ++i)
        printf("%-3d", allocated[p][i]);  // 输出资源分配情况
    printf("\n    需求   : ");
    for (int i = 0; i < nResources; ++i)
        printf("%-3d", need[p][i]);  // 输出资源需求情况
    printf("\n    可用资源:");
    for (int i = 0; i < nResources; ++i)
        printf("%-3d", resources[i]);    // 输出当前可用资源
    printf("\n    分配资源!");
    Sleep(1000);
    printf("\n    进程代码正在运行...");
    Sleep(1000);
    printf("\n    进程代码完成...");
    printf("\n    释放资源中...");
    for (int i = 0; i < nResources; ++i)
        resources[i] += allocated[p][i];
    Sleep(1000);
    printf("\n    资源已释放!");
    printf("\n    现在可用:");
    for (int i = 0; i < nResources; ++i)
        printf("%-3d", resources[i]);
    printf("\n");

    Sleep(1000);

    nProcessRan++;
}