#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "Database/structs.h"
#include "Database/functions.h"


/*
Extra Credit

    The die function needs to be augmented to let you pass the conn variable so it can close it and clean up.
    DONE
    Change the code to accept parameters for MAX_DATA and MAX_ROWS, store them in the Database struct, and write that to the file, thus creating a database that can be arbitrarily sized.
    DONE 
    Add more operations you can do on the database, like find.
 
    Read about how C does it's struct packing, and then try to see why your file is the size it is. See if you can calculate a new size after adding more fields.
 *
    Add some more fields to the Address and make them searchable.
 * 
    Write a shell script that will do your testing automatically for you by running commands in the right order. 
   Hint: Use set -e at the top of a bash to make it abort the whole script if any command has an error.
 
    Try reworking the program to use a single global for the database connection. How does this new version of the program compare to the other one?
 * 
    Go research "stack data structure" and write one in your favorite language, then try to do it in C.
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
    DEBUG_MSG("Creating conn");
    conn = Database_open(filename, action);
    DEBUG_MSG("Created conn");
    int id = 0;

    if (argc > 3) {
        id = atoi(argv[3]);
        //since arrays are zero based I have to decrement
        id--;
        
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
            DEBUG_MSG("!!Get!!");
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
        case 'f':
            Database_search(conn,argv[1]);
        break;
        default:
            Database_exit("Invalid action, only: c=create, g=get, s=set, d=del, l=list",conn);
 //*/
    }

    Database_close(conn);

    return 0;
}

