#pragma once
#include <SFML/Graphics.hpp>

namespace lilsys
{
    class window
    {
        private:
            static sf::RenderWindow* _win;
            static uint32_t _now, _last, _fps, _frames;

        public:
            static void init();
            static void dispose();
            static void run();
            static void update();
            static void draw();

        public:
            static sf::RenderWindow* handle();
    };
}