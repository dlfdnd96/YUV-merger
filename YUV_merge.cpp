#include <iostream>

using namespace std;

int main()
{
    // 파일 구조체 정의
    FILE *pFile = NULL;

    // YUV 파일 로드
    pFile = fopen( "../../Decode-output/test_output_0.yuv", "rb" );

    unsigned int resolution_size = 512 * 512;
    unsigned int u_size = resolution_size / 4;
    unsigned int v_size = resolution_size / 4;
    // YUV 파일의 정보를 담고 있을 배열 공간 정의
    // unsigned short *read_data = new unsigned short[resolution_size];
    // YUV 420 파일을 불러 올 시 resolution에서 1.5배만큼 더 불러와야 한다.
    unsigned short *read_data = new unsigned short[resolution_size + (1 / 2)];
    unsigned short *y_value = new unsigned short[resolution_size];
    unsigned short *u_value = new unsigned short[u_size];
    unsigned short *v_value = new unsigned short[v_size];
    size_t n_size;

    if ( pFile == NULL )
    {
        // 파일을 읽지 못하는 경우, 에러 처리
        fputs( "File error", stderr );
        exit(1);
    }
    else
    {
        // YUV 파일을 읽어, read_data 배열에 복사합니다. n_size는 읽은 바이트 스트림의 갯수를 의미합니다.
        // n_size = fread( read_data, sizeof( unsigned short ), resolution_size, pFile );

        fread( read_data, sizeof( unsigned short ), resolution_size + u_size + v_size, pFile );
    }

    cout << "read data[] = " << read_data[resolution_size-2] << endl;

    // YUV 파일의 픽셀 값 --> read_data[] 배열이 되어 이제 이 read_data 배열을 이용하면 됩니다.
    for ( int i = 0; i < 1; i++ )
    {
        // YUV 영상의 i번째까지의 픽셀 값 정도만 확인합니다.
        // cout << read_data[i] << " ";
    }

    fclose( pFile );
    return 0;
}