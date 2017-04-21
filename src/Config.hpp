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
        
        void setDataFolder( const std::string& path );
        std::string getDataFolder() const;
        
        void setExtractedSounds( const std::string& theExtractedSounds );
        std::string getExtractedSounds() const;
        
        void reset();
    
    private:
        std::string unpackedContent;
        std::string dataFolder;
        std::string extractedSounds;
};

#endif // CONFIG_HPP
