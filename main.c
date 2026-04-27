#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "queue.h" // 引入模組！就能無縫使用裡面的所有功能

int main() {
    system("chcp 65001 > nul"); 
    
    Queue taskQueue;
    initQueue(&taskQueue);
    
    int choice;
    int id_counter = 1; 
    char input_desc[100];
    int input_priority;
    int batch_count;

    loadTasksFromFile(&taskQueue, &id_counter);

    while (1) {
        system("cls"); 
        displayQueue(&taskQueue); 

        printf(COLOR_BOLD "--- 系統控制選項 ---\n" COLOR_RESET);
        printf("1. 新增單筆任務 (Enqueue)\n");
        printf("2. 執行單筆任務 (Dequeue)\n");
        printf("3. 批次自動執行 (Batch Process)\n"); 
        printf("4. 離開系統 (Exit)\n");
        printf("請輸入指令: ");
        
        if (scanf("%d", &choice) != 1) break;
        while(getchar() != '\n'); 
        printf("\n"); 

        switch (choice) {
            case 1:
                printf("請輸入任務描述: ");
                fgets(input_desc, sizeof(input_desc), stdin);
                input_desc[strcspn(input_desc, "\n")] = 0; 
                printf("請選擇優先權 (0: 一般, 1: 緊急): ");
                if (scanf("%d", &input_priority) != 1) input_priority = 0;
                while(getchar() != '\n'); 
                if (input_priority != 1) input_priority = 0;

                enqueue(&taskQueue, id_counter++, input_desc, input_priority, 0);
                system("pause"); 
                break;
            case 2:
                dequeue(&taskQueue);
                system("pause");
                break;
            case 3:
                printf("請輸入要自動處理的任務數量: ");
                if (scanf("%d", &batch_count) != 1 || batch_count <= 0) {
                    printf(COLOR_RED "輸入無效！" COLOR_RESET "\n");
                } else {
                    batchProcessTasks(&taskQueue, batch_count);
                }
                while(getchar() != '\n'); 
                system("pause");
                break;
            case 4:
                freeAllTasks(&taskQueue);
                printf(COLOR_CYAN "系統安全關閉，再見！" COLOR_RESET "\n");
                return 0;
            default:
                printf(COLOR_RED "無效的選擇，請重新輸入。" COLOR_RESET "\n");
                system("pause");
        }
    }
    return 0;
}