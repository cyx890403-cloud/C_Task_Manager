#ifndef QUEUE_H // 防呆機制：避免被重複引入 (Include Guard)
#define QUEUE_H

// --- 顏色巨集定義 ---
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

// --- 資料結構定義 ---
typedef struct TaskNode {
    int task_id;
    int priority; 
    char description[100];
    struct TaskNode *next;
} TaskNode;

typedef struct Queue {
    TaskNode *front;
    TaskNode *rear;
} Queue;

// --- 函式介面宣告 (只寫名字，不寫邏輯) ---
void initQueue(Queue *q);
void showProgressBar();
void saveTasksToFile(Queue *q);
void writeServerLog(int task_id, const char *desc, int priority);
void enqueue(Queue *q, int id, const char *desc, int priority, int is_silent);
void loadTasksFromFile(Queue *q, int *id_counter);
void dequeue(Queue *q);
void batchProcessTasks(Queue *q, int count);
void displayQueue(Queue *q);
void freeAllTasks(Queue *q);

#endif