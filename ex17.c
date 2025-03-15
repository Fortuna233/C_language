#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>


struct Address 
{
	int id;
	int set;
	char *name;
	char *email;
};

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

void die(const char *message, struct Connection *conn)
{
	if(errno) {
		perror(message);
	}
	else {
		printf("ERROR: %s\n", message);
	}
	if(conn)
	{
		for(int i = 0; i<conn->db->MAX_ROWS; i++)
		{
			free(conn->db->rows[i].name);
			free(conn->db->rows[i].email);
		}
		free(conn->db->rows);
		free(conn->db);
		free(conn);
	}

	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1)
		die("Failed to load database.", conn);
}

struct Connection *Database_open(const char *filename, char mode, int datasize, int mrows)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) die("Memory error", conn);

	conn->db = malloc(sizeof(struct Database));
	conn->db->MAX_DATA = datasize;
	conn->db->MAX_ROWS = mrows;
	
	conn->db->rows = malloc(sizeof(struct Address) * MAX_ROWS);
	for(int i = 0; i < MAX_ROWS; i++)
	{
		conn->db->rows[i].name = malloc(MAX_DATA);
		conn->db->rows[i].email = malloc(MAX_DATA);
	}

	if(!conn) die("Memory error", conn);

	if(mode == 'c') {
		conn->file = fopen(filename, "w");
	}
	else {
		conn->file = fopen(filename, "r+");
		if(conn->file)
		{
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
		for(int i = 0; i<conn->db->MAX_ROWS; i++)
		{
			free(conn->db->rows[i].name);
			free(conn->db->rows[i].email);
		}
		free(conn->db->rows);
		free(conn->db);
		free(conn);
	}
}

void Database_write(struct Connection *conn)
{
	rewind(conn->file);
	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die("Failed to write database.", conn);

	rc = fflush(conn->file);
	if(rc == -1) die("cannot flush database", conn);
}

void Database_create(struct Connection *conn, int rows, int datasize)
{
	conn->db->MAX_DATA = datasize;
	conn->db->MAX_ROWS = rows;
	for(int i = 0; i < rows; i++)
	{
		struct Address addr = {.id = i, .set = 0};
		conn->db->rows[i] = addr;
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
	struct Address *addr = &conn->db->rows[id];
	if(addr->set) die("Already set, delete if first", conn);

	addr->set = 1;
	char *res = strncpy(addr->name, name, MAX_DATA);
	if(!res) die("Name copy failed", conn);

	res = strncpy(addr->email, email, MAX_DATA);
	if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
	struct Address *addr = &conn->db->rows[id];
	
	if(addr->set) {
		Address_print(addr);
	}
	else {
		die("ID is not set", conn);
	}
}

void Database_delete(struct Connection *conn, int id)
{
	struct Address addr = {.id = id, .set = 0};
	conn->db->rows[id] =addr;
}

void Database_list(struct Connection *conn)
{
	struct Database *db = conn->db;
	for(int i = 0; i < db->MAX_ROWS; i++)
	{
		struct Address *cur = &db->rows[i];
		if(cur->set) {
			Address_print(cur);
		}
	}
}

int main(int argc, char *argv[])
{
	if(argc < 3) die("USAGE: ex17 <DBFILE> <ACTION> [ACTION PARAMS]\n", NULL);
	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action, datasize = 512, mrows = 100);
	int id = 0;

	if(argc > 3) id = atoi(argv[3]);
	if(id >= MAX_ROWS) die("there's not that many records.", conn);
	switch(action) {
		case 'c':
			Database_create(conn, rows, datasize);
			Database_write(conn);
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
			Database_list(conn);
			Database_write(conn);
			break;

		case 'l':
			Database_list(conn);
			break;

		default:
			die("Invalid action, c=create, g=get, s=set, d=del,l=list", NULL);
	}

	Database_close(conn);
	return 0;
}





