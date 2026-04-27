#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "queue.h" // 必須引入自己的標頭檔！

void initQueue(Queue *q) {
    q->front = NULL;
    q->rear = NULL;
}

void showProgressBar() {
    printf(COLOR_YELLOW "處理中 [");
    for (int i = 0; i < 15; i++) {
        printf("█");
        fflush(stdout);
        Sleep(40);
    }
    printf("] OK!" COLOR_RESET "\n");
}

void saveTasksToFile(Queue *q) {
    FILE *file = fopen("tasks.txt", "w");
    if (file == NULL) return;
    TaskNode *current = q->front;
    while (current != NULL) {
        fprintf(file, "%d|%d|%s\n", current->task_id, current->priority, current->description);
        current = current->next;
    }
    fclose(file);
}

void writeServerLog(int task_id, const char *desc, int priority) {
    FILE *logFile = fopen("server.log", "a");
    if (logFile == NULL) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timeStr[50];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", t);

    const char *prioStr = (priority == 1) ? "EMERGENCY" : "NORMAL";
    fprintf(logFile, "[%s] [%s] Task ID: %03d | Description: %s\n", timeStr, prioStr, task_id, desc);
    
    fclose(logFile);
}

void enqueue(Queue *q, int id, const char *desc, int priority, int is_silent) {
    TaskNode *newNode = (TaskNode *)malloc(sizeof(TaskNode));
    if (newNode == NULL) return;
    
    newNode->task_id = id;
    newNode->priority = priority;
    strncpy(newNode->description, desc, sizeof(newNode->description) - 1);
    newNode->description[sizeof(newNode->description) - 1] = '\0';
    newNode->next = NULL;

    if (q->front == NULL) {
        q->front = newNode;
        q->rear = newNode;
    } else if (priority > q->front->priority) {
        newNode->next = q->front;
        q->front = newNode;
    } else {
        TaskNode *current = q->front;
        while (current->next != NULL && current->next->priority >= priority) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        if (newNode->next == NULL) q->rear = newNode;
    }

    if (!is_silent) {
        if (priority == 1) printf(COLOR_RED "[緊急插隊] 任務 %d 已優先加入！" COLOR_RESET "\n", id);
        else printf(COLOR_GREEN "[新增成功] 任務 %d 已加入排程。" COLOR_RESET "\n", id);
    }
    saveTasksToFile(q);
}

void loadTasksFromFile(Queue *q, int *id_counter) {
    FILE *file = fopen("tasks.txt", "r");
    if (file == NULL) return; 

    char line[256];
    int loaded_count = 0;
    
    while (fgets(line, sizeof(line), file)) {
        int id, priority;
        char desc[100];
        if (sscanf(line, "%d|%d|%99[^\n]", &id, &priority, desc) == 3) {
            enqueue(q, id, desc, priority, 1);
            loaded_count++;
            if (id >= *id_counter) *id_counter = id + 1;
        }
    }
    fclose(file);
    if (loaded_count > 0) {
        printf(COLOR_GREEN ">>> [系統還原] 已從 tasks.txt 載入 %d 個未完成任務。" COLOR_RESET "\n", loaded_count);
        system("pause"); 
    }
}

void dequeue(Queue *q) {
    if (q->front == NULL) {
        printf(COLOR_RED "[提示] 目前沒有待辦任務。" COLOR_RESET "\n");
        return;
    }
    TaskNode *temp = q->front;
    
    if (temp->priority == 1) printf(COLOR_RED ">>> [緊急處理] 任務 ID: %03d" COLOR_RESET "\n", temp->task_id);
    else printf(COLOR_CYAN ">>> [一般處理] 任務 ID: %03d" COLOR_RESET "\n", temp->task_id);
    
    showProgressBar();
    writeServerLog(temp->task_id, temp->description, temp->priority);
    
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    saveTasksToFile(q);
}

void batchProcessTasks(Queue *q, int count) {
    if (q->front == NULL) {
        printf(COLOR_RED "[提示] 佇列為空，沒有任務可以批次處理。" COLOR_RESET "\n");
        return;
    }

    printf(COLOR_CYAN ">>> 開始自動批次處理 %d 個任務...\n" COLOR_RESET, count);
    int processed = 0;

    for (int i = 0; i < count; i++) {
        if (q->front == NULL) {
            printf(COLOR_YELLOW "\n[提示] 佇列已提早清空，結束批次處理。" COLOR_RESET "\n");
            break;
        }
        TaskNode *temp = q->front;
        printf(" -> 正在消化任務 ID: %03d ... ", temp->task_id);
        
        showProgressBar(); 
        writeServerLog(temp->task_id, temp->description, temp->priority); 
        
        q->front = q->front->next;
        if (q->front == NULL) q->rear = NULL;
        free(temp);
        processed++;
        saveTasksToFile(q);
    }
    printf(COLOR_GREEN "\n>>> 批次處理結束，共完成了 %d 個任務。" COLOR_RESET "\n", processed);
}

void displayQueue(Queue *q) {
    printf(COLOR_CYAN "=================================================\n");
    printf("           📊 伺服器當前任務排程面板           \n");
    printf("=================================================" COLOR_RESET "\n");

    if (q->front == NULL) {
        printf(COLOR_YELLOW "             (目前佇列為空，系統閒置中)             " COLOR_RESET "\n");
    } else {
        TaskNode *current = q->front;
        int pos = 1;
        while (current != NULL) {
            char *prio_str = (current->priority == 1) ? COLOR_RED "[緊急]" COLOR_RESET : COLOR_GREEN "[一般]" COLOR_RESET;
            printf(" [%d] ID: %03d | 狀態: %s | 描述: %s\n", pos++, current->task_id, prio_str, current->description);
            current = current->next;
        }
    }
    printf(COLOR_CYAN "=================================================" COLOR_RESET "\n\n");
}

void freeAllTasks(Queue *q) {
    TaskNode *current = q->front;
    TaskNode *nextTask;
    while (current != NULL) {
        nextTask = current->next;
        free(current);
        current = nextTask;
    }
}