/* Disk Scheduling Algorithms: SSTF, SCAN and C-LOOK */
#include <stdio.h>
#include <stdlib.h>

#define MAX 100

void sort(int arr[], int n)
{
    int i, j, temp;
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (arr[i] > arr[j]) {
                temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
            }
}

void printResult(int totalSeek, int n)
{
    printf("\nTotal Tracks Traversed = %d\n", totalSeek);
    printf("Average Seek Length = %.2f\n", (float)totalSeek / n);
}

void SSTF(int req[], int n, int head)
{
    int visited[MAX] = {0}, totalSeek = 0;
    printf("\nSSTF Seek Sequence: %d", head);
    for (int count = 0; count < n; count++) {
        int min = 99999, index = -1;
        for (int i = 0; i < n; i++)
            if (!visited[i]) {
                int dist = abs(head - req[i]);
                if (dist < min) { min = dist; index = i; }
            }
        visited[index] = 1;
        totalSeek += abs(head - req[index]);
        head = req[index];
        printf(" -> %d", head);
    }
    printResult(totalSeek, n);
}

void SCAN(int req[], int n, int head, int diskSize)
{
    int arr[MAX], pos = 0, totalSeek = 0;
    for (int i = 0; i < n; i++) arr[i] = req[i];
    sort(arr, n);
    while (pos < n && arr[pos] < head) pos++;

    printf("\nSCAN Seek Sequence: %d", head);
    /* Move towards higher-numbered tracks first. */
    for (int i = pos; i < n; i++) {
        totalSeek += abs(head - arr[i]);
        head = arr[i];
        printf(" -> %d", head);
    }
    totalSeek += abs(head - (diskSize - 1));
    head = diskSize - 1;
    printf(" -> %d", head);
    for (int i = pos - 1; i >= 0; i--) {
        totalSeek += abs(head - arr[i]);
        head = arr[i];
        printf(" -> %d", head);
    }
    printResult(totalSeek, n);
}

void CLOOK(int req[], int n, int head)
{
    int arr[MAX], pos = 0, totalSeek = 0;
    for (int i = 0; i < n; i++) arr[i] = req[i];
    sort(arr, n);
    while (pos < n && arr[pos] < head) pos++;

    printf("\nC-LOOK Seek Sequence: %d", head);
    for (int i = pos; i < n; i++) {
        totalSeek += abs(head - arr[i]);
        head = arr[i];
        printf(" -> %d", head);
    }
    if (pos > 0) {
        totalSeek += abs(head - arr[0]);
        head = arr[0];
        printf(" -> %d", head);
    }
    for (int i = 1; i < pos; i++) {
        totalSeek += abs(head - arr[i]);
        head = arr[i];
        printf(" -> %d", head);
    }
    printResult(totalSeek, n);
}

int main(void)
{
    int req[MAX], n, head, diskSize, choice;
    printf("Enter Number of Requests: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of requests.\n"); return 1;
    }
    printf("Enter Disk Requests:\n");
    for (int i = 0; i < n; i++) scanf("%d", &req[i]);
    printf("Enter Initial Head Position: ");
    scanf("%d", &head);
    printf("Enter Disk Size: ");
    scanf("%d", &diskSize);

    do {
        printf("\n===== Disk Scheduling Menu =====\n");
        printf("1. SSTF\n2. SCAN\n3. C-LOOK\n4. Exit\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: SSTF(req, n, head); break;
            case 2: SCAN(req, n, head, diskSize); break;
            case 3: CLOOK(req, n, head); break;
            case 4: printf("Exiting...\n"); break;
            default: printf("Invalid Choice!\n");
        }
    } while (choice != 4);
    return 0;
}
