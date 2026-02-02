# 임베디드 보안 SW 개발 (AP ECU, HSM, MACSec, SecOC)

## 참고자료 (Notion)
[임베디드 보안 SW 개발](https://iscmyoo.notion.site/SW-AP-Linux-HSM-MACsec-SecOC-2efa05c043f7803e94faf355f9db0a44)

## 1. 프로젝트 소개
### 임베디드 환경에서 보안 환경 구축 및 SW 개발을 해보자! 
임베디드 환경 중 가장 많이 사용되는 AP기반 ECU 환경과 HSM 기능을 탑제한 MCU 간의 보안 환경 구축 및 기타 ECU 간의 보안 통신 환경을 구현하는 프로젝트 입니다.

### ⚙ 기술 스택
#### 🖥 AP ECU
<img alt="C" src ="https://img.shields.io/badge/C-A8B9CC.svg?&style=for-the-badge&logo=C&logoColor=white"/><img alt="ARM" src ="https://img.shields.io/badge/arm-%230091BD.svg?style=for-the-badge&logo=arm&logoColor=white"/><img alt="QEMU" src ="https://img.shields.io/badge/qemu-qemu.svg?style=for-the-badge&logo=qemu&logoColor=white"/><img alt="Yocto" src ="https://img.shields.io/badge/Yocto-7289DA.svg?style=for-the-badge&logo=yocto&logoColor=white"/><img alt="PWNGDB" src ="https://img.shields.io/badge/PwnGDB-41454A.svg?style=for-the-badge&logo=gdb&logoColor=white"/></br><img alt="STMicroelectronics" src ="https://img.shields.io/badge/STMicroelectronics-03234B.svg?style=for-the-badge&logo=stm&logoColor=white"/><img alt="OP-TEE" src ="https://img.shields.io/badge/OP_TEE-FF1B2D.svg?style=for-the-badge&logo=tee&logoColor=white"/><img alt="Linux" src ="https://img.shields.io/badge/Linux-FCC624.svg?style=for-the-badge&logo=linux&logoColor=black"/>

#### 🖥️ ECU
<img alt="C" src ="https://img.shields.io/badge/C-A8B9CC.svg?&style=for-the-badge&logo=C&logoColor=white"/><img alt="Raspberrypi" src ="https://img.shields.io/badge/-Raspberry_Pi-C51A4A?style=for-the-badge&logo=Raspberry-Pi"/>

#### ⚙️ HSM
<img alt="C" src ="https://img.shields.io/badge/C-A8B9CC.svg?&style=for-the-badge&logo=C&logoColor=white"/><img alt="STMicroelectronics" src ="https://img.shields.io/badge/STMicroelectronics-03234B.svg?style=for-the-badge&logo=stm&logoColor=white"/>

#### 📄 내용 정리
<img alt="GITHUB" src ="https://img.shields.io/badge/github-%23121011.svg?style=for-the-badge&logo=github&logoColor=white"/><img alt="Notion" src ="https://img.shields.io/badge/Notion-%23000000.svg?style=for-the-badge&logo=notion&logoColor=white"/>

### 서비스 구조
<img width="1338" height="630" alt="Image" src="https://github.com/user-attachments/assets/bf4640c2-c469-4fec-9b3e-03714c03ce86" />

### 개발 보드
#### 🖥 AP ECU
##### STM32MP15D DK1
<img width="250" height="210" src = "https://github.com/user-attachments/assets/54b9a3fe-5973-465c-ae73-14cdcd2b911d">

#### 🖥️ ECU
##### Raspberrypi4
<img width="250" height="210" src = "https://github.com/user-attachments/assets/3240b77a-616f-4959-a5d2-08fe968432e5">

#### ⚙️ HSM
##### NUCLEO-144 STM32L4R5ZI
<img width="250" height="210" src = "https://github.com/user-attachments/assets/3390f886-6906-44dd-8fc2-d8cde86016e2">