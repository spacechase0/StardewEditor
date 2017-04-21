#ifndef UI_SOUNDPLAYER_HPP
#define UI_SOUNDPLAYER_HPP

#include <SFML/Audio/Music.hpp>
#include <string>
#include <vector>

class Editor;
class Ui;

class SoundPlayer
{
    public:
        SoundPlayer( Editor& theEditor, Ui& theUi );
        
        void update();
        
        void refreshList();
        
        bool isShowing() const;
        void show();
        void hide();
        
        std::string getCurrentName() const;
        sf::Time getCurrentProgress() const;
        sf::Time getCurrentDuration() const;
    
    private:
        Editor& editor;
        Ui& ui;
        
        bool showing = false;
        
        std::vector< std::string > choices;
        std::string choicesStr;
        
        std::string current;
        sf::Music playing;
};

#endif // UI_SOUNDPLAYER_HPP
