#include "../include/message_queue.h"

/**
 * Creates a message queue
 * 
 * @param key The key to identify the message queue
 * @return The message queue ID on success, -1 on failure
 */
int create_message_queue(key_t key) {
    int msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1) {
        fprintf(stderr, "Failed to create message queue: %s\n", strerror(errno));
        return -1;
    }
    return msqid;
}

/**
 * Gets an existing message queue ID
 * 
 * @param key The key to identify the message queue
 * @return The message queue ID on success, -1 on failure
 */
int get_message_queue(key_t key) {
    int msqid = msgget(key, 0666);
    if (msqid == -1) {
        fprintf(stderr, "Failed to get message queue: %s\n", strerror(errno));
        return -1;
    }
    return msqid;
}

/**
 * Sends a message to a message queue
 * 
 * @param msqid The message queue ID
 * @param msgp Pointer to the message structure (must start with long mtype)
 * @param msgsz The size of the message data (excluding the mtype field)
 * @return 0 on success, -1 on failure
 */
int send_message(int msqid, const void* msgp, size_t msgsz) {
    if (msgsnd(msqid, msgp, msgsz, 0) == -1) {
        fprintf(stderr, "Failed to send message: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

/**
 * Receives a message from a message queue
 * 
 * @param msqid The message queue ID
 * @param msgp Pointer to the buffer to receive the message
 * @param msgsz The maximum size of the message data to receive
 * @param msgtyp The desired message type (0 for first message, >0 for specific type, <0 for type <= |msgtyp|)
 * @param msgflg Flags (e.g., IPC_NOWAIT)
 * @return The number of bytes received (excluding mtype) on success, -1 on failure
 */
int receive_message(int msqid, void* msgp, size_t msgsz, long msgtyp, int msgflg) {
    ssize_t received_size = msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
    if (received_size == -1) {
        // Don't print error if IPC_NOWAIT and no message (EWOULDBLOCK or ENOMSG)
        if (!(msgflg & IPC_NOWAIT && (errno == EWOULDBLOCK || errno == ENOMSG))) {
             fprintf(stderr, "Failed to receive message: %s\n", strerror(errno));
        }
        return -1;
    }
    return (int)received_size;
}

/**
 * Destroys a message queue
 * 
 * @param msqid The message queue ID
 * @return 0 on success, -1 on failure
 */
int destroy_message_queue(int msqid) {
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "Failed to destroy message queue: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

