#include "secoc.h"

uint32_t freshness = 0;

int secoc_verify(secoc_pdu_t* pdu){
    // check freshness
    if(pdu->freshness <= freshness){
        return -1; // Replay
    }

    // MAC 검증 (not now)

    // freshness 갱신
    freshness = pdu->freshness;

    return 0;
}