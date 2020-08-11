#include "user.h"
#include <pthread.h>

int acc_num;

bool login_normal(int new_skt)
{
    bool valid;
    normal_user user, check_user;
    read(new_skt, &user, sizeof(normal_user));
    acc_num = user.account_num;

    int o = open("normal_user.txt", O_RDONLY);
    lseek(o, (acc_num - 100)*sizeof(normal_user), SEEK_SET);
    read(o, &check_user, sizeof(normal_user));
    if(!(strcmp(user.password, check_user.password)) && !(check_user.is_deleted))
        valid = true;
    else
        valid = false;
    close(o);
    write(new_skt, &valid, sizeof(bool));
    return valid;
}

bool login_joint(int new_skt)
{
    bool valid;
    joint_user user, check_user;
    read(new_skt, &user, sizeof(joint_user));
    acc_num = user.account_num;

    int o = open("joint_user.txt", O_RDONLY);
    lseek(o, (acc_num - 200)*sizeof(joint_user), SEEK_SET);
    read(o, &check_user, sizeof(joint_user));
    if(!(strcmp(user.password, check_user.password)) && !(check_user.is_deleted))
        valid = true;
    else
        valid = false;
    close(o);
    write(new_skt, &valid, sizeof(bool));
    return valid;
}

bool login_admin(int new_skt)
{
    bool valid;
    admin user, check_user;
    read(new_skt, &user, sizeof(admin));
    acc_num = user.account_num;

    int o = open("admin.txt", O_RDONLY);
    lseek(o, (acc_num - 300)*sizeof(admin), SEEK_SET);
    read(o, &check_user, sizeof(admin));
    if(!(strcmp(user.password, check_user.password)))
        valid = true;
    else
        valid = false;
    close(o);
    write(new_skt, &valid, sizeof(bool));
    return valid;
}

void deposit(int new_skt, int utype)
{
    bool status;
    float amount;
    read(new_skt, &amount, sizeof(float));
    if(utype == 1)
    {
        normal_user user;
        int o = open("normal_user.txt", O_RDWR);
        lseek(o, (acc_num - 100)*sizeof(normal_user), SEEK_SET);
        read(o, &user, sizeof(normal_user));
        if(!(user.is_deleted))
        {
            user.balance += amount;
            lseek(o, (-1)*sizeof(normal_user), SEEK_CUR);
            write(o, &user, sizeof(normal_user));
            status = true;
        }
        else
            status = false;
        close(o);
    }
    else if(utype == 2)
    {
        joint_user user;
        int o = open("joint_user.txt", O_RDWR);
        struct flock l;
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = (acc_num - 200)*sizeof(joint_user);
        l.l_len = sizeof(joint_user);
        l.l_pid = getpid();
        fcntl(o, F_SETLKW, &l);
        lseek(o, (acc_num - 200)*sizeof(joint_user), SEEK_SET);
        read(o, &user, sizeof(joint_user));
        if(!(user.is_deleted))
        {
            user.balance += amount;
            lseek(o, (-1)*sizeof(joint_user), SEEK_CUR);
            write(o, &user, sizeof(joint_user));
            // getchar();
            status = true;
        }
        else
            status = false;
        l.l_type = F_UNLCK;
        fcntl(o, F_SETLK, &l);
        close(o);
    }
    else
        status = false;
    write(new_skt, &status, sizeof(bool));
}

void withdraw(int new_skt, int utype)
{
    bool status;
    float amount;
    read(new_skt, &amount, sizeof(float));
    if(utype == 1)
    {
        normal_user user;
        int o = open("normal_user.txt", O_RDWR);
        lseek(o, (acc_num - 100)*sizeof(normal_user), SEEK_SET);
        read(o, &user, sizeof(normal_user));
        if(user.balance >= amount && !(user.is_deleted))
        {
            user.balance -= amount;
            lseek(o, (-1)*sizeof(normal_user), SEEK_CUR);
            write(o, &user, sizeof(normal_user));
            // getchar();
            status = true;
        }
        else
            status = false;
        close(o);
    }
    else if(utype == 2)
    {
        joint_user user;
        int o = open("joint_user.txt", O_RDWR);
        struct flock l;
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = (acc_num - 200)*sizeof(joint_user);
        l.l_len = sizeof(joint_user);
        l.l_pid = getpid();
        fcntl(o, F_SETLKW, &l);
        lseek(o, (acc_num - 200)*sizeof(joint_user), SEEK_SET);
        read(o, &user, sizeof(joint_user));
        if(user.balance >= amount && !(user.is_deleted))
        {
            user.balance -= amount;
            lseek(o, (-1)*sizeof(joint_user), SEEK_CUR);
            write(o, &user, sizeof(joint_user));
            status = true;
        }
        else
            status = false;
        l.l_type = F_UNLCK;
        fcntl(o, F_SETLK, &l);
        close(o);
    }
    else
        status = false;
    write(new_skt, &status, sizeof(bool));
}

void check_balance(int new_skt, int utype)
{
    float balance = -1.0;
    if(utype == 1)
    {
        normal_user user;
        int o = open("normal_user.txt", O_RDWR);
        lseek(o, (acc_num - 100)*sizeof(normal_user), SEEK_SET);
        read(o, &user, sizeof(normal_user));
        balance = user.balance;
        close(o);
    }
    else if(utype == 2)
    {
        joint_user user;
        int o = open("joint_user.txt", O_RDWR);
        struct flock l;
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = (acc_num - 200)*sizeof(joint_user);
        l.l_len = sizeof(joint_user);
        l.l_pid = getpid();
        fcntl(o, F_SETLKW, &l);
        lseek(o, (acc_num - 200)*sizeof(joint_user), SEEK_SET);
        read(o, &user, sizeof(joint_user));
        balance = user.balance;
        l.l_type = F_UNLCK;
        fcntl(o, F_SETLK, &l);
        close(o);
    }
    write(new_skt, &balance, sizeof(float));
}

void change_password(int new_skt, int utype)
{
    bool status;
    char pass[20];
    read(new_skt, pass, sizeof(pass));
    if(utype == 1)
    {
        normal_user user;
        int o = open("normal_user.txt", O_RDWR);
        lseek(o, (acc_num - 100)*sizeof(normal_user), SEEK_SET);
        read(o, &user, sizeof(normal_user));
        if(!(user.is_deleted))
        {
            strcpy(user.password, pass);
            lseek(o, (-1)*sizeof(normal_user), SEEK_CUR);
            write(o, &user, sizeof(normal_user));
            status = true;
        }
        else
            status = false;
        close(o);
    }
    else if(utype == 2)
    {
        joint_user user;
        int o = open("joint_user.txt", O_RDWR);
        struct flock l;
        l.l_type = F_WRLCK;
        l.l_whence = SEEK_SET;
        l.l_start = (acc_num - 200)*sizeof(joint_user);
        l.l_len = sizeof(joint_user);
        l.l_pid = getpid();
        fcntl(o, F_SETLKW, &l);
        lseek(o, (acc_num - 200)*sizeof(joint_user), SEEK_SET);
        read(o, &user, sizeof(joint_user));
        if(!(user.is_deleted))
        {
            strcpy(user.password, pass);
            lseek(o, (-1)*sizeof(joint_user), SEEK_CUR);
            write(o, &user, sizeof(joint_user));
            // getchar();
            status = true;
        }
        else
            status = false;
        l.l_type = F_UNLCK;
        fcntl(o, F_SETLK, &l);
        close(o);
    }
    else
        status = false;
    write(new_skt, &status, sizeof(bool));
}

void view_info(int new_skt, int utype)
{
    if(utype == 1)
    {
        normal_user user;
        int o = open("normal_user.txt", O_RDWR);
        lseek(o, (acc_num - 100)*sizeof(normal_user), SEEK_SET);
        read(o, &user, sizeof(normal_user));
        write(new_skt, &user, sizeof(normal_user));
        close(o);
    }
    else if(utype == 2)
    {
        joint_user user;
        int o = open("joint_user.txt", O_RDWR);
        struct flock l;
        l.l_type = F_RDLCK;
        l.l_whence = SEEK_SET;
        l.l_start = (acc_num - 200)*sizeof(joint_user);
        l.l_len = sizeof(joint_user);
        l.l_pid = getpid();
        fcntl(o, F_SETLKW, &l);
        lseek(o, (acc_num - 200)*sizeof(joint_user), SEEK_SET);
        read(o, &user, sizeof(joint_user));
        write(new_skt, &user, sizeof(joint_user));
        l.l_type = F_UNLCK;
        fcntl(o, F_SETLK, &l);
        close(o);
    }
}

void add_user(int new_skt, int utype)
{
    bool status;
    if(utype == 1)
    {
        normal_user user, temp;
        read(new_skt, &user, sizeof(normal_user));
        int o = open("normal_user.txt", O_RDWR);
        lseek(o, (-1)*sizeof(normal_user), SEEK_END);
        read(o, &temp, sizeof(normal_user));
        user.account_num = temp.account_num + 1;
        user.is_deleted = 0;
        write(o, &user, sizeof(normal_user));
        printf("Your account number is: %d\n", user.account_num);
        status = true;
        close(o);
    }
    else if(utype == 2)
    {
        joint_user user, temp;
        read(new_skt, &user, sizeof(joint_user));
        int o = open("joint_user.txt", O_RDWR);
        lseek(o, (-1)*sizeof(joint_user), SEEK_END);
        read(o, &temp, sizeof(joint_user));
        user.account_num = temp.account_num + 1;
        user.is_deleted = 0;
        write(o, &user, sizeof(joint_user));
        printf("Your account number is: %d\n", user.account_num);
        status = true;
        close(o);
    }
    else
        status = false;
    write(new_skt, &status, sizeof(bool));
}

void modify_user(int new_skt, int utype)
{
    bool status;
    if(utype == 1)
    {
        normal_user user, old_user;
        read(new_skt, &user, sizeof(normal_user));
        int o = open("normal_user.txt", O_RDWR);
        lseek(o, (user.account_num - 100)*sizeof(normal_user), SEEK_SET);
        read(o, &old_user, sizeof(normal_user));
        if(!(old_user.is_deleted))
        {
            lseek(o, (-1)*sizeof(normal_user), SEEK_CUR);
            write(o, &user, sizeof(normal_user));
            status = true;
        }
        else
            status = false;
        close(o);
    }
    else if(utype == 2)
    {
        joint_user user, old_user;
        read(new_skt, &user, sizeof(joint_user));
        int o = open("joint_user.txt", O_RDWR);
        lseek(o, (user.account_num - 200)*sizeof(joint_user), SEEK_SET);
        read(o, &old_user, sizeof(joint_user));
        if(!(old_user.is_deleted))
        {
            lseek(o, (-1)*sizeof(joint_user), SEEK_CUR);
            write(o, &user, sizeof(joint_user));
            status = true;
        }
        else
            status = false;
        close(o);
    }
    else
        status = false;
    write(new_skt, &status, sizeof(bool));
}

void delete_user(int new_skt, int utype)
{
    bool status;
    int an;
    read(new_skt, &an, sizeof(int));
    if(utype == 1)
    {
        normal_user user;
        int o = open("normal_user.txt", O_RDWR);
        lseek(o, (an - 100)*sizeof(normal_user), SEEK_SET);
        read(o, &user, sizeof(normal_user));
        if(!(user.is_deleted))
        {
            user.is_deleted = 1;
            strcpy(user.username, "");
            strcpy(user.password, "");
            user.balance = -1.0;
            lseek(o, (-1)*sizeof(normal_user), SEEK_CUR);
            write(o, &user, sizeof(normal_user));
            status = true;
        }
        else
            status = false;
        close(o);
    }
    else if(utype == 2)
    {
        joint_user user;
        int o = open("joint_user.txt", O_RDWR);
        lseek(o, (an - 200)*sizeof(joint_user), SEEK_SET);
        read(o, &user, sizeof(joint_user));
        if(!(user.is_deleted))
        {
            user.is_deleted = 1;
            strcpy(user.username1, "");
            strcpy(user.username2, "");
            strcpy(user.password, "");
            user.balance = -1.0;
            lseek(o, (-1)*sizeof(joint_user), SEEK_CUR);
            write(o, &user, sizeof(joint_user));
            status = true;
        }
        else
            status = false;
        close(o);
    }
    else
        status = false;
    write(new_skt, &status, sizeof(bool));
}

void search_user(int new_skt, int utype)
{
    int an;
    read(new_skt, &an, sizeof(int));
    if(utype == 1)
    {
        normal_user user;
        int o = open("normal_user.txt", O_RDWR);
        lseek(o, (an - 100)*sizeof(normal_user), SEEK_SET);
        read(o, &user, sizeof(normal_user));
        write(new_skt, &user, sizeof(normal_user));
        close(o);
    }
    else if(utype == 2)
    {
        joint_user user;
        int o = open("joint_user.txt", O_RDWR);
        lseek(o, (an - 200)*sizeof(joint_user), SEEK_SET);
        read(o, &user, sizeof(joint_user));
        write(new_skt, &user, sizeof(joint_user));
        close(o);
    }
}

void driver(int new_skt)
{
    bool valid;
    int utype, choice, utype_admin;
    while(1)
    {
        read(new_skt, &utype, sizeof(int));
        switch(utype)
        {
            case 1: valid = login_normal(new_skt); break;
            case 2: valid = login_joint(new_skt); break;
            case 3: valid = login_admin(new_skt); break;
            default: valid = false; break;
        }
        if(valid)
            break;
    }

    while(1)
    {
        read(new_skt, &choice, sizeof(int));
        if(utype == 1 || utype == 2)
        {
            switch(choice)
            {
                case 1: deposit(new_skt, utype); break;
                case 2: withdraw(new_skt, utype); break;
                case 3: check_balance(new_skt, utype); break;
                case 4: change_password(new_skt, utype); break;
                case 5: view_info(new_skt, utype); break;
                default: break;
            }
            if(choice == 6)
                break;
        }
        else if(utype == 3)
        {
            read(new_skt, &utype_admin, sizeof(int));
            switch(choice)
            {
                case 1: add_user(new_skt, utype_admin); break;
                case 2: modify_user(new_skt, utype_admin); break;
                case 3: delete_user(new_skt, utype_admin); break;
                case 4: search_user(new_skt, utype_admin); break;
                default: break;
            }
            if(choice == 5)
                break;
        }
    }
}

void* connection_handler(void* new_skt)
{
    int skt = *(int *) new_skt;
    driver(skt);
    close(skt);
    printf("Closed connection\n\n");
}

int main()
{
    struct sockaddr_in saddr, caddr;
    int new_skt, len = sizeof(caddr);
    int skt = socket(AF_INET, SOCK_STREAM, 0);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(PORT);
    bind(skt, (struct sockaddr *) &saddr, sizeof(saddr));
    listen(skt, 5);
    printf("Waiting for client...\n");
    while(1)
    {
        new_skt = accept(skt, (struct sockaddr *) &caddr, &len);
        printf("Connected to client\n\n");
        // if(!fork())
        // {
        //     close(skt);
        //     driver(new_skt);
        //     close(new_skt);
        //     printf("Closed connection\n\n");
        //     exit(0);
        // }
        // else
        //     close(new_skt);
        pthread_t thrid;
        pthread_create(&thrid, NULL, connection_handler, (void *) &new_skt);
    }
    pthread_exit(NULL);
    close(skt);
    return 0;
}
