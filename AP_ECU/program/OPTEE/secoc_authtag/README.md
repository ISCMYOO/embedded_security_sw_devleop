## SecOC freshness and authtag(MAC) with OP-TEE 기능 구현 예제 (프로그램)
해당 코드는 STM32MP157D-DK1과 라즈베리파이간의 SecOC 통신 중 OP-TEE 환경에서의 Freshness와 AuthTag(MAC) 기능 구현 예제 코드입니다.

기존 OP_TEE 환경에서의 Freshness 예제 코드에서 AuthTag(MAC) 기능을 추가 구현한 예제 코드입니다. ([기존 예제 코드](https://github.com/ISCMYOO/embedded_security_sw_devleop/tree/main/AP_ECU/program/OPTEE/secoc_freshness))


수신자 역할을 수행하는 코드로 STM32MP157D-DK1에서 동작하며 송신자 코드 및 컴파일, 구동 방법에 대한 자세한 내용은 아래 링크를 참고해주세요. 

## 참고자료
### 코드 설명 및 컴파일, 구동 방법 (Notion)
[SecOC의 AuthTag(MAC) 구현 (to 라즈베리파이)](https://www.notion.so/iscmyoo/SecOC-AuthTag-MAC-to-304a05c043f7805bbefce01cf21cb394?source=copy_link)

### 수신자 코드 (Github)
[secoc_udp_sender](https://github.com/ISCMYOO/embedded_security_sw_devleop/tree/main/Raspberry_PI/program/secoc_udp_communication)