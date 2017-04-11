#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <map>
#include <string>

class Config
{
    public:
        Config();
        
        bool loadFromFile( const std::string& path );
        bool saveToFile( const std::string& path ) const;
        
        void setUnpackedContentFolder( const std::string& path );
        std::string getUnpackedContentFolder() const;
        
        void setMapImagesFolder( const std::string& path );
        std::string getMapImagesFolder() const;
        
        void reset();
    
    private:
        std::string unpackedContent;
        std::string mapImages;
};

#endif // CONFIG_HPP
