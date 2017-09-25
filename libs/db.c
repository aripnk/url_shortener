
/*
 * Copyright (c) 2017 Arif Nur Khoirudin
 *
 * mail : <hello@arifnurkhoirudin.com>
 * site : https://arifnurkhoirudin.com
 *
 * Don't redistribute or copying without owner permission
*/

#include "db.h"

int
db_init(db_t* db)
{
  if (db == NULL) return db_GENERAL_ERROR;

  db->lastError = NULL;
  db->options = leveldb_options_create();
  db->woptions = leveldb_writeoptions_create();
  db->roptions = leveldb_readoptions_create();

  /*jika db ada just open, jika belum create it*/
  leveldb_options_set_create_if_missing(db->options, 1);
  db->db = leveldb_open(db->options, DBNAME, &(db->lastError));

  if (db->lastError != NULL){
    leveldb_free(db->lastError); db->lastError = NULL;
    return db_GENERAL_ERROR;
  }

  leveldb_free(db->lastError); db->lastError = NULL;
  return db_SUCCESS;
}

int
db_write(db_t* db, const char* key,
             uint32_t keylen, const char* val, uint32_t vallen)
{
  if (db == NULL || key == NULL || val == NULL) return db_GENERAL_ERROR;

  leveldb_put(db->db, db->woptions, key,
              keylen, val, vallen,
              &(db->lastError));

  if (db->lastError != NULL){
    leveldb_free(db->lastError); db->lastError = NULL;
    return db_WRITE_ERROR;
  }

  leveldb_free(db->lastError); db->lastError = NULL;
  return db_SUCCESS;
}

int
db_read(db_t* db, const char* key,
            uint32_t keylen, char** val, size_t vallen)
{

  if (db == NULL || key == NULL || val == NULL) return db_GENERAL_ERROR;

  *val = leveldb_get(db->db, db->roptions, (char*)key,
                    keylen, &(db->datalen),
                    &(db->lastError));


  if (db->lastError != NULL){
    leveldb_free(db->lastError); db->lastError = NULL;
    return db_READ_ERROR;
  }

  leveldb_free(db->lastError); db->lastError = NULL;
  return db_SUCCESS;
}

int
db_delete(db_t* db, const char* key, uint32_t keylen)
{
  if (db == NULL || key == NULL) return db_GENERAL_ERROR;

  leveldb_delete(db->db, db->woptions, key,
                 keylen, &(db->lastError));

  if (db->lastError != NULL){
    leveldb_free(db->lastError); db->lastError = NULL;
    return db_DELETE_ERROR;
  }

  leveldb_free(db->lastError); db->lastError = NULL;
  return db_SUCCESS;
}


void
db_free(db_t* db)
{
  if (db == NULL) return;
  leveldb_close(db->db);
}

int
db_destroy(db_t* db)
{
  if (db == NULL) return db_GENERAL_ERROR;

  leveldb_destroy_db(db->options, DBNAME, &(db->lastError));

  if (db->lastError != NULL){
      leveldb_free(db->lastError); db->lastError = NULL;
      return db_DESTROY_ERROR;
  }

  leveldb_free(db->lastError); db->lastError = NULL;
  return db_SUCCESS;
}
