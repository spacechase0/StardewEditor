#ifndef UIMODULE_HPP
#define UIMODULE_HPP

class UiModule
{
    public:
        virtual ~UiModule();
        
        virtual void menu() = 0;
        virtual void update() = 0;
        
        virtual void mapChanged();
};

#endif // UIMODULE_HPP
