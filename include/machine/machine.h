#pragma once
#include <stdint.h>
#include <thread>
#include <SFML/Graphics.hpp>
#include "machine/bus.h"
#include "machine/cpu.h"

namespace lilsys
{
    class machine
    {
        public:
            processing_unit cpu;
            bus_controller  bus;
            std::thread     thread;
        
        private:
            uint32_t*   _vid_pixels;
            sf::Texture _vid_texture;

        public:
            machine(uint8_t* prog, size_t size);
            machine(const char* fname);
            void dispose();

        private:
            void init();
            static void thread_main(machine* m);

        public:
            void run();
            void update_pixels();
            void display_pixels();
    };
}