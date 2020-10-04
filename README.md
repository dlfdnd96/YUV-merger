# YUV Merger 과제
YUV tile들을 하나의 yuv파일로 합치는 프로그램
* 생성 날짜: 20.01.16
* 개발자: 류일웅
* 사용 언어: C/C++
* 개발 도구: Visual Studio Code
* 지원 OS: Windows, Linux

# 필요한 자료들
1. Python3 가상환경
2. configure 파일 및 bitstream 파일
3. HM-16.20, 360Lib 파일
4. Visual Studio 2015(윈도우일 경우)
5. YUV player(윈도우일 경우)
6. CMake
7. 컴파일
    * MinGw 컴파일(윈도우)
    * gcc 컴파일(리눅스)

# 사용 방법
1. 비트스트림 디코딩
    * Script/Tile_bitsteram_decoding_script.py 실행
2. build
    * 윈도우일 경우 build_gcc_window.bat 실행
    * 리눅스일 경우 build_gcc_linux.sh 실행
3. yuv merger
    * build/Debug/YUV-merger-0.1.out 실행(윈도우일 경우 build/Debug/YUV-merger-0.1.out.exe)

# 주의사항
* 필요한 자료들에 적혀있는 자료들 확인하기
* 스크립트나 yuv merger 실행 시 파라미터 경로 주의

# YUV Merger 명령어 실행 예
* .\build\Debug\YUV_merger-0.1.out.exe .\YUV-configure-and-tile-bitstreames\v0_4096_2048_420_10b_22_0-31_merged_cfg.cfg .\Output\yuv_merger_output.yuv(윈도우)
* /build/Debug/YUV_merger-0.1.out ./YUV-configure-and-tile-bitstreames/v0_4096_2048_420_10b_22_0-31_merged_cfg.cfg ./Output/yuv_merger_output.yuv(리눅스)

# 결과 확인 방법
* yuv player로 yuv merger 결과 파일을 확인(윈도우)
* ffmpeg 명령어 실행(리눅스)
    * 예: ffmpeg -s 4096x2048 -pix_fmt yuv420p10le - i ./Output/yuv_merger_output.yuv -vf trim=start_frame=0:end_frame=1 -f image2 ./Output/yuv_merger_output.png
    * 출처: https://blog.naver.com/uof4949/221471764086
