# coding:utf-8

"""
Tile bitstream decoding 스크립트

@Author: iwryu
@Since: 20.01.21
"""
import sys
import os
import subprocess

bitstream_file_name = ('v0_4096_2048_420_10b_22_0-32_tile4.265', 'v0_4096_2048_420_10b_22_0-32_tile11.265',
'v0_4096_2048_420_10b_22_0-32_tile12.265', 'v0_4096_2048_420_10b_22_0-32_tile13.265', 'v0_4096_2048_420_10b_22_0-32_tile19.265',
'v0_4096_2048_420_10b_22_0-32_tile20.265', 'v0_4096_2048_420_10b_22_0-32_tile21.265', 'v0_4096_2048_420_10b_22_0-32_tile28.265')
tile_index = ('04', '11', '12', '13', '19', '20', '21', '28')
TAppDecoder_path = ""
bitstream_path = ""
output_path = ""

"""
==============================
초기화면 메세지 표시

@Author: iwryu
@Since: 20.01.21
@param: 
@return: 
==============================
"""
def init_show_message():
    init_script_message = """
    이 스크립트는 타일비트스트림 decoding을 편리하게 하기 위한 스크립트입니다.
    스크립트를 실행하기 전에 HM-16.20이 build 되어있는지 확인하시길 바랍니다.
    또한, 폴더명에 공백과 같은 특수문자가 있는지 확인하시길 바랍니다.
    모든 경로는 절대경로로 입력 해 주세요.
    """

    print(init_script_message)

"""
==============================
decoding을 하기 위한 경로 환경설정

@Author: iwryu
@Since: 20.01.21
@param: 
@return: 
==============================
"""
def set_decode_environment():
    decoder_path = input("HM decoder 경로를 입력하세요: ")
    bitstream_path = input("bitstream 경로를 입력하세요: ")
    output_path = input("YUV 결과파일 저장 경로를 입력하세요: ")

    check_path_space(decoder_path, bitstream_path, output_path)
    check_existing_decoder(decoder_path)
    check_existing_bitstream_file(bitstream_path)

    return decoder_path, bitstream_path, output_path

"""
==============================
사용자가 입력한 경로가 유효한지 검사

@Author: iwryu
@Since: 20.01.21
@param: decoder, bitstream, output
@return: 
==============================
"""
def check_path_space(decoder, bitstream, output):
    error_message = "누락된 경로가 있습니다."

    if decoder is "" or bitstream is "" or output is "":
        print(error_message)
        sys.exit(1)

"""
==============================
사용자가 입력한 경로에 TAppDecoder.exe가 있는지 확인

@Author: iwryu
@Since: 20.01.21
@param: decoder
@return: 
==============================
"""
def check_existing_decoder(decoder):
    decoder_file_name = 'TAppDecoder.exe'
    error_message = "TappDecoder.exe를 찾을 수 없습니다. 파일의 누락 혹은 경로가 맞는지 확인하세요."

    os.chdir(decoder)
    if os.path.isfile(decoder_file_name) is False:
        print(error_message)
        sys.exit(1)

"""
==============================
bitstream 파일이 존재하는지 검사

@Author: iwryu
@Since: 20.01.21
@param: bitstream
@return: 
==============================
"""
def check_existing_bitstream_file(bitstream):
    error_message = "비트스트림 파일을 찾을 수 없습니다. 파일의 누락 혹은 경로가 맞는지 확인하세요."

    os.chdir(bitstream)
    for file in bitstream_file_name:
        if os.path.isfile(file) is False:
            print(error_message)
            sys.exit(1)

"""
==============================
decoding 수행

@Author: iwryu
@Since: 20.01.21
@param: decoder, bitstream, output
@return: 
==============================
"""
def execute_decoding(decoder, bitstream, output):
    procs = []

    os.chdir(decoder)
    for index, file_name in enumerate(bitstream_file_name):
        proc = run_TAppDecoder(bitstream, output, file_name, index)
        procs.append(proc)
    
    # communicate()로 자식 프로세스들이 I/O를 마치고 종료하기를 기다림
    for proc in procs:
        proc.communicate()

    print("decoding 완료")
    sys.exit(0)


"""
==============================
decoding 과정을 자식 자식프로세스로 생성

@Author: iwryu
@Since: 20.01.21
@param: bitstream, output, file_name, index
@return: 
==============================
"""
def run_TAppDecoder(bitstream, output, file_name, index):
    subprocess_args = "TAppDecoder.exe -b " + bitstream + "\\" + file_name + " -o " + output + r"\v0_4096_2048_420_10b_22_0-32_tile" + str(tile_index[index]) + ".yuv"
    proc = subprocess.Popen(subprocess_args, shell=True)
    return proc

if __name__ == '__main__':
    init_show_message()
    TAppDecoder_path, bitstream_path, output_path = set_decode_environment()
    execute_decoding(TAppDecoder_path, bitstream_path, output_path)
