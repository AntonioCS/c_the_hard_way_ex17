#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "Database/structs.h"
#include "Database/functions.h"


/*
 Change the code to accept parameters for MAX_DATA and MAX_ROWS, 
 * store them in the Database struct, 
 * and write that to the file, thus creating a database that can be arbitrarily sized.
 */

/*#define MAX_DATA 512
#define MAX_ROWS 100*/

int main(int argc, char *argv[]) {
    struct Connection *conn = NULL;
    
    if (argc < 3) {
        Database_exit("USAGE: ex17 <dbfile> <action> [action params]", conn);
    }

    char *filename = argv[1];
    char action = argv[2][0];

    /**
     * 
     * Start connection
     */
    conn = Database_open(filename, action);
    
    int id = 0;

    if (argc > 3) {
        id = atoi(argv[3]);
    } 
       
    // if(id >= MAX_ROWS) die("There's not that many records.");

    switch (action) {
        case 'c':
            if (argc < 3) {
                Database_exit("You need to specify MAX_ROWS and MAX_DATA", conn);
            }            
            int max_rows = atoi(argv[3]);
            int max_data = atoi(argv[4]);                        
            
            Database_create(conn,max_rows,max_data);
            Database_write(conn);
            break;
///*
        case 'g':
            if (argc != 4) Database_exit("Need an id to get",conn);

            Database_get(conn, id);
            break;

        case 's':
            if (argc != 6) Database_exit("Need id, name, email to set",conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if (argc != 4) Database_exit("Need id to delete",conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
        default:
            Database_exit("Invalid action, only: c=create, g=get, s=set, d=del, l=list",conn);
 //*/
    }

    Database_close(conn);

    return 0;
}

