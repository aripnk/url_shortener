
/*
 * Copyright (c) 2017 Arif Nur Khoirudin
 *
 * mail : <hello@arifnurkhoirudin.com>
 * site : https://arifnurkhoirudin.com
 *
 * Don't redistribute or copying without owner permission
*/

#include "shortensvc.h"

void usage(void){
	printf("Usage:\n");
	printf(" -h<host>\n");
	printf(" -p<port>\n");
	printf(" -n<name>\n");
	printf(" -i<index.html>\n");
	printf("Example : ./shortn -h 0.0.0.0 -p 10000 -n shortn1 -i src/html/index.html\n");
	exit (8);
}

/*===============================================================*/
/*========================MAIN FUNCTION==========================*/

int main(int argc, char **argv){
	char host[24];
	int port;
	char name[24];
	char indexhtml[24];
	if (argc < 4){
		usage();
	}

	int opt;
	while ((opt = getopt (argc, argv, "h:p:n:i:")) != -1)
	{
		switch (opt)
		{
			case 'h':
				sprintf(host, "%s", optarg);
				printf ("Host: \"%s\"\n", optarg);
				break;
			case 'p':
				port = atoi(optarg);
				printf ("Port: \"%s\"\n", optarg);
				break;
			case 'n':
				sprintf(name, "%s", optarg);
				printf ("Name: \"%s\"\n", optarg);
				break;
			case 'i':
				sprintf(indexhtml, "%s", optarg);
				printf ("Index: \"%s\"\n", optarg);
		}
	}


	//for pidfile
	char namepid[24];
	sprintf(namepid, "%s.pid",name);
	FILE *fp;
	fp = fopen (namepid,"w");
	fprintf(fp, "%d\n",getpid()+1);
	fclose(fp);

	FILE *fl;
	int ch = 0;
	char html[4089];
	memset(html, 0, 4089);
	int len=0;
	fl=fopen(indexhtml,"r");
  if(!fl) return -1;
  while(ch != EOF){
    ch=fgetc(fl);
		html[len]=ch;
		len++;


	}
	html[len-1] = 0x00;
	fclose(fl);

	printf("%s\n", html);


	struct event_base *base;
	struct evhttp *http;
	struct evhttp_bound_socket *handle;

	base = event_base_new();
	if (!base) {
		fprintf(stderr, "Couldn't create an event_base: exiting\n");
		return 1;
	}

	http = evhttp_new(base);
	if (!http) {
		fprintf(stderr, "couldn't create evhttp. Exiting.\n");
		return 1;
	}


	shortn_t shortn;
	shortn_init(&shortn);

	memset(shortn.index, 0, 4089);
	// sprintf(shortn.index, "%s", html);
	strncpy(shortn.index, html, strlen(html));

	evhttp_set_cb(http, "/short_random", short_random_cb, (void*)&shortn); // untuk random url
  evhttp_set_cb(http, "/short_custom", short_custom_cb, (void*)&shortn); // untuk custom url
	evhttp_set_gencb (http, redirector_cb, (void*)&shortn); //redirector

	printf("%s -- %d\n",host, port);
	handle = evhttp_bind_socket_with_handle(http, host, port);
	if (!handle) {
		fprintf(stderr, "couldn't bind to port %d. Exiting.\n",
		    port);
		return 1;
	}

	printf("Program Starting with PID = %d\n",getpid());
	printf("Writing PID file %s.pid ... \n",name );

	event_base_dispatch(base);

	shortn_free(&shortn);
	return 0;
}


/*===============================================================*/
/*========================CALLBACK FUNCTION==========================*/
static void short_random_cb(struct evhttp_request *req, void *arg){
	printf("%s\n","random called" );
	struct evbuffer *evb = NULL;
	char answer[1024];

  const char *uri = evhttp_request_get_uri (req);
  struct evhttp_uri *decoded = NULL;
  decoded = evhttp_uri_parse (uri);
	char* decoded_uri = evhttp_decode_uri(uri);

  struct evkeyvalq kv;
  memset (&kv, 0, sizeof (kv));
  struct evbuffer *buf = evhttp_request_get_input_buffer (req);
  evbuffer_add (buf, "", 1);    /* NUL-terminate the buffer */
  char *payload = (char *) evbuffer_pullup (buf, -1);

  //can't decode URI
  if (! decoded){
		evhttp_send_error (req, HTTP_BADREQUEST, 0);
		return;
	}

	//malformed json
	if (shortn_validate_payload(payload) != SHORTN_SUCCESS){\
			evhttp_send_error (req, HTTP_BADREQUEST, 0);
			return;
	}

  //not POST request
  if (evhttp_request_get_command(req) != EVHTTP_REQ_POST){
		evhttp_send_error (req, HTTP_NOTFOUND, 0);
		return;
  }


	shortn_t* shortn = (shortn_t*)arg;

	char* url = shortn_find_payload(shortn, payload, "url");

	if (url == NULL || strlen(url) < 4){
		evhttp_send_error (req, HTTP_BADREQUEST, 0);
		return;
	}


	shortn_assign_counter(shortn);
	shortn_getrandomurl(shortn,url);

	//get data
	int valid = shortn_write(shortn);

  if (valid == SHORTN_SUCCESS){
		char shorted[24];
		memcpy(shorted,shortn->shortn,shortn->shortnsize);
    evutil_snprintf (answer,1024,"{\"status\":%d, \"url\":\"%s\"}",valid,shorted);
  }else{
    evutil_snprintf (answer,1024,"{\"status\":1, \"url\":\"null\"}");
  }

  evhttp_clear_headers (&kv);
  evb = evbuffer_new ();

  evbuffer_add_printf(evb, "%s",answer);

  evhttp_add_header (evhttp_request_get_output_headers (req), "Access-Control-Allow-Origin", "*");
  evhttp_add_header (evhttp_request_get_output_headers (req), "Content-Type", "application/json;");

  evhttp_send_reply(req, 200, "OK", evb);
  evhttp_uri_free (decoded);
	free(url);
  if (evb) evbuffer_free (evb);
}




static void short_custom_cb(struct evhttp_request *req, void *arg){
	struct evbuffer *evb = NULL;
	char answer[1024];

  const char *uri = evhttp_request_get_uri (req);
  struct evhttp_uri *decoded = NULL;
  decoded = evhttp_uri_parse (uri);
	char* decoded_uri = evhttp_decode_uri(uri);

  struct evkeyvalq kv;
  memset (&kv, 0, sizeof (kv));
  struct evbuffer *buf = evhttp_request_get_input_buffer (req);
  evbuffer_add (buf, "", 1);    /* NUL-terminate the buffer */
  char *payload = (char *) evbuffer_pullup (buf, -1);

  //can't decode URI
  if (! decoded){
		evhttp_send_error (req, HTTP_BADREQUEST, 0);
		return;
	}
  //malformed payload
  // if (0 != evhttp_parse_query_str (payload, &kv)){
	// 	evhttp_send_error (req, HTTP_BADREQUEST, 0);
	// 	return;
	// }

	//malformed json
	if (shortn_validate_payload(payload) != SHORTN_SUCCESS){
			evhttp_send_error (req, HTTP_BADREQUEST, 0);
			return;
	}

  //not POST request
  if (evhttp_request_get_command(req) != EVHTTP_REQ_POST){
		evhttp_send_error (req, HTTP_NOTFOUND, 0);
		return;
  }


	shortn_t* shortn = (shortn_t*)arg;

	char* url;
	char* custom;

	url = shortn_find_payload(shortn, payload, "url");
	custom = shortn_find_payload(shortn, payload, "custom");


	if (url == NULL || strlen(url) < 4){
		evhttp_send_error (req, HTTP_BADREQUEST, 0);
		return;
	}

	if (shortn_getcustomurl(shortn,url,custom) != SHORTN_SUCCESS){
		evutil_snprintf (answer,1024,"{\"status\":1, \"url\":\"null\"}");
	}else{
		//get data
		int valid = shortn_write(shortn);

		if (valid == SHORTN_SUCCESS){
			// char shorted[MAX_SHORTN_LENGTH];
			// strncpy(shorted,shortn->shortn,shortn->shortnlen);
			evutil_snprintf (answer,1024,"{\"status\":%d, \"url\":\"%s\"}",valid,custom);
		}else{
			evutil_snprintf (answer,1024,"{\"status\":1, \"url\":\"null\"}");
		}
	}


  evhttp_clear_headers (&kv);
  evb = evbuffer_new ();

  evbuffer_add_printf(evb, "%s",answer);
  evhttp_add_header (evhttp_request_get_output_headers (req), "Access-Control-Allow-Origin", "*");
  evhttp_add_header (evhttp_request_get_output_headers (req), "Content-Type", "application/json;");

  evhttp_send_reply(req, 200, "OK", evb);
  evhttp_uri_free (decoded);
	free(url);
	free(custom);
  if (evb) evbuffer_free (evb);
}

static void redirector_cb(struct evhttp_request *req, void *arg){
	shortn_t* shortn = (shortn_t*)arg;
	struct evbuffer *evb = NULL;
	// char answer[1024000];
	const char *uri = evhttp_request_get_uri (req);
	printf("%s\n",uri);
	if (strlen(uri)<=1){
		evb = evbuffer_new ();
	  evbuffer_add_printf(evb, "%s",shortn->index);
		evhttp_add_header (evhttp_request_get_output_headers (req), "Content-Type", "text/html");
	  evhttp_send_reply(req, 200, "OK", evb);
	  if (evb) evbuffer_free (evb);
		return;
	}

	evhttp_clear_headers (evhttp_request_get_output_headers (req));
	if (shortn_redirect(shortn, uri) == SHORTN_SUCCESS){
		if (strstr(uri,".") != NULL){
			evb = evbuffer_new ();
		  evbuffer_add_printf(evb, "%s",shortn->url);
			evhttp_add_header (evhttp_request_get_output_headers (req), "Content-Type", "text/html");
		  evhttp_send_reply(req, 200, "OK", evb);
		  if (evb) evbuffer_free (evb);
			return;
		}
		evhttp_add_header (evhttp_request_get_output_headers (req), "Content-Type", "text/html;");
		evhttp_add_header (evhttp_request_get_output_headers (req), "Location", shortn->url);
		evhttp_send_reply(req, HTTP_MOVEPERM, "HTTP/1.1 301 Moved Permanently", NULL);
	}else{
		evhttp_add_header (evhttp_request_get_output_headers (req), "Content-Type", "text/html");
		evhttp_add_header (evhttp_request_get_output_headers (req), "Location", "https://di.upil.in/404.html");
		evhttp_send_reply(req, HTTP_MOVEPERM, "HTTP/1.1 301 Moved Permanently", NULL);
	}

	if (evb) evbuffer_free (evb);
}
