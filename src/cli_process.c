#include <stdio.h>
#include <stdlib.h>
#include <sys/dispatch.h>

#include "ipc_protocol.h"

int main(void)
{
    int coid, choice, i;
    cli_msg_t msg;
    logger_reply_t reply;

    coid = name_open(LOGGER_ATTACH_POINT, 0);
    if (coid == -1) {
        perror("name_open");
        return EXIT_FAILURE;
    }

    printf("CLI process started...\n");

    while (1) {
        printf("\n1. Get last reading\n");
        printf("2. Get all readings\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            msg.type = MSG_GET_LAST;
        else if (choice == 2)
            msg.type = MSG_GET_ALL;
        else
            continue;

        MsgSend(coid, &msg, sizeof(msg), &reply, sizeof(reply));

        printf("\n---- Readings ----\n");
        for (i = 0; i < reply.count; i++)
            printf("%d C\n", reply.temps[i]);
    }
}

