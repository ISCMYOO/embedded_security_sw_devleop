#ifndef TAAPI_H
#define TAAPI_H

#include <tee_client_api.h>
#include "../include/common.h"
#include "common.h"

typedef struct{
    TEEC_Context ctx;
    TEEC_Session session;
} TeeSecOC;

int init(TeeSecOC* secoc_obj);

int store_freshness(TeeSecOC* secoc_obj, const char* alias, const uint32_t freshness);

TEEC_Result load_freshness(TeeSecOC* secoc_obj, const char* alias, uint32_t* freshness);

int delete_freshness(TeeSecOC* secoc_obj, const char* alias);

int loadOrGenFreshness(TeeSecOC* secoc_obj, const char* alias, uint32_t* freshness);

#endif