/* 
 * File:   functions.h
 * Author: AntonioCS
 *
 * Created on 29 de Junho de 2013, 17:42
 */

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

struct Connection *Database_open(const char *, char);
void Database_create(struct Connection *, int, int);
void Database_exit(const char *, struct Connection *);
void Database_write(struct Connection *);
void Database_set(struct Connection *, int, const char *, const char *);
void Database_get(struct Connection *, int);
bool Database_close(struct Connection *);
void Database_list(struct Connection *);
void Database_delete(struct Connection *, int);
void Database_search(struct Connection *, char *[]);

#endif	/* FUNCTIONS_H */

