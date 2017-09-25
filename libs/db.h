
/*
 * Copyright (c) 2017 Arif Nur Khoirudin
 *
 * mail : <hello@arifnurkhoirudin.com>
 * site : https://arifnurkhoirudin.com
 *
 * Don't redistribute or copying without owner permission
*/

#include "commons.h"

#define DBNAME                  "shortn"

#define MAX_KEY_LEN             1024
#define MAX_VAL_LEN             1024*32

#define db_SUCCESS        0
#define db_GENERAL_ERROR  1
#define db_WRITE_ERROR    2
#define db_READ_ERROR     3
#define db_DELETE_ERROR   4
#define db_UPDATE_ERROR   5
#define db_DESTROY_ERROR  6


struct db_t{
  leveldb_t *db;
  leveldb_options_t *options;
  leveldb_readoptions_t *roptions;
  leveldb_writeoptions_t *woptions;
  char* lastError;
  size_t datalen;
};
typedef struct db_t db_t;


int
db_init(db_t* db);

int
db_write(db_t* db, const char* key,
             uint32_t keylen, const char* val, uint32_t vallen);

int
db_read(db_t* db, const char* key,
            uint32_t keylen, char** val, size_t vallen);

int
db_delete(db_t* db, const char* key, uint32_t keylen);

void
db_free(db_t* db);

int
db_destroy(db_t* db);
