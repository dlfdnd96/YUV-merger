/*
타일 비트스트림 YUV Merger 프로그램

@Author: iwryu
@Since: 20.01.21
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CONFIG_DATA_COUNT 7
#define TILE_INDEX_CONFIG_DATA_COUNT 8
#define RESOLUTION_WIDTH 512
#define RESOLUTION_HEIGHT 512
#define OUTPUT_RESOLUTION_WIDTH 4096
#define OUTPUT_RESOLUTION_HEIGHT 2048

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
YUV 파일을 읽어 배열에 복사

@Author: iwryu
@Since: 20.01.28
@Param: input_YUV_file, index_cfg_data, read_data, file_read_size
@Return: read_data
*/
unsigned short* read_yuv_data(FILE *input_YUV_file, struct yuv_tile_index_config_data *index_cfg_data, unsigned short *read_data, const int file_read_size)
{
    input_YUV_file = fopen(index_cfg_data[0].value, "rb");
    check_file_open_error(input_YUV_file);

    // YUV 파일을 읽어, read_data 배열에 복사합니다.
    fread(read_data, sizeof(unsigned short), file_read_size, input_YUV_file);

    fclose(input_YUV_file);

    return read_data;
}

/*
Y, U, V 값 분리

@Author: iwryu
@Since: 20.01.28
@Param: y_size, uv_size, YUV_data, read_data, u_data
@Return: YUV_data
*/
unsigned short* split_yuv(const int y_size, const int uv_size, unsigned short *YUV_data, unsigned short *read_data, bool u_data)
{
    if (!uv_size)
    {
        for (int i = 0; i < y_size; i++)
        {
            YUV_data[i] = read_data[i];
        }
    }
    else
    {
        if (u_data)
        {
            for (int i = 0; i < y_size; i++)
            {
                YUV_data[i] = read_data[y_size + i];
            }
        }
        else
        {
            for (int i = 0; i < y_size; i++)
            {
                YUV_data[i] = read_data[y_size + uv_size + i];
            }
        }
        
    }

    return YUV_data;
}

/*
최종 YUV 파일 초기화

@Author: iwryu
@Since: 20.01.28
@Param: total_write_size, total_data
@Return: total_data
*/
unsigned short* init_yuv_merge(const int total_write_size, unsigned short *total_data)
{
    for (int i = 0; i < total_write_size; i++)
    {
        total_data[i] = 0;
    }

    return total_data;
}

/*
YUV Merge

@Author: iwryu
@Since: 20.01.28
@Param: out_yuv, YUV_data, y_data
@Return: out_yuv
*/
unsigned short* execute_yuv_merge(unsigned short *out_yuv, unsigned short *YUV_data, bool y_data)
{
    if (y_data)
    {
        for (int i = 0; i < RESOLUTION_HEIGHT; i++)
        {
            for (int j = 0; j < RESOLUTION_WIDTH; j++)
            {
                out_yuv[(OUTPUT_RESOLUTION_WIDTH * i) + j] = YUV_data[(RESOLUTION_WIDTH * i) + j];
            }
        }
    }
    else
    {
        for (int i = 0; i < RESOLUTION_HEIGHT / 2; i++)
        {
            for (int j = 0; j < RESOLUTION_WIDTH / 2; j++)
            {
                out_yuv[(OUTPUT_RESOLUTION_WIDTH * i / 2) + j] = YUV_data[(RESOLUTION_WIDTH * i / 2) + j];
            }
        }
    }
    
    return out_yuv;
}

/*
YUV Merge한 결과를 파일에 저장

@Author: iwryu
@Since: 20.01.28
@Param: 
@Return: 
*/
void write_yuv_merge_file(FILE *output_YUV_file, const char *argv, unsigned short* total_data, const int total_write_size)
{
    output_YUV_file = fopen(argv, "wb");
    fwrite(total_data, sizeof(unsigned short), total_write_size, output_YUV_file);

    fclose(output_YUV_file);
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

// TODO: option 라이브러리 쓰기
// TODO: 함수화 하기
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

    // TODO: 주소값으로 계산하기
    // YUV 420 파일을 불러올 시 resolution에서 1.5배만큼 더 불러와야 한다.
    unsigned int y_size = RESOLUTION_WIDTH * RESOLUTION_HEIGHT;
    unsigned int uv_size = y_size / 4;
    unsigned int file_read_size = (y_size + (y_size / 2));
    unsigned short *read_data = new unsigned short[file_read_size];
    FILE *input_YUV_file = NULL;
    
    read_data = read_yuv_data(input_YUV_file, index_cfg_data, read_data, file_read_size);

    unsigned short *Y_data = new unsigned short[y_size];
    unsigned short *U_data = new unsigned short[uv_size];
    unsigned short *V_data = new unsigned short[uv_size];

    // Y, U, V 값 분리
    Y_data = split_yuv(y_size, 0, Y_data, read_data, false);
    U_data = split_yuv(y_size, uv_size, U_data, read_data, false);
    V_data = split_yuv(y_size, uv_size, V_data, read_data, true);
    // for (int i = 0; i < y_size; i++)
    // {
    //     Y_data[i] = read_data[i];
    // }
    // for (int i = 0; i < uv_size; i++)
    // {
    //     U_data[i] = read_data[y_size + i];
    //     V_data[i] = read_data[y_size + uv_size + i];
    // }

    unsigned int output_y_size = OUTPUT_RESOLUTION_WIDTH * OUTPUT_RESOLUTION_HEIGHT;
    unsigned int output_uv_size = output_y_size / 4;
    unsigned int total_write_size = (output_y_size + (output_y_size / 2));
    unsigned short *total_data = new unsigned short[total_write_size];

    // 최종 YUV 파일 초기화
    // for (int i = 0; i < total_write_size; i++)
    // {
    //     total_data[i] = 256;
    // }
    total_data = init_yuv_merge(total_write_size, total_data);

    unsigned short *pOutY = &total_data[0];
    unsigned short *pOutU = &total_data[output_y_size];
    unsigned short *pOutV = &total_data[output_y_size + output_uv_size];

    // 여러개의 YUV 파일을 하나의 최종 YUV 파일로 합치기
    // for (int i = 0; i < RESOLUTION_HEIGHT; i++)
    // {
    //     for (int j = 0; j < RESOLUTION_WIDTH; j++)
    //     {
    //         pOutY[(OUTPUT_RESOLUTION_WIDTH * i    ) + j] = Y_data[(RESOLUTION_WIDTH * i    ) + j];
    //     }
    // }

    // for (int i = 0; i < RESOLUTION_HEIGHT / 2; i++)
    // {
    //     for (int j = 0; j < RESOLUTION_WIDTH / 2; j++)
    //     {
    //         pOutU[(OUTPUT_RESOLUTION_WIDTH * i / 2) + j] = U_data[(RESOLUTION_WIDTH * i / 2) + j];
    //         pOutV[(OUTPUT_RESOLUTION_WIDTH * i / 2) + j] = V_data[(RESOLUTION_WIDTH * i / 2) + j];
    //     }
    // }
    pOutY = execute_yuv_merge(pOutY, Y_data, true);
    pOutU = execute_yuv_merge(pOutU, U_data, false);
    pOutV = execute_yuv_merge(pOutV, V_data, false);

    FILE *output_YUV_file = NULL;
    // output_YUV_file = fopen(argv[2], "wb");
    // fwrite(total_data, sizeof(unsigned short), total_write_size, output_YUV_file);

    // fclose(output_YUV_file);
    write_yuv_merge_file(output_YUV_file, argv[2], total_data, total_write_size);

    delete[] total_data;
    delete[] V_data;
    delete[] U_data;
    delete[] Y_data;
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