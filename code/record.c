#include "user.h"

int main()
{
    int o = open("normal_user.txt", O_CREAT | O_RDWR, 0777);
    normal_user dummy_normal_user;
    dummy_normal_user.account_num = 100;
    strcpy(dummy_normal_user.username, "NormalUser_100");
    strcpy(dummy_normal_user.password, "Password100");
    dummy_normal_user.balance = 0.0;
    dummy_normal_user.is_deleted = 0;
    write(o, &dummy_normal_user, sizeof(normal_user));
    lseek(o, 0, SEEK_SET);
    normal_user nu;
    read(o, &nu, sizeof(normal_user));
    printf("Created dummy normal user:\n"
            "Account Number: %d\n"
            "Username: %s\n"
            "Password: %s\n"
            "Account Balance: %.2f\n\n"
            , nu.account_num
            , nu.username
            , nu.password
            , nu.balance);
    close(o);

    o = open("joint_user.txt", O_CREAT | O_RDWR, 0777);
    joint_user dummy_joint_user;
    dummy_joint_user.account_num = 200;
    strcpy(dummy_joint_user.username1, "JointUser1_200");
    strcpy(dummy_joint_user.username2, "JointUser2_200");
    strcpy(dummy_joint_user.password, "Password200");
    dummy_joint_user.balance = 0.0;
    dummy_joint_user.is_deleted = 0;
    write(o, &dummy_joint_user, sizeof(joint_user));
    lseek(o, 0, SEEK_SET);
    joint_user ju;
    read(o, &ju, sizeof(joint_user));
    printf("Created dummy joint user:\n"
            "Account Number: %d\n"
            "Username 1: %s\n"
            "Username 2: %s\n"
            "Password: %s\n"
            "Account Balance: %.2f\n\n"
            , ju.account_num
            , ju.username1
            , ju.username2
            , ju.password
            , ju.balance);
    close(o);

    o = open("admin.txt", O_CREAT | O_RDWR, 0777);
    admin first_admin;
    first_admin.account_num = 300;
    strcpy(first_admin.username, "Admin_300");
    strcpy(first_admin.password, "Password300");
    write(o, &first_admin, sizeof(admin));
    lseek(o, 0, SEEK_SET);
    admin a;
    read(o, &a, sizeof(admin));
    printf("Created first (and only) admin:\n"
            "Account Number: %d\n"
            "Username: %s\n"
            "Password: %s\n"
            , a.account_num
            , a.username
            , a.password);
    close(o);
    
    return 0;
}