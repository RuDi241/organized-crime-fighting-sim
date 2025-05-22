#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

// Message Queue Keys (Example values, ensure uniqueness)
#define MSG_KEY_TARGET_GENERATOR 5555
#define MSG_KEY_MEMBER_GENERATOR 6666
#define MSG_KEY_POLICE 7777
#define MSG_KEY_GRAPHICS 8888


// Maximum message size (adjust as needed)
#define MAX_MSG_SIZE 256

// Customer order message
typedef struct {
    long mtype; // Message type (must be first member)
    int order_id; //unique order ID.
    int product_type; // e.g., Bread, Cake, Sandwich
    int product_id;   // Specific type/flavor ID
    int quantity;
    float price;
    int already_seen; // 1 if already seen, 0 otherwise
} OrderMessage;

// Ingredient request message
typedef struct {
    long mtype; // Message type
    int team_id; // ID of the requesting chef team
    int ingredient_type; // e.g., Wheat, Yeast, Cheese
    int quantity;
    int order_id; // Order ID this ingredient is related to
} IngredientRequestMessage;

// Notification message
typedef struct {
    long mtype; // Message type
    int sender_id; // Process ID or Team ID of the sender
    int item_id; // ID of the item related to the notification
    int quantity; // Quantity related to the notification
    int order_id; // Order ID this item belong to
    int already_seen; // 1 if already seen, 0 otherwise
    int order_type;
} NotificationMessage;

// Generic message buffer (can hold any of the above)
typedef struct {
    long mtype;
    char mtext[MAX_MSG_SIZE];
} GenericMessage;

// Function prototypes
int create_message_queue(key_t key);
int get_message_queue(key_t key);
int send_message(int msqid, const void* msgp, size_t msgsz);
int receive_message(int msqid, void* msgp, size_t msgsz, long msgtyp, int msgflg);
int destroy_message_queue(int msqid);

#endif // MESSAGE_QUEUE_H
