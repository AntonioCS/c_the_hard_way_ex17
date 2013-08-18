
#include "structs.h"
#include "functions.h"


static size_t gSizeofint = sizeof(int);

static void Database_address_print(struct Address *);
static char *safe_strncpy(char *, const char *, size_t);
static struct Address *create_address(int,int,int);
static struct Address *create_address_fill(int,int,int,char*,char*);
static struct Address **create_address_array(int);
static void Database_create_structure(struct Connection *,int,void *);



static struct Address **create_address_array(int max_rows) {
    return malloc(sizeof(struct Address *) * max_rows);
}

static struct Address *create_address(int max_data,int id, int set) {
    
    struct Address *addr = malloc(sizeof(struct Address));            
    char *name = malloc(max_data);
    char *email = malloc(max_data);
        
    if (addr && name && email) {
        addr->id = id;
        addr->set = set;
        addr->name = name;
        addr->email = email;
    }
    else {
        addr = NULL;
    }

    return addr;    
}

/**
 * Return an address structure with everything set
 * 
 * @param max_data
 * @param id
 * @param name
 * @param email
 * @return 
 */
static struct Address *create_address_fill(int max_data, int id, int set,char *name, char *email) {    
    struct Address *addr = create_address(max_data,id,set);    
    
    if (addr && name && email) {        
        if (memcpy(addr->name, name,max_data) == NULL || memcpy(addr->email, email,max_data) == NULL) {
            addr = NULL;
        }
    }
    
    return addr;
}

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
          
        }
        
    }

    if (!conn->file) {
        Database_exit("Failed to open the file",conn);
    }
    
    conn->mode = mode;
    
    return conn;
}

/**
 * Create the struct
 * 
 * @param conn
 * @param max_rows
 * @param max_data
 */
void Database_create(struct Connection *conn, int max_rows, int max_data) {
    
    //int i = 0;            
    
    
    conn->db->rows = create_address_array(max_rows);
    
    if (conn->db->rows == NULL) {
        printf("No memory for rows");
        exit(1);
    }    
    
    conn->db->max_rows = max_rows;
    conn->db->max_data = max_data;
    
    Database_create_structure(conn,max_data,NULL);
    
    //Create the structure now (not the best idea if you ask me :P)    
    /*
    for (i = 0; i < max_rows; i++) {              
        conn->db->rows[i] = create_address(max_data,i,0);
        
        if (conn->db->rows[i] == NULL) {
            Database_exit("Memory error for address", conn);
        }
    }*/
}
/**
 *  Use this to create the rows of addresses in the Database struct
 *  Must figure out a way of either creating a blank address of creating an address with data from the file
 * 
 * @param conn
 * @param fp
 */
static void Database_create_structure(struct Connection *conn,int max_data,void *fp) {
    //static struct Address *create_address(int,int,int);
    //static struct Address *create_address_fill(int,int,int,char*,char*);
        //Loop through the data. 
        /*
         * How it is structured 
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
                    

    for (int i = 0, m = conn->db->max_rows; i < m; i++) {
        if (fp != NULL) {            
            char *name = malloc(max_data);
            char *email = malloc(max_data);
            int *id = malloc(gSizeofint);
            int *set = malloc(gSizeofint);
            
            fread(id,gSizeofint,1,fp);
            fread(set,gSizeofint,1,fp);
            fread(name,max_data,1,fp);            
            fread(email,max_data,1,fp);
            
            conn->db->rows[i] = create_address_fill(max_data,*id,*set,name,email);
        
            free(name);
            free(email);
        }
        else {
            conn->db->rows[i] = create_address(max_data,i,0);
        }
        
        
        
        
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
        fwrite(&(conn->db->rows[i]->id),intsize,1,conn->file);
        fwrite(&(conn->db->rows[i]->set),intsize,1,conn->file);
        
        fwrite(conn->db->rows[i]->name,max_data,1,conn->file);
        fwrite(conn->db->rows[i]->email,max_data,1,conn->file);
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
    
    struct Address *addr = conn->db->rows[id];
    
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
    struct Address *addr = conn->db->rows[id];

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
    if (conn->db->rows[id]->set) {
        conn->db->rows[id]->set = 0;
        
        memset(conn->db->rows[id]->name,0,conn->db->max_data);
        memset(conn->db->rows[id]->name,0,conn->db->max_data);                
    }
}

void Database_list(struct Connection *conn)
{
    for(int i = 0; i < conn->db->max_rows; i++) {        

        if(conn->db->rows[i]->set) {
            Database_address_print(conn->db->rows[i]);
        }
    }
}