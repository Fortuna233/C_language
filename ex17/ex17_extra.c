#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
    int id;
    int age;
    int set;
    char *name;
    char *email;
    char *phone;
    char *city;
};

struct Database {
    int max_data;
    int max_rows;
    struct Address *rows;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    Database_close(conn);
    exit(1);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %d %s %s %s\n",
           addr->id, addr->name, addr->age,
           addr->email, addr->phone, addr->city);
}

static void Address_free(struct Address *addr)
{
    if(!addr) return;

    free(addr->name);
    free(addr->email);
    free(addr->phone);
    free(addr->city);

    addr->name = NULL;
    addr->email = NULL;
    addr->phone = NULL;
    addr->city = NULL;
}

static void Address_alloc(struct Connection *conn, struct Address *addr)
{
    int max_data = conn->db->max_data;

    addr->name = calloc(max_data, sizeof(char));
    addr->email = calloc(max_data, sizeof(char));
    addr->phone = calloc(max_data, sizeof(char));
    addr->city = calloc(max_data, sizeof(char));

    if(!addr->name || !addr->email || !addr->phone || !addr->city)
        die("Memory error", conn);
}

static void Database_alloc_rows(struct Connection *conn)
{
    int max_rows = conn->db->max_rows;

    conn->db->rows = calloc(max_rows, sizeof(struct Address));
    if(!conn->db->rows)
        die("Memory error", conn);

    for(int i = 0; i < max_rows; i++) {
        struct Address *addr = &conn->db->rows[i];
        addr->id = i;
        addr->age = 0;
        addr->set = 0;
        Address_alloc(conn, addr);
    }
}

void Database_load(struct Connection *conn)
{
    assert(conn && conn->db && conn->file);

    if(fread(&conn->db->max_data, sizeof(int), 1, conn->file) != 1)
        die("Failed to load max_data", conn);

    if(fread(&conn->db->max_rows, sizeof(int), 1, conn->file) != 1)
        die("Failed to load max_rows", conn);

    if(conn->db->max_rows <= 0 || conn->db->max_data <= 1)
        die("Invalid database header", conn);

    Database_alloc_rows(conn);

    for(int i = 0; i < conn->db->max_rows; i++) {
        struct Address *row = &conn->db->rows[i];

        if(fread(&row->id, sizeof(int), 1, conn->file) != 1)
            die("Failed to load id", conn);
        if(fread(&row->age, sizeof(int), 1, conn->file) != 1)
            die("Failed to load age", conn);
        if(fread(&row->set, sizeof(int), 1, conn->file) != 1)
            die("Failed to load set", conn);

        if(fread(row->name, conn->db->max_data, 1, conn->file) != 1)
            die("Failed to load name", conn);
        if(fread(row->email, conn->db->max_data, 1, conn->file) != 1)
            die("Failed to load email", conn);
        if(fread(row->phone, conn->db->max_data, 1, conn->file) != 1)
            die("Failed to load phone", conn);
        if(fread(row->city, conn->db->max_data, 1, conn->file) != 1)
            die("Failed to load city", conn);

        row->name[conn->db->max_data - 1] = '\0';
        row->email[conn->db->max_data - 1] = '\0';
        row->phone[conn->db->max_data - 1] = '\0';
        row->city[conn->db->max_data - 1] = '\0';
    }
}

struct Connection *Database_open(const char *filename, char mode)
{
    struct Connection *conn = calloc(1, sizeof(struct Connection));
    if(!conn)
        die("Memory error", NULL);

    conn->db = calloc(1, sizeof(struct Database));
    if(!conn->db)
        die("Memory error", conn);

    if(mode == 'c') {
        conn->file = fopen(filename, "wb+");
    } else {
        conn->file = fopen(filename, "rb+");
        if(conn->file) {
            Database_load(conn);
        }
    }

    if(!conn->file)
        die("Failed to open the file", conn);

    return conn;
}

void Database_close(struct Connection *conn)
{
    if(conn) {
        if(conn->db) {
            if(conn->db->rows) {
                for(int i = 0; i < conn->db->max_rows; i++) {
                    Address_free(&conn->db->rows[i]);
                }
                free(conn->db->rows);
            }
            free(conn->db);
        }

        if(conn->file)
            fclose(conn->file);

        free(conn);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    if(fwrite(&conn->db->max_data, sizeof(int), 1, conn->file) != 1)
        die("Failed to write max_data", conn);

    if(fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file) != 1)
        die("Failed to write max_rows", conn);

    for(int i = 0; i < conn->db->max_rows; i++) {
        struct Address *row = &conn->db->rows[i];

        if(fwrite(&row->id, sizeof(int), 1, conn->file) != 1)
            die("Failed to write id", conn);
        if(fwrite(&row->age, sizeof(int), 1, conn->file) != 1)
            die("Failed to write age", conn);
        if(fwrite(&row->set, sizeof(int), 1, conn->file) != 1)
            die("Failed to write set", conn);

        if(fwrite(row->name, conn->db->max_data, 1, conn->file) != 1)
            die("Failed to write name", conn);
        if(fwrite(row->email, conn->db->max_data, 1, conn->file) != 1)
            die("Failed to write email", conn);
        if(fwrite(row->phone, conn->db->max_data, 1, conn->file) != 1)
            die("Failed to write phone", conn);
        if(fwrite(row->city, conn->db->max_data, 1, conn->file) != 1)
            die("Failed to write city", conn);
    }

    if(fflush(conn->file) == -1)
        die("Cannot flush database", conn);
}

void Database_create(struct Connection *conn, int max_data, int max_rows)
{
    conn->db->max_data = max_data;
    conn->db->max_rows = max_rows;

    if(conn->db->max_rows <= 0 || conn->db->max_data <= 1)
        die("max_data must be > 1 and max_rows must be > 0", conn);

    Database_alloc_rows(conn);
}

void Database_set(struct Connection *conn, int id, int age,
                  const char *name, const char *email,
                  const char *phone, const char *city)
{
    if(id < 0 || id >= conn->db->max_rows)
        die("Invalid id", conn);

    struct Address *addr = &conn->db->rows[id];

    if(addr->set)
        die("Already set, delete it first", conn);

    addr->id = id;
    addr->age = age;
    addr->set = 1;

    snprintf(addr->name, conn->db->max_data, "%s", name);
    snprintf(addr->email, conn->db->max_data, "%s", email);
    snprintf(addr->phone, conn->db->max_data, "%s", phone);
    snprintf(addr->city, conn->db->max_data, "%s", city);
}

void Database_get(struct Connection *conn, int id)
{
    if(id < 0 || id >= conn->db->max_rows)
        die("Invalid id", conn);

    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    if(id < 0 || id >= conn->db->max_rows)
        die("Invalid id", conn);

    struct Address *addr = &conn->db->rows[id];

    addr->age = 0;
    addr->set = 0;
    memset(addr->name, 0, conn->db->max_data);
    memset(addr->email, 0, conn->db->max_data);
    memset(addr->phone, 0, conn->db->max_data);
    memset(addr->city, 0, conn->db->max_data);
}

void Database_list(struct Connection *conn)
{
    struct Database *db = conn->db;

    for(int i = 0; i < db->max_rows; i++) {
        struct Address *cur = &db->rows[i];
        if(cur->set) {
            Address_print(cur);
        }
    }
}

void Database_find(struct Connection *conn, const char *keyword)
{
    struct Database *db = conn->db;
    int found = 0;

    for(int i = 0; i < db->max_rows; i++) {
        struct Address *cur = &db->rows[i];

        if(cur->set &&
           (strstr(cur->name, keyword) ||
            strstr(cur->email, keyword) ||
            strstr(cur->phone, keyword) ||
            strstr(cur->city, keyword))) {
            Address_print(cur);
            found++;
        }
    }

    if(!found)
        printf("not found\n");
}

static void usage(void)
{
    printf("USAGE:\n");
    printf("  ex17 <dbfile> c <max_data> <max_rows>\n");
    printf("  ex17 <dbfile> s <id> <age> <name> <email> <phone> <city>\n");
    printf("  ex17 <dbfile> g <id>\n");
    printf("  ex17 <dbfile> d <id>\n");
    printf("  ex17 <dbfile> l\n");
    printf("  ex17 <dbfile> f <keyword>\n");
}

int main(int argc, char *argv[])
{
    if(argc < 3) {
        usage();
        return 1;
    }

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    if(argc > 3)
        id = atoi(argv[3]);

    switch(action) {
        case 'c':
            if(argc != 5)
                die("Need max_data and max_rows to create", conn);
            Database_create(conn, atoi(argv[3]), atoi(argv[4]));
            Database_write(conn);
            printf("created! max_data=%d max_rows=%d\n",
                   conn->db->max_data, conn->db->max_rows);
            break;

        case 'g':
            if(argc != 4)
                die("Need an id to get", conn);
            Database_get(conn, id);
            break;

        case 's':
            if(argc != 9)
                die("Need id, age, name, email, phone, and city to set", conn);
            Database_set(conn, id, atoi(argv[4]), argv[5], argv[6], argv[7], argv[8]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4)
                die("Need id to delete", conn);
            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            if(argc != 3)
                die("List takes no extra parameters", conn);
            Database_list(conn);
            break;

        case 'f':
            if(argc != 4)
                die("Need keyword to find", conn);
            Database_find(conn, argv[3]);
            break;

        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list, f=find", conn);
    }

    Database_close(conn);
    return 0;
}

/*
结构体打包说明：
- 本版本不会把 struct Address 整体写入文件，因为它包含指针。指针值只在当前进程有效，写入文件没有意义。
- 磁盘格式为：
  int max_data + int max_rows + max_rows * (int id + int age + int set + 4 * max_data 字节字符串)
- 因此文件大小公式通常为：
  2 * sizeof(int) + max_rows * (3 * sizeof(int) + 4 * max_data)
- 例如 max_data=64, max_rows=10, sizeof(int)=4 时：
  2*4 + 10*(3*4 + 4*64) = 2688 字节。
- 内存中的 sizeof(struct Address) 会包含指针和 padding；在 64 位系统上通常是 48 字节：
  3 个 int 为 12 字节，随后 4 字节 padding 对齐到 8 字节，4 个 char* 为 32 字节，总计 48 字节。
*/
