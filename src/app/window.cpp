#include "app/window.h"
#include "app/debug.h"
#include "machine/bus.h"
#include "machine/machine.h"

static const char WIN_TITLE[] = "LilSys VM";

namespace lilsys
{
    sf::RenderWindow* window::_win = NULL;
    uint32_t window::_now          = 0;
    uint32_t window::_last         = 0;
    uint32_t window::_fps          = 0; 
    uint32_t window::_frames       = 0;

    machine* vm = NULL;

    void window::init()
    {
        _win = new sf::RenderWindow(sf::VideoMode(160 * 4, 120 * 4), "LilSys", sf::Style::Titlebar | sf::Style::Close);
        printf("%s Initialized application window - Size:%ux%u\n", DEBUG_OK, _win->getSize().x, _win->getSize().y);

        uint8_t prog[] = 
        {
            isa::LD.opcode, 0x00, 0x00, 0xB3,   // ld r0, 0xB300
            isa::LD.opcode, 0x01, 0x04, 0x0E,   // ld r1, 0x0E04
            isa::WRRW.opcode, 0x00, 0x01,       // wrrw r0, r1
            isa::ADD.opcode, 0x00, 0x02, 0x00,  // add r0, 2
            isa::WRRW.opcode, 0x00, 0x01,       // wrrw r0, r1
            isa::ADD.opcode, 0x00, 0x02, 0x00,  // add r0, 2
            isa::WRRW.opcode, 0x00, 0x01,       // wrrw r0, r1
            isa::ADD.opcode, 0x00, 0x02, 0x00,  // add r0, 2
            isa::WRRW.opcode, 0x00, 0x01,       // wrrw r0, r1
            isa::HLT.opcode,                    // hlt
        };

        vm = new machine(prog, sizeof(prog));
        vm->run();
    }

    void window::dispose()
    {
        delete _win;
        vm->dispose();
        delete vm;
    }

    void window::run()
    {
        printf("%s Entered application main loop\n", DEBUG_INFO);

        while (_win->isOpen())
        {
            sf::Event event;
            while (_win->pollEvent(event))
            {
                if (event.type == sf::Event::Closed) { _win->close(); exit(0); }
            }

            update();
            draw();
        }
    }

    void window::update()
    {
        _now = time(NULL);
        if (_now != _last)
        {
            _last   = _now;
            _fps    = _frames;
            _frames = 0;
            std::string title = WIN_TITLE + std::string(" - FPS: ") + std::to_string(_fps);
            _win->setTitle(title);
        }
    }

    void window::draw()
    {        
        _frames++;

        vm->update_pixels();

        _win->clear(sf::Color::Black);
        vm->display_pixels();
        _win->display();
    }

    sf::RenderWindow* window::handle() { return _win; }
}