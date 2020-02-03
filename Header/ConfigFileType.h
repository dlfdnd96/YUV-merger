/*
Config 파일 상수화 클래스

@Author: iwryu
@Since: 20.01.29
*/
#include <string>

class ConfigFileType
{
    public:
        inline static const std::string START_FRAME = "startFrame";
        inline static const std::string NUMBER_OF_FRAMES = "numberOfFrames";
        inline static const std::string TILES = "tiles";
        inline static const std::string PIC_WIDTH = "picWidth";
        inline static const std::string PIC_HEIGHT = "picHeight";
        inline static const std::string TILE_WIDTH = "tileWidth";
        inline static const std::string TILE_HEIGHT = "tileHeight";
};