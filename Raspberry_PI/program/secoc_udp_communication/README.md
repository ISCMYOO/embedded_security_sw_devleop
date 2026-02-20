## SecOC Freshness 기능 구현 예제 (프로그램)
해당 코드는 STM32MP157D-DK1과 라즈베리파이간의 SecOC의 Freshness 기능 구현 예제 코드입니다.

송신자 역할을 수행하는 코드로 라즈베리파이에서 동작하며 수신자 코드 및 컴파일, 구동 방법에 대한 자세한 내용은 아래 페이지를 참고해주세요. 

## 참고자료
### 코드 설명 및 컴파일, 구동 방법 (Notion)
[SecOC의 Freshness Sender 구현 (to STM32MP157D-DK1)](https://www.notion.so/iscmyoo/SecOC-Freshness-Sender-to-STM32MP157D-DK1-305a05c043f780b2bf33c9a19ce23f22?source=copy_link)

[SecOC의 AuthTag(MAC) Sender 구현 (to STM32MP157D-DK1)](https://www.notion.so/iscmyoo/SecOC-AuthTag-MAC-to-STM32MP157D-DK1-30da05c043f78009b42dc3ea11d70a01?source=copy_link)

### 수신자 코드 (Github)
[secoc_udp_receiver](https://github.com/ISCMYOO/embedded_security_sw_devleop/tree/main/AP_ECU/program/secoc_communication)