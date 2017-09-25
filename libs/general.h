
/*
 * Copyright (c) 2017 Arif Nur Khoirudin
 *
 * mail : <hello@arifnurkhoirudin.com>
 * site : https://arifnurkhoirudin.com
 *
 * Don't redistribute or copying without owner permission
*/

#include "commons.h"
#include "db.h"
#include "../external/cJSON/cJSON.h"

#define MAX_URL_LENGTH      1024*32
#define MAX_SHORTN_LENGTH   64

#define SHORTN_SUCCESS 0
#define SHORTN_ERROR   1

struct shortn_t{
  db_t db;
  uint32_t urllen;
  uint32_t shortnlen;
  uint32_t shortnsize;
  uint32_t urlsize;
  char url[MAX_URL_LENGTH];
  char shortn[MAX_SHORTN_LENGTH];
  long unsigned int counter;
  size_t counterlen;
  char index[4089];
  char data_tmp[2048];
  char apilist[2048];
  cJSON* root;
};
typedef struct shortn_t shortn_t;

void
shortn_init(shortn_t* shortn);

void
shortn_reset(shortn_t* shortn);

void
shortn_free(shortn_t* shortn);

void
shortn_getrandomurl(shortn_t* shortn, const char* url);

void
shortn_assign_counter(shortn_t* shortn);

int
is_avalaible(shortn_t* shortn);

int
shortn_getcustomurl(shortn_t* shortn, const char* url, const char* custom);

int
shortn_redirect(shortn_t* shortn, const char* uri);

void
shortn_generaterandom(shortn_t* shortn);

int
shortn_write(shortn_t* shortn);

void
shortn_read(shortn_t* shortn);

char*
shortn_find_payload(shortn_t* shortn, const char* payload, char* head);

int
shortn_validate_payload(const char* payload);

void
shortn_incr_stats(shortn_t* shortn, char* dest);

char* base64enc(long unsigned int value);
