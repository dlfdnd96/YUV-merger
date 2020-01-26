/*
타일 비트스트림 YUV Merger 프로그램

@Author: iwryu
@Since: 20.01.21
*/
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_CONFIG_DATA_COUNT 7
#define TILE_INDEX_CONFIG_DATA_COUNT 8
#define RESOLUTION_SIZE 512 * 512

/*
매개변수 확인

@Author: iwryu
@Since: 20.01.26
@Param: argc_count
@Return: 
*/
void check_argc(int argc_count)
{
    if (argc_count == 1)
    {
        fputs("매개변수를 입력 하세요.", stderr);
        exit(1);
    }
    else if (argc_count == 2)
    {
        fputs("매개변수가 적습니다.", stderr);
        exit(1);
    }
    else if (argc_count > 3)
    {
        fputs("매개변수가 많습니다.", stderr);
        exit(1);
    }
}

/*
파일 열기오류 확인

@Author: iwryu
@Since: 20.01.26
@Param: fp
@Return: 
*/
void check_file_open_error(FILE *fp)
{
    if (fp == NULL)
    {
        fputs("File error", stderr);
        exit(1);
    }
}

/*
YUV config 기본 설정 구조체

@Author: iwryu
@Since: 20.01.26
*/
struct yuv_default_config_data
{
    char key[20];
    int value;
};

/*
YUV config 타일 인덱스 및 yuv 파일 이름 설정 구조체

@Author: iwryu
@Since: 20.01.26
*/
struct yuv_tile_index_config_data
{
    int key;
    char value[100];
};

// TODO: cfg 파일 입력받은 후 yuv파일 값을 배열로 저장하기
int main(int argc, char *argv[])
{
    check_argc(argc);

    FILE *config_file = NULL;

    config_file = fopen(argv[1], "r");
    check_file_open_error(config_file);

    // config 파일 데이터들을 struct에 저장
    struct yuv_default_config_data default_cfg_data[DEFAULT_CONFIG_DATA_COUNT];
    struct yuv_tile_index_config_data index_cfg_data[TILE_INDEX_CONFIG_DATA_COUNT];
    for (int i = 0; i < DEFAULT_CONFIG_DATA_COUNT; i++)
    {
        fscanf(config_file, "%s %d", &default_cfg_data[i].key, &default_cfg_data[i].value);
    }
    for (int i = 0; i < TILE_INDEX_CONFIG_DATA_COUNT; i++)
    {
        fscanf(config_file, "%d %s", &index_cfg_data[i].key, &index_cfg_data[i].value);
    }

    fclose(config_file);

    FILE *input_YUV_file = NULL;
    FILE *output_YUV_file = NULL;

    input_YUV_file = fopen(index_cfg_data[0].value, "rb");
    output_YUV_file = fopen(argv[2], "wb");

    // YUV 420 파일을 불러 올 시 resolution에서 1.5배만큼 더 불러와야 한다.
    unsigned int file_read_size = (RESOLUTION_SIZE + (RESOLUTION_SIZE / 2 )) * 33;
    unsigned short *read_data = new unsigned short[file_read_size];

    check_file_open_error(input_YUV_file);

    // YUV 파일을 읽어, read_data 배열에 복사합니다.
    fread(read_data, sizeof(unsigned short), file_read_size, input_YUV_file);
    fwrite(read_data, sizeof(unsigned short), file_read_size, output_YUV_file);

    fclose(input_YUV_file);
    fclose(output_YUV_file);

    delete[] read_data;
    return 0;
}

// int main()
// {
//     // 파일 구조체 정의
//     FILE *input_YUV_file = NULL;

//     // YUV 파일 로드
//     input_YUV_file = fopen( "./Decoding-output/test_output_0.yuv", "rb" );

//     unsigned int resolution_size = 512 * 512;

//     // YUV 파일의 정보를 담고 있을 배열 공간 정의
//     unsigned short *read_data = new unsigned short[resolution_size];
//     size_t n_size;

//     if ( input_YUV_file == NULL )
//     {
//         // 파일을 읽지 못하는 경우, 에러 처리
//         fputs( "File error", stderr );
//         exit(1);
//     }
//     else
//     {
//         // YUV 파일을 읽어, read_data 배열에 복사합니다. n_size는 읽은 바이트 스트림의 갯수를 의미합니다.
//         n_size = fread( read_data, sizeof( unsigned short ), resolution_size, input_YUV_file );
//     }

//     // YUV 파일의 픽셀 값 --> read_data[] 배열이 되어 이제 이 read_data 배열을 이용하면 됩니다.
//     for ( int i = 0; i < 100; i++ )
//     {
//         // YUV 영상의 i번째까지의 픽셀 값 정도만 확인합니다.
//         cout << read_data[i] << " ";
//     }

//     fclose( input_YUV_file );

//     delete[] read_data;
//     return 0;
// }