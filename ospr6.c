#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define SIZE 1024

int main()
{
    int fd1, fd2;
    char message[SIZE] = "";
    char result[SIZE] = "";

    mkfifo("fifo1", 0666);
    mkfifo("fifo2", 0666);

    printf("Enter multiple sentences (press Ctrl+D to finish):\n");

    int len = 0;

    while (len < SIZE - 1 &&
           fgets(message + len, SIZE - len, stdin) != NULL)
    {
        len = strlen(message);
    }

    /* Send input to Process 2 through fifo1 */
    fd1 = open("fifo1", O_WRONLY);
    if (fd1 == -1)
    {
        perror("Error opening fifo1");
        return 1;
    }

    if (write(fd1, message, strlen(message) + 1) == -1)
    {
        perror("Error writing to fifo1");
        close(fd1);
        return 1;
    }

    close(fd1);

    /* Receive result from Process 2 through fifo2 */
    fd2 = open("fifo2", O_RDONLY);
    if (fd2 == -1)
    {
        perror("Error opening fifo2");
        return 1;
    }

    int n = read(fd2, result, SIZE - 1);
    if (n == -1)
    {
        perror("Error reading from fifo2");
        close(fd2);
        return 1;
    }

    if (n > 0)
        result[n] = '\0';

    close(fd2);

    printf("\nResult received from Process 2:\n");
    printf("%s", result);

    return 0;
}
