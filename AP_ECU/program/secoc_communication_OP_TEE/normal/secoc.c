#include "secoc.h"

uint32_t freshness = 0;

int secoc_verify(TeeSecOC* secoc_obj, secoc_pdu_t* pdu, const char* alias){
    // check freshness
    if(pdu->freshness <= freshness){
        return -1; // Replay
    }

    // MAC 검증

    // 전역변수 freshness 갱신 
    freshness = pdu->freshness;

    // 마지막에 freshness 저장 조건 확인 후 저장하기, 10의 배수마다 저장
    if(freshness % 10 == 0 && store_freshness(secoc_obj, alias, freshness) == -1){
        printf("falied store freshness\n");
        return -1;
    }

    return 0;
}

int secoc_load_freshness(TeeSecOC* secoc_obj, const char* alias){
    uint32_t tmp_freshness;
    // load freshness from RPMB(eMMC)
    if(loadOrGenFreshness(secoc_obj, alias, &tmp_freshness) == -1)
        return -1;
    
    freshness = tmp_freshness;

    return 0;
}

int secoc_read_freshness(TeeSecOC* secoc_obj, const char* alias){
    uint32_t ta_freshness;

    if(loadOrGenFreshness(secoc_obj, alias, &ta_freshness) == -1)
        return -1;
    
    printf("RAM Freshness : %d / TA Freshness : %d\n", freshness, ta_freshness);

    return 0;
}

int secoc_store_freshness(TeeSecOC* secoc_obj, const char* alias, const uint32_t freshness_in){
    freshness = freshness_in;

    if(store_freshness(secoc_obj, alias, freshness_in) == -1)
        return -1;
    return 0;
}

int secoc_delete_freshness(TeeSecOC* secoc_obj, const char* alias){
    if(delete_freshness(secoc_obj, alias) == -1)
        return -1;
    
    freshness = 0;

    return 0;
}