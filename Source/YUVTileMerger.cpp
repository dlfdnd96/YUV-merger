/*
타일 비트스트림 YUV Merger 프로그램

@Author: iwryu
@Since: 20.01.21
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <fstream>
#include <stdexcept>
#include <time.h>

#include "ConfigParser.h"
#include "ConfigFileType.h"

/*
config 파일 parser class

@Author: 장형(?)
@Post date: 19.05.26
*/
CConfigParser::CConfigParser(std::string path)
{
	// read File
	std::ifstream openFile(path);

	if (openFile.is_open())
    {
		std::string line;

		while (getline(openFile, line))
        {
			std::string delimiter = " ";
            // token is "scott"
			std::string token1 = line.substr(0, line.find(delimiter));
            // token is "scott"
			std::string token2 = line.substr(
                line.find(delimiter) + delimiter.length(), line.length());
			m_table[token1] = token2;
		}

		openFile.close();
	}
    else
    {
        fputs("config 파일에 내용이 없습니다.", stderr);
    }
}

/*
config 파일과 입력한 문자열이 일치하는지 비교

@Author: 장형(?)
@Post date: 19.05.26
@Param: name
@Return: bool
*/
bool CConfigParser::Contain(std::string name)
{
	if (m_table.find(name) == m_table.end())
    {
        return false;
    }

	return true;
}

/*
입력값과 일치하는 string값 반환

@Author: 장형(?)
@Post date: 19.05.26
@Param: name
@Return: m_table[name]
*/
std::string CConfigParser::GetString(std::string name)
{
	if (Contain(name))
    {
		if (m_table[name].find("\"") == std::string::npos)
        {
            return m_table[name];
        }
		else
        {
            return m_table[name].substr(1, m_table[name].length() - 2);
        }
	}
	else
    {
		throw std::invalid_argument("Not exist Name.");
	}
}

/*
입력값과 일치하는 int값 반환

@Author: 장형(?)
@Post date: 19.05.26
@Param: name
@Return: m_table[name]
*/
int CConfigParser::GetInt(std::string name)
{
	if (Contain(name))
    {
		return std::stoi(m_table[name]);
	}
	else
    {
		throw std::invalid_argument("Not exist Name.");
	}
}

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
config 파일 내용 확인

@Author: iwryu
@Since: 20.01.29
@Param: config_parser
@Return: 
*/
void check_config_file_content(CConfigParser config_parser)
{
	if (!config_parser.IsSuccess())
    {
		fputs("config 파일에 내용이 없습니다.", stderr);
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
@Param: input_YUV_file, config_parser, tile_index, decoding_file_read_size, decoding_read_data
@Return: 
*/
void copy_yuv_data
(
    FILE *input_YUV_file, 
    CConfigParser config_parser, 
    const unsigned int tile_index, 
    const unsigned int decoding_file_read_size, 
    unsigned short* decoding_read_data
)
{
    std::string string_tile_index = std::to_string(tile_index);

    if (string_tile_index.size() == 1)
    {
        string_tile_index.insert(0, 1, '0');
    }
    input_YUV_file = fopen(config_parser.GetString(string_tile_index).c_str(), "rb");
    check_file_open_error(input_YUV_file);

    // YUV 파일을 읽어, decoding_read_data 배열에 복사합니다.
    fread(decoding_read_data, sizeof(unsigned short), decoding_file_read_size, input_YUV_file);

    fclose(input_YUV_file);
}

/*
Y, U, V 값 분리

@Author: iwryu
@Since: 20.01.28
@Param: y_size, uv_size, YUV_data, decoding_read_data, u_data
@Return: 
*/
void split_yuv
(
    const unsigned int y_size, 
    const unsigned int uv_size, 
    unsigned short *YUV_data, 
    unsigned short *decoding_read_data, bool u_data
)
{
    if (!uv_size)
    {
        for (int i = 0; i < y_size; i++)
        {
            YUV_data[i] = decoding_read_data[i];
        }
    }
    else
    {
        if (u_data)
        {
            for (int i = 0; i < uv_size; i++)
            {
                YUV_data[i] = decoding_read_data[y_size + i];
            }
        }
        else
        {
            for (int i = 0; i < uv_size; i++)
            {
                YUV_data[i] = decoding_read_data[y_size + uv_size + i];
            }
        }
        
    }
}

/*
최종 YUV 파일 초기화

@Author: iwryu
@Since: 20.01.28
@Param: output_total_write_size, output_total_data
@Return: 
*/
void init_yuv_merge
(
    const unsigned int output_total_write_size, 
    unsigned short *output_total_data
)
{
    for (int i = 0; i < output_total_write_size; i++)
    {
        output_total_data[i] = 0;
    }
}

/*
YUV Merge

@Author: iwryu
@Since: 20.01.28
@Param: out_yuv, YUV_data, y_data, config_parser
@Return: 
*/
void execute_yuv_merge
(
    unsigned short *out_yuv, 
    unsigned short *YUV_data, 
    bool y_data, 
    CConfigParser config_parser
)
{
    if (y_data)
    {
        for (int i = 0; i < config_parser.GetInt(ConfigFileType::TILE_HEIGHT); i++)
        {
            for (int j = 0; j < config_parser.GetInt(ConfigFileType::TILE_WIDTH); j++)
            {
                out_yuv[(config_parser.GetInt(ConfigFileType::PIC_WIDTH) * i) + j] 
                    = YUV_data[(config_parser.GetInt(ConfigFileType::TILE_WIDTH) * i) + j];
            }
        }
    }
    else
    {
        for (int i = 0; i < config_parser.GetInt(ConfigFileType::TILE_HEIGHT) / 2; i++)
        {
            for (int j = 0; j < config_parser.GetInt(ConfigFileType::TILE_WIDTH) / 2; j++)
            {
                out_yuv[(config_parser.GetInt(ConfigFileType::PIC_WIDTH) * i / 2) + j] 
                    = YUV_data[(config_parser.GetInt(ConfigFileType::TILE_WIDTH) * i / 2) + j];
            }
        }
    }
}

/*
YUV Merge한 결과를 파일에 저장

@Author: iwryu
@Since: 20.01.28
@Param: 
@Return: 
*/
void write_yuv_merge_file
(
    FILE *output_YUV_file, 
    const char *argv, 
    unsigned short* output_total_data, 
    const unsigned int output_total_write_size
)
{
    output_YUV_file = fopen(argv, "wb");

    check_file_open_error(output_YUV_file);

    fwrite(output_total_data, sizeof(unsigned short), 
        output_total_write_size, output_YUV_file);

    fclose(output_YUV_file);
}

int main(int argc, char *argv[])
{
    clock_t start, end;
    double time_result;
    start = clock();
    
    check_argc(argc);

    CConfigParser config_parser(argv[1]);
    check_config_file_content(config_parser);

    // tile 번호
    unsigned int tile_index[8] = {4, 11, 12, 13, 19, 20, 21, 28};

    // 각 tile들의 yuv 크기
    unsigned int y_size 
        = config_parser.GetInt(ConfigFileType::TILE_WIDTH) * config_parser.GetInt(ConfigFileType::TILE_HEIGHT);
    unsigned int uv_size = y_size / 4;
    unsigned int decoding_file_read_size = (y_size + (y_size / 2));
    unsigned short *decoding_read_data 
        = new unsigned short[decoding_file_read_size];

    // Y, U, V의 배열 선언
    unsigned short *Y_data = new unsigned short[y_size];
    unsigned short *U_data = new unsigned short[uv_size];
    unsigned short *V_data = new unsigned short[uv_size];

    // yuv merger할(output) 파일의 yuv 크기
    unsigned int output_y_size 
        = config_parser.GetInt(ConfigFileType::PIC_WIDTH) * config_parser.GetInt(ConfigFileType::PIC_HEIGHT);
    unsigned int output_uv_size = output_y_size / 4;
    unsigned int output_total_write_size = (output_y_size + (output_y_size / 2));
    unsigned short *output_total_data 
        = new unsigned short[output_total_write_size];

    unsigned short *pointer_output_y = nullptr;
    unsigned short *pointer_output_u = nullptr;
    unsigned short *pointer_output_v = nullptr;
    
    init_yuv_merge(output_total_write_size, output_total_data);

    for (int i = 0; i < config_parser.GetInt(ConfigFileType::TILES); i++)
    {
        FILE *input_YUV_file = NULL;
        copy_yuv_data(input_YUV_file, config_parser, 
            tile_index[i], decoding_file_read_size, decoding_read_data);

        split_yuv(y_size, 0, Y_data, decoding_read_data, false);
        split_yuv(y_size, uv_size, U_data, decoding_read_data, true);
        split_yuv(y_size, uv_size, V_data, decoding_read_data, false);

        // 4x8 가로에서 몇 번째 가로인지를 표시
        unsigned int output_row_number 
            = tile_index[i] / (config_parser.GetInt(ConfigFileType::PIC_WIDTH) / config_parser.GetInt(ConfigFileType::TILE_WIDTH));
        // 타일의 y위치 값
        unsigned int tile_y_index 
            = config_parser.GetInt(ConfigFileType::TILE_HEIGHT) * output_row_number;
        // 타일의 uv 가로 크기
        unsigned int uv_tile_width 
            = config_parser.GetInt(ConfigFileType::TILE_WIDTH) / 2;
        // output의 uv 가로 크기
        unsigned int output_uv_width 
            = config_parser.GetInt(ConfigFileType::PIC_WIDTH) / 2;
        // output의 uv 높이위치 값
        unsigned int output_uv_height_index 
            = tile_y_index / 2;
        // 타일이 merger되는 위치
        unsigned int output_row_start_index
            = tile_index[i] - (output_row_number * 8);
        
        pointer_output_y 
            = &output_total_data[(output_row_start_index * config_parser.GetInt(ConfigFileType::TILE_WIDTH)) + (config_parser.GetInt(ConfigFileType::PIC_WIDTH) * tile_y_index)];
        pointer_output_u 
            = &output_total_data[output_row_start_index * uv_tile_width + (output_uv_width * output_uv_height_index) + output_y_size];
        pointer_output_v 
            = &output_total_data[output_row_start_index * uv_tile_width + (output_uv_width * output_uv_height_index) + output_y_size + output_uv_size];

        execute_yuv_merge(pointer_output_y, Y_data, true, config_parser);
        execute_yuv_merge(pointer_output_u, U_data, false, config_parser);
        execute_yuv_merge(pointer_output_v, V_data, false, config_parser);
    }

    FILE *output_YUV_file = NULL;
    write_yuv_merge_file(output_YUV_file, argv[2], 
        output_total_data, output_total_write_size);

    delete[] output_total_data;
    delete[] V_data;
    delete[] U_data;
    delete[] Y_data;
    delete[] decoding_read_data;

    end = clock();
    time_result = (double)(end - start);
    printf("Working time: %.2f seconds\n", time_result / CLOCKS_PER_SEC);

    return 0;
}
