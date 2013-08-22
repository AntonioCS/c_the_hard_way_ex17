#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

/* 
 * File:   structs.h
 * Author: AntonioCS
 *
 * Created on 29 de Junho de 2013, 13:42
 */

#ifndef STRUCTS_H
#define	STRUCTS_H

struct Connection {
    FILE *file;
    struct Database *db;
    char mode;
};

struct Database {   
    //Pointer to an array of stuct Address pointers
    struct Address **rows;
    int max_rows; 
    int max_data;
   //struct Address rows[MAX_ROWS];
};

struct Address {
    int id;
    int set;
    char *name;
    char *email;    
    //    char name[MAX_DATA];
    //  char email[MAX_DATA];
};

#define DEBUG true

#ifdef DEBUG
#define DEBUG_MSG(msg) (printf("Debug: %s\n",msg))
#else
#define DEBUG_MSG(msg)
#endif

#endif	/* STRUCTS_H */