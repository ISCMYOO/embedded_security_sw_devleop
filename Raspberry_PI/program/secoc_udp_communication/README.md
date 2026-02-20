## SecOC 기능 구현 예제 (프로그램)
해당 코드는 STM32MP157D-DK1과 라즈베리파이간의 SecOC 기능 구현 예제 코드입니다.

송신자 역할을 수행하는 코드로 라즈베리파이에서 동작하며 수신자 코드 및 컴파일, 구동 방법에 대한 자세한 내용은 아래 페이지를 참고해주세요. 

## 빌드
```
# 소스코드 다운 및 이동
git clone https://github.com/ISCMYOO/embedded_security_sw_devleop.git
cd ./Raspberry_PI/program/secoc_udp_communication/normal

# 필수 파일 생성
mkdir -p ./var/lib/secoc

touch ./var/lib/secoc/key.bin
touch ./var/lib/secoc/freshness.bin

# 빌드
gcc secoc_udp_sender.c udpcom.c common.c secoc.c -o secoc_udp_sender

# 실행
./secoc_udp_sender

```

## 기능설명
| 서비스 이름 | 기능 설명|
| :-: | :-: |
| Reset Freshness | 라즈베리파이의 Freshness와 STM32MP157D-DK1의 Freshness를 모두 0으로 초기화 |
| Send Message | 라즈베리파이에서 STM32MP157D-DK1으로 입력한 문자열을 전달 |
| Replay Packet | Freshness 값을 변조하여 이전 패킷을 재사용 하도록 설정 후 전송 |
| Read Freshness | 라즈베리파이와 STM32MP157D-DK1에 존재하는 Freshness 값을 읽기 |
| Set Freshness | 라즈베리파이와 STM32MP157D-DK1의 Freshness을 입력 값으로 설정 |
| Read Key | 라즈베리파이와 STM32MP157D-DK1에 설정되어 있는 Key 읽기 |
| Sync Key | STM32MP157D-DK1의 Key 값을 라즈베리파이에서 설정되어 있는 Key 값으로 설정 |
| Gen & Sync Key | 라즈베리파이에서 Key 값을 랜덤으로 설정 후 STM32MP157D-DK1도 동일하게 설정 | 


## 참고자료
### 코드 설명 및 컴파일, 구동 방법 (Notion)
[SecOC의 Freshness Sender 구현 (to STM32MP157D-DK1)](https://www.notion.so/iscmyoo/SecOC-Freshness-Sender-to-STM32MP157D-DK1-305a05c043f780b2bf33c9a19ce23f22?source=copy_link)

[SecOC의 AuthTag(MAC) Sender 구현 (to STM32MP157D-DK1)](https://www.notion.so/iscmyoo/SecOC-AuthTag-MAC-to-STM32MP157D-DK1-30da05c043f78009b42dc3ea11d70a01?source=copy_link)

### 수신자 코드 (Github)
[secoc_udp_receiver](https://github.com/ISCMYOO/embedded_security_sw_devleop/tree/main/AP_ECU/program/secoc_communication)