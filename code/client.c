#include "user.h"

int acc_num;

void deposit(int skt)
{
    bool status;
    int choice = 1;
    float amount;
    printf("Amount: ");
    scanf("%f", &amount);
    if(amount <= 0)
    {
        printf("Invalid amount, please try again\n\n");
        return;
    }

    write(skt, &choice, sizeof(int));
    write(skt, &amount, sizeof(float));
    read(skt, &status, sizeof(bool));

    if(status)
        printf("Deposited successfully\n\n");
    else
        printf("Processing error, please try again\n\n");
}

void withdraw(int skt)
{
    bool status;
    int choice = 2;
    float amount;
    printf("Amount: ");
    scanf("%f", &amount);
    if(amount <= 0)
    {
        printf("Invalid amount, please try again\n\n");
        return;
    }

    write(skt, &choice, sizeof(int));
    write(skt, &amount, sizeof(float));
    read(skt, &status, sizeof(bool));

    if(status)
        printf("Withdrawn successfully\n\n");
    else
        printf("Processing error, please try again\n\n");
}

void check_balance(int skt)
{
    bool status;
    int choice = 3;
    float amount;

    write(skt, &choice, sizeof(int));
    read(skt, &amount, sizeof(float));

    printf("Account Balance: %.2f\n\n", amount);
}

void change_password(int skt)
{
    bool status;
    int choice = 4;
    char new_password[20];
    printf("Enter new password: ");
    scanf("%s", new_password);

    write(skt, &choice, sizeof(int));
    write(skt, new_password, sizeof(new_password));
    read(skt, &status, sizeof(bool));

    if(status)
        printf("Password changed successfully\n\n");
    else
        printf("Could not change password, please try again\n\n");
}

void view_info(int skt, int utype)
{
    int choice = 5;
    write(skt, &choice, sizeof(int));

    if(utype == 1)
    {
        normal_user user;
        int r = read(skt, &user, sizeof(normal_user));
        if(r)
            printf("Account Number: %d\n"
                    "Username: %s\n"
                    "Account Balance: %.2f\n\n"
                    , user.account_num
                    , user.username
                    , user.balance);
        else
            printf("Unable to find account, please try again\n\n");
    }
    else if(utype == 2)
    {
        joint_user user;
        int r = read(skt, &user, sizeof(joint_user));
        if(r)
            printf("Account Number: %d\n"
                    "Username 1: %s\n"
                    "Username 2: %s\n"
                    "Account Balance: %.2f\n\n"
                    , user.account_num
                    , user.username1
                    , user.username2
                    , user.balance);
        else
            printf("Unable to find account, please try again\n\n");
    }
}

void add_account(int skt)
{
    bool status;
    int choice = 1;
    int utype;
    printf("Enter account type: ");
    scanf("%d", &utype);

    write(skt, &choice, sizeof(int));
    write(skt, &utype, sizeof(int));

    if(utype == 1)
    {
        normal_user user;
        printf("Enter username: ");
        scanf("%s", user.username);
        printf("Enter password: ");
        scanf("%s", user.password);
        user.balance = 0;
        write(skt, &user, sizeof(normal_user));
    }
    else if(utype == 2)
    {
        joint_user user;
        printf("Enter username 1: ");
        scanf("%s", user.username1);
        printf("Enter username 2: ");
        scanf("%s", user.username2);
        printf("Enter password: ");
        scanf("%s", user.password);
        user.balance = 0;
        write(skt, &user, sizeof(joint_user));
    }

    read(skt, &status, sizeof(bool));

    if(status)
        printf("Account added successfully\n\n");
    else
        printf("Unable to add account, please try again\n\n");
}

void modify_account(int skt)
{
    bool status;
    int choice = 2;
    int utype, an;
    printf("Enter account type: ");
    scanf("%d", &utype);
    printf("Enter account number: ");
    scanf("%d", &an);

    write(skt, &choice, sizeof(int));
    write(skt, &utype, sizeof(int));

    if (utype == 1)
    {
        normal_user user;
        user.account_num = an;
        printf("Enter new values of the fields:\n");
        printf("Username: ");
        scanf("%s", user.username);
        printf("Account Balance: ");
        scanf("%f", &user.balance);
        write(skt, &user, sizeof(normal_user));
    }
    else if(utype == 2)
    {
        joint_user user;
        user.account_num = an;
        printf("Enter new values of the fields:\n");
        printf("Username 1: ");
        scanf("%s", user.username1);
        printf("Username 2: ");
        scanf("%s", user.username2);
        printf("Account Balance: ");
        scanf("%f", &user.balance);
        write(skt, &user, sizeof(joint_user));
    }

    read(skt, &status, sizeof(bool));

    if(status)
        printf("Modified account details successfully\n\n");
    else
        printf("Unable to modify account details, please try again\n\n");

}

void delete_account(int skt)
{
    bool status;
    int choice = 3;
    int utype, an;
    printf("Enter account type: ");
    scanf("%d", &utype);
    printf("Enter account number: ");
    scanf("%d", &an);

    write(skt, &choice, sizeof(int));
    write(skt, &utype, sizeof(int));
    write(skt, &an, sizeof(int));
    read(skt, &status, sizeof(bool));

    if(status)
        printf("Account deleted successfully\n\n");
    else
        printf("Unable to delete account, please try again\n\n");

}

void search_account(int skt)
{
    bool status;
    int choice = 4;
    int utype, an;
    printf("Enter account type: ");
    scanf("%d", &utype);
    printf("Enter account number: ");
    scanf("%d", &an);

    write(skt, &choice, sizeof(int));
    write(skt, &utype, sizeof(int));
    write(skt, &an, sizeof(int));

    if (utype == 1)
    {
        normal_user user;
        int r = read(skt, &user, sizeof(normal_user));
        if(r)
            printf("Account details:\n"
                    "Account Number: %d\n"
                    "Username: %s\n"
                    "Account Balance: %.2f\n\n"
                    , user.account_num
                    , user.username
                    , user.balance);
        else
            printf("Unable to find account, please try again\n\n");
    }
    else if (utype == 2)
    {
        joint_user user;
        int r = read(skt, &user, sizeof(joint_user));
        if(r)
            printf("Account details:\n"
                    "Account Number: %d\n"
                    "Username 1: %s\n"
                    "Username 2: %s\n"
                    "Account Balance: %.2f\n\n"
                    , user.account_num
                    , user.username1
                    , user.username2
                    , user.balance);
        else
            printf("Unable to find account, please try again\n\n");
    }
}

void account_menu(int skt, int utype)
{
    int choice = 0;
    if(utype == 1 || utype == 2)
    {
        while(choice != 6)
        {
            printf("1. Deposit\n"
                    "2. Withdraw\n"
                    "3. Check Balance\n"
                    "4. Change Password\n"
                    "5. View Account Info\n"
                    "6. Exit\n"
                    "What would you like to do? ");
            scanf("%d", &choice);
            printf("\n\n");

            switch(choice)
            {
                case 1: deposit(skt); break;
                case 2: withdraw(skt); break;
                case 3: check_balance(skt); break;
                case 4: change_password(skt); break;
                case 5: view_info(skt, utype); break;
                case 6: write(skt, &choice, sizeof(int)); exit(0);
                default: printf("Invalid choice, please try again\n\n"); break;
            }
        }
    }
    else if(utype == 3)
    {
        while(choice != 5)
        {
            printf("1. Add Account\n"
                    "2. Modify Account\n"
                    "3. Delete Account\n"
                    "4. Search Account\n"
                    "5. Exit\n"
                    "What would you like to do? ");
            scanf("%d", &choice);
            printf("\n\n");

            switch (choice)
            {
                case 1: add_account(skt); break;
                case 2: modify_account(skt); break;
                case 3: delete_account(skt); break;
                case 4: search_account(skt); break;
                case 5: write(skt, &choice, sizeof(int)); exit(0);
                default: printf("Invalid choice, please try again\n\n"); break;
            }
        }
    }
}

void normal_login(int skt)
{
    bool valid;
    int utype = 1;
    normal_user user;
    printf("Account Number: ");
    scanf("%d", &user.account_num);
    printf("Password: ");
    scanf("%s", user.password);
    acc_num = user.account_num;

    write(skt, &utype, sizeof(int));
    write(skt, &user, sizeof(normal_user));
    read(skt, &valid, sizeof(bool));

    if(valid)
    {
        printf("Logged in successfully\n\n");
        account_menu(skt, utype);
    }
    else
        printf("Invalid account or password, please try again\n\n");
}

void joint_login(int skt)
{
    bool valid;
    int utype = 2;
    joint_user user;
    printf("Account Number: ");
    scanf("%d", &user.account_num);
    printf("Password: ");
    scanf("%s", user.password);
    acc_num = user.account_num;

    write(skt, &utype, sizeof(int));
    write(skt, &user, sizeof(joint_user));
    read(skt, &valid, sizeof(bool));

    if(valid)
    {
        printf("Logged in successfully\n\n");
        account_menu(skt, utype);
    }
    else
        printf("Invalid account or password, please try again\n\n");
}

void admin_login(int skt)
{
    bool valid;
    int utype = 3;
    admin user;
    printf("Account Number: ");
    scanf("%d", &user.account_num);
    printf("Password: ");
    scanf("%s", user.password);
    acc_num = user.account_num;

    write(skt, &utype, sizeof(int));
    write(skt, &user, sizeof(admin));
    read(skt, &valid, sizeof(bool));

    if(valid)
    {
        printf("Logged in successfully\n\n");
        account_menu(skt, utype);
    }
    else
        printf("Invalid account or password, please try again\n\n");
}

void user_menu(int skt)
{
    int choice = 0;
    while(choice != 4)
    {
        printf("1. Normal User\n"
                "2. Joint User\n"
                "3. Admin\n"
                "Select a user type: ");
        scanf("%d", &choice);
        printf("\n\n");

        switch (choice)
        {
            case 1: normal_login(skt); break;
            case 2: joint_login(skt); break;
            case 3: admin_login(skt); break;
            default: printf("Invalid choice, please try again\n\n"); break;
        }
    }
}

int main()
{
    struct sockaddr_in saddr;
    int skt = socket(AF_INET, SOCK_STREAM, 0);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(PORT);
    connect(skt, (struct sockaddr *) &saddr, sizeof(saddr));
    user_menu(skt);
    close(skt);
    return 0;
}