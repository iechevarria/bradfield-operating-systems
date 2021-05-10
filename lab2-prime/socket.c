#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(void)
{
    int sv[2]; /* the pair of socket descriptors */
    int buf[1000]; /* for data exchange between processes */

    if (socketpair(AF_UNIX, SOCK_DGRAM, 0, sv) == -1) {
        perror("socketpair");
        exit(1);
    }

    if (!fork()) {  /* child */
        printf("child - socket descriptor: %d\n", sv[1]);
        recv(sv[1], &buf, sizeof(int), 0);
        printf("child: read %d\n", buf[0]);
        buf[0]++;
        send(sv[1], &buf, sizeof(int), 0);
        printf("child: sent %d\n", buf[0]);

    } else { /* parent */
        printf("parent - socket descriptor: %d\n", sv[0]);
        buf[0] = 71;
        send(sv[0], &buf, sizeof(int), 0);
        printf("parent: sent 71\n");
        recv(sv[0], &buf, sizeof(int), 0);
        printf("parent: read %d\n", buf[0]);
        wait(NULL); /* wait for child to die */
    }

    return 0;
}
