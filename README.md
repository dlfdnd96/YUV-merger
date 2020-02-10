# YUV Merger 과제
* 생성 날짜: 20.01.16
* 개발자: 류일웅
* 사용 언어: C++
* 개발 도구: Visual Studio Code
* 지원 OS: Windows, Linux

# 필요한 자료들
1. 파이썬 가상환경 생성 및 패키지 설정
2. configure 파일 및 bitstream 파일
3. HM-16.20, 360Lib 파일
4. visual studio 2015
5. HM-16.20 빌드
6. YUV player
7. CMake 설치 여부 확인
8. 컴파일 설치 여부
    * MinGw 컴파일(윈도우)
    * gcc 컴파일(리눅스)

# 실행 방법
## 비트스트림 디코딩 방법
* Script/Tile_bitsteram_decoding_script.py 실행
## build 방법
* 윈도우일 경우 build_gcc_window.bat 실행
* 리눅스일 경우 build_gcc_linux.sh 실행
## yuv merger 방법
* build/Debug/YUV-merger-0.1.out 실행(윈도우일 경우 build/Debug/YUV-merger-0.1.out.exe)

# 주의사항
* 필요한 자료들에 있는 자료들 확인하기
* 스크립트나 yuv merger 실행 시 파라미터 경로 주의