#include <iostream>

using namespace std;

// TODO: cfg 파일 입력받은 후 yuv파일 값을 배열로 저장하기
int main()
{
    // 파일 구조체 정의
    FILE *input_YUV_file = NULL;
    FILE *output_YUV_file = NULL;

    // YUV 파일 로드
    input_YUV_file = fopen( "./Decoding-output/test_output_5.yuv", "rb" );
    output_YUV_file = fopen( "./Decoding-output/test_yuv_merge.yuv", "wb" );

    unsigned int resolution_size = 512 * 512;
    // YUV 420 파일을 불러 올 시 resolution에서 1.5배만큼 더 불러와야 한다.
    unsigned int file_read_size = ( resolution_size + ( resolution_size / 2 ) ) * 33;
    unsigned short *read_data = new unsigned short[file_read_size];

    if ( input_YUV_file == NULL )
    {
        // 파일을 읽지 못하는 경우, 에러 처리
        fputs( "File error", stderr );
        exit(1);
    }
    else
    {
        // YUV 파일을 읽어, read_data 배열에 복사합니다.
        fread( read_data, sizeof( unsigned short ), file_read_size, input_YUV_file );
        fwrite( read_data, sizeof( unsigned short ), file_read_size, output_YUV_file );
    }

    // // YUV 파일의 픽셀 값 --> read_data[] 배열이 되어 이제 이 read_data 배열을 이용하면 됩니다.
    // for ( int i = 0; i < file_read_size; i++ )
    // {
    //     // YUV 영상의 i번째까지의 픽셀 값 정도만 확인합니다.
    //     // cout << read_data[i] << " ";
    // }

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