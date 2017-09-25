
/*
 * Copyright (c) 2017 Arif Nur Khoirudin
 *
 * mail : <hello@arifnurkhoirudin.com>
 * site : https://arifnurkhoirudin.com
 *
 * Don't redistribute or copying without owner permission
*/

#include "general.h"

void
shortn_init(shortn_t* shortn)
{
  if (shortn == NULL) return;
  memset(shortn, 0, sizeof(shortn_t));
  db_init(&(shortn->db));
}

void
shortn_reset(shortn_t* shortn)
{
  if (shortn == NULL) return;
  memset(shortn, 0, sizeof(shortn_t));
}

void
shortn_free(shortn_t* shortn)
{
  if (shortn == NULL) return;
  memset(shortn, 0, sizeof(shortn_t));
  db_free(&(shortn->db));
}

void
shortn_getrandomurl(shortn_t* shortn, const char* url)
{
  if (shortn == NULL || url == NULL) return;
  shortn->urllen = strlen(url);
  memset(shortn->url,0,MAX_URL_LENGTH);
  strcpy(shortn->url, url);

  shortn_generaterandom(shortn);
  while (is_avalaible(shortn) == SHORTN_ERROR){
    shortn->counter++;
    shortn_generaterandom(shortn);
  }
}

int
shortn_getcustomurl(shortn_t* shortn, const char* url, const char* custom)
{
  if (shortn == NULL || url == NULL || custom == NULL) return SHORTN_ERROR;
  shortn->urllen = strlen(url);
  memset(shortn->url,0,MAX_URL_LENGTH);
  strcpy(shortn->url, url);
  shortn->shortnlen = strlen(custom);
  shortn->shortnsize = sizeof(custom);
  memset(shortn->shortn, 0, MAX_SHORTN_LENGTH);
  strncpy(shortn->shortn, custom, shortn->shortnlen);
  // memcpy(shortn->shortn, custom, shortn->shortnlen);

  printf("get custom url %s\n", shortn->shortn);
  if (is_avalaible(shortn) == SHORTN_ERROR){
    return SHORTN_ERROR;
  }

  return SHORTN_SUCCESS;
}

int
is_avalaible(shortn_t* shortn)
{
  if (shortn == NULL) return -1;
  char* data = NULL;
  db_read(&(shortn->db), shortn->shortn, shortn->shortnlen, &data, shortn->counterlen);
  if (data != NULL){
    //sudah ada data
    printf("%s\n", "sudah ada data");
    return SHORTN_ERROR;
  }else{
    //DATA TIDAK AD == OK
    return SHORTN_SUCCESS;
  }
}

void
shortn_generaterandom(shortn_t* shortn){
  if (shortn == NULL) return;
  char* encoded = base64enc(shortn->counter);
  shortn->shortnlen = strlen(encoded);
  shortn->shortnsize = sizeof(encoded);
  strcpy(shortn->shortn,encoded);
  free (encoded);
}

void
shortn_assign_counter(shortn_t* shortn)
{
  if (shortn == NULL) return;
  char* counter;
  char buffer[24];
  db_read(&(shortn->db), "counter", 7, &counter, shortn->counterlen);
  if (counter != NULL){
    shortn->counter = atoi(counter)+1;
  }else{
    shortn->counter = 0;
  }
  sprintf(buffer, "%lu",shortn->counter);

  db_write(&(shortn->db), "counter", 7, buffer, strlen(buffer));
}

int
shortn_redirect(shortn_t* shortn, const char* uri)
{
  if (shortn == NULL) return SHORTN_ERROR;
  char dst[24];
  memcpy(dst, uri+1, strlen(uri)+1);

  char* data = NULL;
  size_t vallen;
  db_read(&(shortn->db), dst, strlen(dst), &data, vallen);

  if (data == NULL){
    return SHORTN_ERROR;
  }

  shortn->urllen = shortn->db.datalen;
  memcpy(shortn->shortn, dst, strlen(dst));
  memset(shortn->url,0,MAX_URL_LENGTH);
  strncpy(shortn->url, data, shortn->urllen);
  if (strstr(dst,".") == NULL) shortn_incr_stats(shortn, dst);
  return SHORTN_SUCCESS;
}

void
shortn_incr_stats(shortn_t* shortn, char* dest)
{
  if (shortn == NULL || dest == NULL) return;
  char* statscounter;
  char buffer[24];
  char url[24];
  int tmp;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  sprintf(url,"%s.%d.%d.%d",dest,tm.tm_year + 1900, tm.tm_mon + 1,tm.tm_mday); //yyymmdd
  db_read(&(shortn->db), url, strlen(url), &statscounter, shortn->counterlen);
  if (statscounter != NULL){
    tmp = atoi(statscounter)+1;
  }else{
    tmp = 1;
  }
  sprintf(buffer,"%d",tmp);
  // printf("url %s count %s\n",url,buffer);
  db_write(&(shortn->db), url, strlen(url), buffer, strlen(buffer));

  sprintf(url,"%s.%d.%d",dest,tm.tm_year + 1900, tm.tm_mon + 1); //yyymm
  db_read(&(shortn->db), url, strlen(url), &statscounter, shortn->counterlen);
  if (statscounter != NULL){
    tmp = atoi(statscounter)+1;
  }else{
    tmp = 1;
  }
  sprintf(buffer,"%d",tmp);
  // printf("url %s count %s\n",url,buffer);

  db_write(&(shortn->db), url, strlen(url), buffer, strlen(buffer));

  sprintf(url,"%s.%d",dest,tm.tm_year + 1900); //yyy
  db_read(&(shortn->db), url, strlen(url), &statscounter, shortn->counterlen);
  if (statscounter != NULL){
    tmp = atoi(statscounter)+1;
  }else{
    tmp = 1;
  }
  sprintf(buffer,"%d",tmp);

  db_write(&(shortn->db), url, strlen(url), buffer, strlen(buffer));
}

int
shortn_write(shortn_t* shortn)
{
  if (shortn == NULL) return SHORTN_ERROR;
  if (db_write(&(shortn->db), (const char*)(shortn->shortn),
           shortn->shortnlen, (const char*)(shortn->url), shortn->urllen) == db_SUCCESS)
           return SHORTN_SUCCESS;

  return SHORTN_ERROR;
}

char*
shortn_find_payload(shortn_t* shortn, const char* payload, char* head)
{
  if (payload == NULL || head == NULL || shortn == NULL) return NULL;
  // printf("%s\n","here" );
  shortn->root = cJSON_Parse(payload);
  int i;
  for (i = 0; i < cJSON_GetArraySize(shortn->root); i++){
    cJSON *subitem = cJSON_GetArrayItem(shortn->root, i);
    // handle subitem
    char* found = cJSON_GetObjectItem(subitem, "name")->valuestring;

    if (strcmp(head,found) == 0){
      strcpy(shortn->data_tmp,cJSON_GetObjectItem(subitem, "value")->valuestring);
      break;
    }
  }
  cJSON_Delete(shortn->root);
  // printf("found val = %s\n",shortn->data_tmp);

  return strdup(shortn->data_tmp);
}


int
shortn_validate_payload(const char* payload)
{
  if (payload == NULL) return SHORTN_ERROR;
  // printf("%s\n","start validating" );
  int i;
  int total;
  char data[10240];
  strcpy(data,payload);
  // printf("%s\n",data);
  for (i = 0; i < strlen(payload); i++){
    if (data[i]=='}' || data[i] == '{') total++;
  }
  // printf("%d\n",total%2);
  if (total % 2 == 0) return SHORTN_SUCCESS;
  return SHORTN_ERROR;
}


char* base64enc(long unsigned int value)
{
    char base64[62] = "jB7D8FuHIJzLMNOPrRSbUdWXYZaTcVe3ghiAklmn4pqQstGvwxyK012fo56CE9";

    /* log(2**64) / log(64) = 12.38 => max 13 char + '\0' */
    char buffer[12];
    unsigned int offset = sizeof(buffer);

    buffer[--offset] = '\0';
    do {
        buffer[--offset] = base64[value % 62];
    } while (value /= 62);

    return strdup(&buffer[offset]);
}
