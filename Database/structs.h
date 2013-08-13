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

struct Address {
    int id;
    int set;
    //    char name[MAX_DATA];
    //  char email[MAX_DATA];
    char *name;
    char *email;
};

struct Database {
    //struct Address rows[MAX_ROWS];
    struct Address *rows;
    int max_rows; 
    int max_data;
};

struct Connection {
    FILE *file;
    struct Database *db;
    char mode;
};
#endif	/* STRUCTS_H */