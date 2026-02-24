#include "secoc.h"

int secoc_verify(TeeSecOC* secoc_obj, secoc_pdu_t* pdu, const char* alias){
    // check freshness
    if(!check_freshness(secoc_obj, alias, pdu->freshness)){
        return -1; // Replay
    }

    // MAC 검증

    return 0;
}

int secoc_load_freshness(TeeSecOC* secoc_obj, const char* alias){
    // load freshness from RPMB(eMMC)
    if(load_freshness(secoc_obj, alias) == -1)
        return -1;
    return 0;
}

int secoc_read_freshness(TeeSecOC* secoc_obj, const char* alias){
    read_freshness(secoc_obj, alias);
    return 0;
}

int secoc_store_freshness(TeeSecOC* secoc_obj, const char* alias, const uint32_t freshness_in){
    if(store_freshness(secoc_obj, alias, freshness_in) == -1)
        return -1;
    return 0;
}

int secoc_delete_freshness(TeeSecOC* secoc_obj, const char* alias){
    if(delete_freshness(secoc_obj, alias) == -1)
        return -1;
    return 0;
}