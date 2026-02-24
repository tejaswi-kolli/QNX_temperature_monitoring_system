#ifndef IPC_PROTOCOL_H
#define IPC_PROTOCOL_H

/* ================= IPC ATTACH POINT ================= */

#define LOGGER_ATTACH_POINT "logger_server"

/* ================= CONSTANTS ================= */

#define MAX_LOGS 100

/* ================= MESSAGE TYPES ================= */

typedef enum {
    MSG_SENSOR_DATA = 1,
    MSG_GET_LAST,
    MSG_GET_ALL
} msg_type_t;

/* ================= MESSAGE STRUCTURES ================= */

/* Sensor → Logger */
typedef struct {
    msg_type_t type;
    int temperature;
} sensor_msg_t;

/* CLI → Logger */
typedef struct {
    msg_type_t type;
} cli_msg_t;

/* Logger → CLI */
typedef struct {
    int count;
    int temps[MAX_LOGS];
} logger_reply_t;

#endif /* IPC_PROTOCOL_H */

