#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#define PORT 8000

typedef struct normal_user
{
    int account_num;
    char username[20];
    char password[20];
    float balance;
    int is_deleted;
} normal_user;

typedef struct joint_user
{
    int account_num;
    char username1[20];
    char username2[20];
    char password[20];
    float balance;
    int is_deleted;
} joint_user;

typedef struct admin
{
    int account_num;
    char username[20];
    char password[20];
} admin;