
/*
 * Copyright (c) 2017 Arif Nur Khoirudin
 *
 * mail : <hello@arifnurkhoirudin.com>
 * site : https://arifnurkhoirudin.com
 *
 * Don't redistribute or copying without owner permission
*/

#include "../libs/general.h"

static void short_random_cb(struct evhttp_request *req, void *arg);
static void short_custom_cb(struct evhttp_request *req, void *arg);
static void redirector_cb(struct evhttp_request *req, void *arg);
