#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// T5 可用于搜索的Address字段
struct Address
{
    int id;
    int age;
    int set;
    char *name;
    char *email;
    char *phone;
    char *city;
};

// T2 修改代码，接收参数作为MAX_DATA和MAX_ROWS，将它们储存在Database结构体中，并且将它们写到文件。这样就可以创建任意大小的数据库。
struct Database
{
    int MAX_DATA;
    int MAX_ROWS;
    struct Address *rows;
};

struct Connection
{
    FILE *file;
    struct Database *db;
};

void Database_close(struct Connection *conn);

// T1 die函数接收conn变量作为参数，清理并关闭之
// errno: C标准库出错时的错误编号
void die(const char *message, struct Connection *conn)
{
    if (errno)
    {
        perror(message);
    }
    else
    {
        printf("ERROR: %s\n", message);
    }
    Database_close(conn);
    exit(1);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %d %s %s %s\n", addr->id, addr->name, addr->age, addr->email, addr->phone, addr->city);
}

void Database_load(struct Connection *conn)
{
    assert(conn->db && conn->file);
    if (fread(conn->db, sizeof(struct Database), 1, conn->file) != 1)
    {
        die("Failed to load database.", conn);
    }
    conn->db->rows = (struct Address *)malloc(sizeof(struct Address) * conn->db->MAX_ROWS);
    for (int i = 0; i < conn->db->MAX_ROWS; i++)
    {
        struct Address *row = &conn->db->rows[i];
        if (fread(&row->id, sizeof(int), 1, conn->file) != 1)
            die("Failed to load id.", conn);
        if (fread(&row->set, sizeof(int), 1, conn->file) != 1)
            die("Failed to load set.", conn);
        row->name = malloc(conn->db->MAX_DATA);
        row->email = malloc(conn->db->MAX_DATA);
        if (fread(row->name, conn->db->MAX_DATA, 1, conn->file) != 1)
            die("Faile to load name", conn);
        if (fread(row->email, conn->db->MAX_DATA, 1, conn->file) != 1)
            die("Faile to load email", conn);
    }
}

struct Connection *Database_open(const char *filename, char mode)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn)
        die("Memory error", conn);

    conn->db = malloc(sizeof(struct Database));
    if (!conn->db)
        die("Memory error", conn);
    if (mode == 'c')
    {
        conn->file = fopen(filename, "w");
    }
    else
    {
        conn->file = fopen(filename, "r+");

        if (conn->file)
        {
            Database_load(conn);
        }
    }

    if (!conn->file)
        die("Failed to open the file", conn);

    return conn;
}

void Database_close(struct Connection *conn)
{
    if (conn)
    {
        for (int i = 0; i < conn->db->MAX_ROWS; i++)
        {
            struct Address *row = &conn->db->rows[i];
            if (row->name)
                free(row->name);
            if (row->email)
                free(row->email);
        }
        if (conn->db->rows)
            free(conn->db->rows);
        if (conn->db)
            free(conn->db);
        if (conn->file)
            fclose(conn->file);
        free(conn);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    if (fwrite(&conn->db->MAX_DATA, sizeof(int), 1, conn->file) != 1)
        die("Failed to write MAX_DATA", conn);

    if (fwrite(&conn->db->MAX_ROWS, sizeof(int), 1, conn->file) != 1)
        die("Failed to write MAX_ROWS", conn);

    for (int i = 0; i < conn->db->MAX_ROWS; i++)
    {
        struct Address *row = &conn->db->rows[i];

        if (fwrite(&row->id, sizeof(int), 1, conn->file) != 1)
            die("Failed to write id", conn);

        if (fwrite(&row->age, sizeof(int), 1, conn->file) != 1)
            die("Failed to write age", conn);

        if (fwrite(&row->set, sizeof(int), 1, conn->file) != 1)
            die("Failed to write set", conn);

        if (fwrite(row->name, conn->db->MAX_DATA, 1, conn->file) != 1)
            die("Failed to write name", conn);

        if (fwrite(row->email, conn->db->MAX_DATA, 1, conn->file) != 1)
            die("Failed to write email", conn);

        if (fwrite(row->phone, conn->db->MAX_DATA, 1, conn->file) != 1)
            die("Failed to write phone", conn);

        if (fwrite(row->city, conn->db->MAX_DATA, 1, conn->file) != 1)
            die("Failed to write city", conn);
    }

    if (fflush(conn->file) == -1)
        die("Cannot flush database", conn);
}

void Database_create(struct Connection *conn, int max_data, int max_rows)
{
    conn->db->MAX_DATA = max_data;
    conn->db->MAX_ROWS = max_rows;

    if (conn->db->MAX_ROWS <= 0 || conn->db->MAX_DATA <= 1)
        die("MAX not proper", conn);

    conn->db->rows = malloc(sizeof(struct Address) * conn->db->MAX_ROWS);
    if (!conn->db->rows)
        die("Memory error", conn);

    for (int i = 0; i < conn->db->MAX_ROWS; i++)
    {
        struct Address *addr = &conn->db->rows[i];

        addr->id = i;
        addr->age = 0;
        addr->set = 0;

        addr->name = calloc(conn->db->MAX_DATA, sizeof(char));
        addr->email = calloc(conn->db->MAX_DATA, sizeof(char));
        addr->phone = calloc(conn->db->MAX_DATA, sizeof(char));
        addr->city = calloc(conn->db->MAX_DATA, sizeof(char));

        if (!addr->name || !addr->email || !addr->phone || !addr->city)
            die("Memory error", conn);
    }
}

void Database_set(
    struct Connection *conn,
    int id,
    int age,
    const char *name,
    const char *email,
    const char *phone,
    const char *city)
{
    if (id < 0 || id >= conn->db->MAX_ROWS)
        die("Invalid id", conn);

    struct Address *addr = &conn->db->rows[id];

    if (addr->set)
        die("Already set, delete it first", conn);

    addr->id = id;
    addr->age = age;
    addr->set = 1;

    snprintf(addr->name, conn->db->MAX_DATA, "%s", name);
    snprintf(addr->email, conn->db->MAX_DATA, "%s", email);
    snprintf(addr->phone, conn->db->MAX_DATA, "%s", phone);
    snprintf(addr->city, conn->db->MAX_DATA, "%s", city);
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if (addr->set)
    {
        Address_print(addr);
    }
    else
    {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    char *p1 = (char *)malloc(conn->db->MAX_DATA);
    char *p2 = (char *)malloc(conn->db->MAX_DATA);
    free(conn->db->rows[id].name);
    free(conn->db->rows[id].email);
    struct Address addr = {.id = id, .set = 0, .name = p1, .email = p2};
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    struct Database *db = conn->db;

    for (int i = 0; i < conn->db->MAX_ROWS; i++)
    {
        struct Address *cur = &db->rows[i];
        if (cur->set)
        {
            Address_print(cur);
        }
    }
}

// T3 find
void Database_find(struct Connection *conn, char *find)
{
    struct Database *db = conn->db;
    int count = 0;
    for (int i = 0; i < db->MAX_ROWS; i++)
    {
        struct Address *cur = &db->rows[i];
        if (cur->set == 1 && (strstr(cur->name, find) ||
                              strstr(cur->email, find) ||
                              strstr(cur->phone, find) ||
                              strstr(cur->city, find)

                                  ))
        {
            count++;
            Address_print(cur);
        }
    }
    if (!count)
    {
        printf("not found\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
        die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    if (argc > 3)
        id = atoi(argv[3]);

    switch (action)
    {
    case 'c':
        Database_create(conn, 64, 10);
        Database_write(conn);
        printf("created! \n");
        break;

    case 'g':
        if (argc != 4)
            die("Need an id to get", conn);

        Database_get(conn, id);
        break;

    case 's':
        if (argc != 9)
            die("Need age, name, email, phone and city to set", conn);

        Database_set(conn, id, atoi(argv[4]), argv[5], argv[6], argv[7], argv[8]);
        Database_write(conn);
        break;

    case 'd':
        if (argc != 4)
            die("Need id to delete", conn);

        Database_delete(conn, id);
        Database_write(conn);
        break;

    case 'l':
        Database_list(conn);
        break;
    case 'f':
        if (argc != 4)
            die("输入关键词", conn);
        Database_find(conn, argv[3]);
        break;
    default:
        die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
    }

    Database_close(conn);
    return 0;
}

/*
T4 C语言中结构体大小
sizeof(struct) 得到的是包含内部 padding 和尾部 padding 的总大小
*/
