/*
Config 파일을 파싱하는 클래스
참고 URL: https://developstudy.tistory.com/49

@Author: 장형(?)
@Post date: 19.05.26
*/
#include <string>
#include <map>

class CConfigParser
{
    public:
        CConfigParser(std::string path); 
        bool IsSuccess() { return m_table.size() != 0; }
        bool Contain(std::string name);
        std::string GetString(std::string name);
        int GetInt(std::string name);
    
    private:
        std::map<std::string, std::string> m_table;
};