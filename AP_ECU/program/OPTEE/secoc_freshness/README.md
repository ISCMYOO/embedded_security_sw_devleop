## SecOC Freshness with OP-TEE 기능 구현 예제 (프로그램)
해당 코드는 STM32MP157D-DK1과 라즈베리파이간의 SecOC 통신 중 OP-TEE 환경에서의 Freshness 기능 구현 예제 코드입니다.

기존 Freshness 예제 코드에서 OP-TEE 환경을 고려하여 추가 구현한 예제 코드입니다. ([기존 예제 코드](https://github.com/ISCMYOO/embedded_security_sw_devleop/tree/main/AP_ECU/program/secoc_communication))


수신자 역할을 수행하는 코드로 STM32MP157D-DK1에서 동작하며 송신자 코드 및 컴파일, 구동 방법에 대한 자세한 내용은 아래 링크를 참고해주세요. 

## 참고자료
### 코드 설명 및 컴파일, 구동 방법 (Notion)
[SecOC의 OP-TEE Freshness 구현 (to 라즈베리파이) ](https://www.notion.so/iscmyoo/SecOC-OP-TEE-Freshness-to-305a05c043f7807e8006cd82da11f3ac?v=2efa05c043f780a1bcf3000ccd297f53&source=copy_link)

### 수신자 코드 (Github)
[secoc_udp_sender](https://github.com/ISCMYOO/embedded_security_sw_devleop/tree/main/Raspberry_PI/program/secoc_udp_communication)