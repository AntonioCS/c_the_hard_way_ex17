
#include "structs.h"
#include "functions.h"

static void Database_address_print(struct Address *);
static char *safe_strncpy(char *, const char *, size_t);
static struct Address *create_address(int,int);
static struct Address *create_address_array(int);
static void Initialize_database_struct(struct Database *,int);

void Database_exit(const char *message, struct Connection *conn) {
    if (errno) {
        perror(message);
    } 
    else {
        printf("ERROR: %s\n", message);
    }

    conn = NULL;

    exit(1);
}
//Por aqui database. max_rows e max_data
static void Initialize_database_struct(struct Database *conn,int max_rows) {
    
}
static struct Address *create_address_array(int max_rows) {
    return malloc(sizeof(struct Address) * max_rows);
}

struct Address create_address(int max_data, int id) {
    struct Address *addr = malloc(sizeof(struct Address));
    
    if (addr) {
        int size = sizeof(char) * max_data;
        
        char *name = malloc(size);
        char *email = malloc(size);
        
        if (name && email) {
            addr->id = id;
            addr->set = 0;
            addr->name = name;
            addr->email = email;                                              
        }
        else {
            addr = NULL;
        }
        
    }
    
    return addr;    
}

struct Address create_address_fill(struct Connection *conn, int id, char *name, char *email) {
    int max_data = conn->db->max_data;
    struct Address *addr = create_address(max_data,id);    
    
    if (addr && name && email) {
        int size = sizeof(char) * max_data;
        
        if (memmove(addr->name, name,size) == NULL || memmove(addr->email, email,size) == NULL) {
            addr = NULL;
        }
    }
    
    return addr;
    
}


/**
 * Function to load a file or create a new file
 * @param filename
 * @param mode
 * @return 
 */
struct Connection *Database_open(const char *filename, char mode) {
    struct Connection *conn = malloc(sizeof (struct Connection));
    if (!conn) {
        Database_exit("Memory error",conn);
    }

    conn->db = malloc(sizeof (struct Database));
    
    if (!conn->db) {
        Database_exit("Memory error",conn);
    }   

    if (mode == 'c') {
        conn->file = fopen(filename, "w");
    }
    else {
        conn->file = fopen(filename, "r+");  
        if (conn->file) {
            fread(conn->db->max_rows,sizeof(int),1,conn->file);
            fread(conn->db->max_data,sizeof(int),1,conn->file);
                    
            
            char *name;
            char *email;          
            
            for (int i = 0; i < conn->db->max_rows; i++) {
            
            }
            
        //}
        //Loop through the data. 
        /*
         * How it is structures 
         * <int>max_rows
         * <int>max_data
         * ([structure_data]
         *      <int>
         *      <int>
         *      <array char max_data>
         *      <array char max_data>
         * [structure_data]*max_rows)
         * 
         */ 
                    
        }
        
    }

    if (!conn->file) {
        Database_exit("Failed to open the file",conn);
    }
    
    conn->mode = mode;
    
    return conn;
}

void Database_create(struct Connection *conn, int max_rows, int max_data) {
    
    int i = 0;    
    
    struct Address *rows = create_address_array(max_rows);
    
    if (!rows) {
        printf("No memory for rows");
        exit(1);
    }
    
    conn->db->rows = rows;
    conn->db->max_rows = max_rows;
    conn->db->max_data = max_data;
    
    //Create the structure now (not the best idea if you ask me :P)    
    for (i = 0; i < max_rows; i++) {              
        conn->db->rows[i] = create_address(conn,i);
        
        if (conn->db->rows[i] == NULL) {
            Database_exit("Memory error for address", conn);
        }
    }
}

void Database_write(struct Connection *conn) {
    rewind(conn->file);
    
    const int max_rows = conn->db->max_rows;
    const int max_data = conn->db->max_rows;
    
    size_t intsize = sizeof(int);
    
    fwrite(&max_rows,intsize,1,conn->file);
    fwrite(&max_data,intsize,1,conn->file);
    
    for (int i = 0;i < max_rows;i++) {
        fwrite(&(conn->db->rows[i].id),intsize,1,conn->file);
        fwrite(&(conn->db->rows[i].set),intsize,1,conn->file);
        
        fwrite(conn->db->rows[i].name,max_data,1,conn->file);
        fwrite(conn->db->rows[i].email,max_data,1,conn->file);
    }
    
}

/*
int Database_save(struct Connection *conn) {
    int dbSize = sizeof(struct Database);
    fseek(conn->file,dbSize,SEEK_SET);
    
    int x;
    for (x = 0;x<conn->db->max_rows;x++) {
        fwrite(conn->db->rows, sizeof(struct Address) * conn->db->max_rows, 1, conn->file);
        conn->db->max_rows[x]
    }
    //int rc = fwrite(conn->db->rows, sizeof(struct Address) * conn->db->max_rows, 1, conn->file);
    
}
*/
void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
    
    int max_data = conn->db->max_data;
    int max_rows = conn->db->max_rows;    
    
    if (id < 0 || id >= max_rows) { 
        printf("Max rows -> %d\n",max_rows);
        printf("Key -> %d\n",id);
        Database_exit("Invalid key", conn);
    }
    
    struct Address *addr = &conn->db->rows[id];
    
    if (addr->set) {
        Database_exit("Already set, delete it first", conn);
    }

    addr->set = 1;
        
    if (!safe_strncpy(addr->name, name, max_data)) {
        Database_exit("Name copy failed", conn);
    }
    
    if (!safe_strncpy(addr->email, email, max_data)) {
        Database_exit("Email copy failed", conn);
    }
}

void Database_get(struct Connection *conn, int id) {
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Database_address_print(addr);
    } 
    else {
        Database_exit("ID is not set",conn);
    }
}

static void Database_address_print(struct Address *addr) {
    printf("%d %s %s\n",addr->id, addr->name, addr->email);
}

/**
 * This function is needed because strncpy will not include the \0 if the buffer is smaller than the copied string
 * 
 * @param dest
 * @param src
 * @param n
 * @return 
 */
static char *safe_strncpy(char *dest, const char *src, size_t n) {
    if (strlen(src) >= n) {
        n--;
    }
    return strncpy(dest,src,n);
}

bool Database_close(struct Connection *conn)
{
    if(conn) {
        if(conn->file) {
            fclose(conn->file);
        }
        if(conn->db) {
            free(conn->db);
        }
        
        free(conn);
        
        return true;                
    }
    
    return false;
}

void Database_delete(struct Connection *conn, int id)
{
    struct Address addr = {
                    .id = id, 
                    .set = 0
    };
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{

    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < conn->db->max_rows; i++) {
        struct Address *cur = &db->rows[i];

        if(cur->set) {
            Database_address_print(cur);
        }
    }
}