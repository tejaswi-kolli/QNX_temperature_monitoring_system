#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>

#include "ipc_protocol.h"

int main(void)
{
    name_attach_t *attach;
    int rcvid;

    union {
        sensor_msg_t sensor;
        cli_msg_t cli;
    } msg;

    logger_reply_t reply;
    static int log_buffer[MAX_LOGS];
    static int log_count = 0;

    attach = name_attach(NULL, LOGGER_ATTACH_POINT, 0);
    if (!attach) {
        perror("name_attach");
        return EXIT_FAILURE;
    }

    printf("Logger process started...\n");

    while (1) {
        rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);
        if (rcvid == -1)
            continue;

        if (msg.sensor.type == MSG_SENSOR_DATA) {
            if (log_count < MAX_LOGS)
                log_buffer[log_count++] = msg.sensor.temperature;

            MsgReply(rcvid, EOK, NULL, 0);
        }
        else if (msg.cli.type == MSG_GET_LAST) {
            reply.count = (log_count > 0) ? 1 : 0;
            if (reply.count)
                reply.temps[0] = log_buffer[log_count - 1];

            MsgReply(rcvid, EOK, &reply, sizeof(reply));
        }
        else if (msg.cli.type == MSG_GET_ALL) {
            reply.count = log_count;
            memcpy(reply.temps, log_buffer, log_count * sizeof(int));

            MsgReply(rcvid, EOK, &reply, sizeof(reply));
        }
        else {
            MsgReply(rcvid, EINVAL, NULL, 0);
        }
    }
}

