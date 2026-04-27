# 伺服器任務排程模擬系統 (Server Task Scheduler)

## 📌 專題簡介
本專案實作一個具備「優先權插隊」與「自動執行日誌」功能的伺服器任務管理系統。旨在透過 C 語言底層操作，模擬真實伺服器環境中的工作排程。

## 🛠 使用技術 (Core Requirements)
本專案嚴格遵循期末規範，實作以下技術亮點：
- **指標應用**：透過指標維護 Queue 的 `front` 與 `rear` 節點。
- [cite_start]**動態記憶體**：使用 `malloc` 配置任務空間，並在 Dequeue 時精準 `free` 記憶體 。
- **資料結構**：實作 Linked List 基礎的 Queue，並加入優先權排序演算法。
- **檔案 I/O**：
  - `tasks.txt`：自動備份當前未完成任務（覆寫模式）。
  - `server.log`：完整記錄任務執行時間與結果（附加模式）。

## 🏗 系統架構
本專案採用模組化設計（分層架構）：
- `queue.h`：定義資料結構與功能介面。
- `queue.c`：核心邏輯與記憶體管理實作。
- `main.c`：終端機互動介面與系統入口。

## 🚀 執行方式
1. **編譯程式**：
   ```bash
   gcc main.c queue.c -o task_manager
2. **運行系統**：
   ./task_manager.exe