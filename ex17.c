#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
    int id;
    int set;
    char *name;
    char *email;
};

struct Database {
	int MAX_DATA;
	int MAX_ROWS;
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
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	assert(conn->db && conn->file);
	if(fread(conn->db, sizeof(struct Database), 1, conn->file) != 1)
	{
		die("Failed to load database.", conn);
	}
	conn->db->rows = (struct Address*)malloc(sizeof(struct Address) * conn->db->MAX_ROWS);
	for(int i = 0; i < conn->db->MAX_ROWS; i++)
	{
		struct Address* row = &conn->db->rows[i];
		if(fread(&row->id, sizeof(int), 1, conn->file) != 1) die("Failed to load id.", conn);
		if(fread(&row->set, sizeof(int), 1, conn->file) != 1) die("Failed to load set.", conn);
		row->name = malloc(conn->db->MAX_DATA);
		row->email = malloc(conn->db->MAX_DATA);
		if(fread(row->name, conn->db->MAX_DATA, 1, conn->file) != 1) die("Faile to load name", conn);
		if(fread(row->email, conn->db->MAX_DATA, 1, conn->file) != 1) die("Faile to load email", conn);
   }
}

struct Connection *Database_open(const char *filename, char mode)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(!conn) die("Memory error", conn);

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) die("Memory error", conn);
    if(mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if(conn->file) {
            Database_load(conn);
        }
    }

    if(!conn->file) die("Failed to open the file", conn);

    return conn;
}

void Database_close(struct Connection *conn)
{
  if(conn)
  {
	  for(int i = 0; i < conn->db->MAX_ROWS; i++)
	  {
		struct Address* row = &conn->db->rows[i];
		if(row->name) free(row->name);
		if(row->email) free(row->email);
	  }
	  if(conn->db->rows) free(conn->db->rows);
	  if(conn->db) free(conn->db);
	  if(conn->file) fclose(conn->file);
	  free(conn);
  }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1)
		{
			die("Failed to write database.", conn);
		}

		for(int i = 0; i < conn->db->MAX_ROWS; i++)
		{
			if(fwrite(&((conn->db->rows[i]).id), sizeof(int), 1, conn->file) != 1)
				die("failed to write id\n", conn);
			if(fwrite(&((conn->db->rows[i]).set), sizeof(int), 1, conn->file) != 1)
				die("failed to write set\n", conn);
			if(fwrite(conn->db->rows[i].name, conn->db->MAX_DATA, 1, conn->file) != 1)
				die("failed to write name\n", conn);
			if(fwrite(&conn->db->rows[i].email, conn->db->MAX_DATA, 1, conn->file) != 1)
				die("failed to write email\n", conn);
		}
    rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn)
{
	printf("输入MAX_DATA MAX_ROWS\n");
	scanf("%d %d", &conn->db->MAX_DATA, &conn->db->MAX_ROWS);
	printf("%d %d\n", conn->db->MAX_DATA, conn->db->MAX_ROWS);
	if(conn->db->MAX_ROWS < 0 || conn->db->MAX_DATA < 0) die("MAX not proper\n", conn);

	conn->db->rows = (struct Address*)malloc(sizeof(struct Address)*conn->db->MAX_ROWS);
	for(int i = 0; i < conn->db->MAX_ROWS; i++)
	{
		char* a = (char*)malloc(conn->db->MAX_ROWS);
		char* b = (char*)malloc(conn->db->MAX_DATA);
		memset(a, 0, conn->db->MAX_ROWS);
		memset(b, 0, conn->db->MAX_DATA);
		struct Address addr = {.id = i, .set = 0, .name = a, .email = b};
		conn->db->rows[i] = addr;
  }
}

void Database_set(struct Connection *conn, int id, char *name, char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die("Already set, delete it first", conn);

    addr->set = 1;
	// 传入的字符指针的最长长度设置
	name[conn->db->MAX_DATA-1] = '\0';
	email[conn->db->MAX_DATA-1] = '\0';
    char *res = strncpy(addr->name, name, conn->db->MAX_DATA);
    if(!res) die("Name copy failed", conn);

    res = strncpy(addr->email, email, conn->db->MAX_DATA);
    if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
	char* p1 = (char*)malloc(conn->db->MAX_DATA);
	char* p2 = (char*)malloc(conn->db->MAX_DATA);
	free(conn->db->rows[id].name);
	free(conn->db->rows[id].email);
    struct Address addr = {.id = id, .set = 0, .name = p1, .email =p2};
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    struct Database *db = conn->db;

    for(int i = 0; i < conn->db->MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];
        if(cur->set) {
            Address_print(cur);
        }
    }
}

void Database_find(struct Connection *conn, char* find)
{
	int count = 0;
	for(int i = 0; i < conn->db->MAX_ROWS; i++)
	{
		if(conn->db->rows[i].set == 1)
		{
			if(strstr(conn->db->rows[i].name, find) != NULL || strstr(conn->db->rows[i].email, find) != NULL)
			{
				count++;
				Address_print(&conn->db->rows[i]);
			}
		}
	}
	if(count == 0)
	{
		printf("not found\n");
	}
}
	



int main(int argc, char *argv[])
{
    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    if(argc > 3) id = atoi(argv[3]);

    switch(action) {
        case 'c':
            Database_create(conn);
            Database_write(conn);
			printf("成功创建i\n");
            break;

        case 'g':
            if(argc != 4) die("Need an id to get", conn);

            Database_get(conn, id);
            break;

        case 's':
            if(argc != 6) die("Need id, name, email to set", conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4) die("Need id to delete", conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
		case 'f':
			if(argc != 4) die("输入关键词", conn);
			Database_find(conn, argv[3]);
			break;
        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
    }

    Database_close(conn);
    return 0;
}
