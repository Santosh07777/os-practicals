#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void bubbleSort(int a[], int n)
{
    int i, j, temp;

    for(i = 0; i < n - 1; i++)
    {
        for(j = 0; j < n - i - 1; j++)
        {
            if(a[j] > a[j + 1])
            {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

void selectionSort(int a[], int n)
{
    int i, j, min, temp;

    for(i = 0; i < n - 1; i++)
    {
        min = i;

        for(j = i + 1; j < n; j++)
        {
            if(a[j] < a[min])
                min = j;
        }

        temp = a[i];
        a[i] = a[min];
        a[min] = temp;
    }
}

void display(int a[], int n)
{
    for(int i = 0; i < n; i++)
        printf("%d ", a[i]);

    printf("\n");
}

int main()
{
    int n, i;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr1[n], arr2[n];

    printf("Enter elements:\n");

    for(i = 0; i < n; i++)
    {
        scanf("%d", &arr1[i]);
        arr2[i] = arr1[i];
    }

    pid_t child1, child2;

    /* ---------------- Child 1 ---------------- */
    child1 = fork();

    if(child1 == 0)
    {
        printf("\n--- Child 1 ---\n");
        printf("PID = %d  Parent PID = %d\n",
                getpid(), getppid());

        bubbleSort(arr1, n);

        printf("Bubble Sorted Array:\n");
        display(arr1, n);

        exit(0);
    }

    /* ---------------- Child 2 ---------------- */
    child2 = fork();

    if(child2 == 0)
    {
        printf("\n--- Child 2 (Orphan Demo) ---\n");

        sleep(10);

        printf("PID = %d\n", getpid());
        printf("New Parent PID = %d\n", getppid());

        selectionSort(arr2, n);

        printf("Selection Sorted Array:\n");
        display(arr2, n);

        exit(0);
    }

    /* ---------------- Parent ---------------- */
    printf("\n--- Parent Process ---\n");
    printf("PID = %d\n", getpid());
    printf("Child1 PID = %d\n", child1);
    printf("Child2 PID = %d\n", child2);

    /*
       Zombie Demonstration:
       Child1 exits immediately.
       Parent sleeps before wait().
       During this time Child1 becomes Zombie.
    */

    printf("\nZombie Demonstration...\n");
    sleep(5);

    printf("Use another terminal and execute:\n");
    printf("ps -el | grep Z\n");

    wait(NULL);
    printf("\nChild1 collected using wait()\n");

    /*
       Orphan Demonstration:
       Parent exits before Child2 finishes.
    */

    printf("\nOrphan Demonstration...\n");
    printf("Parent exiting before Child2 completes.\n");

    exit(0);
}
/*
How the Demonstration Works
Zombie State
Child 1 finishes immediately.
Parent sleeps for 5 seconds and does not call wait().
During this interval, Child 1 is in the Zombie state.
Check using:
ps -el | grep Z
You will see a process with state Z.
Removal of Zombie
wait(NULL);
collects Child 1's exit status and removes the zombie entry.


Orphan State
Child 2 sleeps for 10 seconds.
Parent exits before Child 2 finishes.
Child 2 loses its parent and becomes an Orphan process.
The system reassigns it to PID 1 (init/systemd).
getppid()
will display 1 (or the PID of systemd on some Linux systems).

               Parent
              /      \
         Child1      Child2
        (Zombie)    (Orphan)
        
This is a comprehensive OS laboratory program covering fork(), wait(), multiple child processes, zombie state, and orphan state in one implementation.

Terminal 1:

sw@sw-HP-Pro-Tower-280-G9-PCI-Desktop-PC:~$ gcc process-control.c
sw@sw-HP-Pro-Tower-280-G9-PCI-Desktop-PC:~$ ./a.out
Enter number of elements: 5
Enter elements:
23
11
678
5
00

--- Parent Process ---
PID = 5132
Child1 PID = 5155
Child2 PID = 5156

Zombie Demonstration...

--- Child 1 ---
PID = 5155  Parent PID = 5132
Bubble Sorted Array:
0 5 11 23 678 

--- Child 2 (Orphan Demo) ---
Use another terminal and execute:
ps -el | grep Z

Child1 collected using wait()

Orphan Demonstration...
Parent exiting before Child2 completes.
sw@sw-HP-Pro-Tower-280-G9-PCI-Desktop-PC:~$ PID = 5156
New Parent PID = 2087
Selection Sorted Array:
0 5 11 23 678 
^C
sw@sw-HP-Pro-Tower-280-G9-PCI-Desktop-PC:~$


Terminal 2:
sw@sw-HP-Pro-Tower-280-G9-PCI-Desktop-PC:~$ ps -el | grep Z
F S   UID     PID    PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
1 Z  1000    5155    5132  0  80   0 -     0 -      pts/0    00:00:00 a.out
sw@sw-HP-Pro-Tower-280-G9-PCI-Desktop-PC:~$ 

*/
