#include "machine/machine.h"
#include "app/debug.h"
#include "app/window.h"

namespace lilsys
{
    machine::machine(uint8_t* prog, size_t size)
    {
        init();
        
        // resize rom device
        bus.devices[DEV_ROM].address = 0;
        bus.devices[DEV_ROM].size = malign(size, 0x100);

        // re-size available memory
        bus.devices[DEV_RAM].address = bus.devices[DEV_ROM].address + bus.devices[DEV_ROM].size;
        bus.devices[DEV_RAM].size = BUS_SIZE - (bus.devices[DEV_ROM].size + VRAM_SIZE + PORTS_SIZE);

        // load program into memory
        bus.wr(bus.devices[DEV_ROM].address, prog, bus.devices[DEV_ROM].size);

        printf("%s ROM           Address:0x%04X-0x%04X  RW:%u\n", DEBUG_INFO, bus.devices[DEV_ROM].address,   bus.devices[DEV_ROM].address   + bus.devices[DEV_ROM].size,   bus.devices[DEV_ROM].read_only ? 0 : 1);
        printf("%s RAM           Address:0x%04X-0x%04X  RW:%u\n", DEBUG_INFO, bus.devices[DEV_RAM].address,   bus.devices[DEV_RAM].address   + bus.devices[DEV_RAM].size,   bus.devices[DEV_RAM].read_only ? 0 : 1);
        printf("%s VRAM          Address:0x%04X-0x%04X  RW:%u\n", DEBUG_INFO, bus.devices[DEV_VRAM].address,  bus.devices[DEV_VRAM].address  + bus.devices[DEV_VRAM].size,  bus.devices[DEV_VRAM].read_only ? 0 : 1);
        printf("%s PORTS         Address:0x%04X-0x%04X RW:%u\n", DEBUG_INFO, bus.devices[DEV_PORTS].address, bus.devices[DEV_PORTS].address + bus.devices[DEV_PORTS].size, bus.devices[DEV_PORTS].read_only ? 0 : 1);
    }

    machine::machine(const char* fname)
    {
        init();
    }

    void machine::init()
    {
        _vid_pixels = new uint32_t[VID_WIDTH * VID_HEIGHT];
        _vid_texture.create(VID_WIDTH, VID_HEIGHT);

        bus.attach(this);
        bus.init();

        cpu.attach(this);
        cpu.init();

        printf("%s Created machine instance\n", DEBUG_OK);
    }

    void machine::dispose()
    {
        delete[] _vid_pixels;
    }

    void machine::run()
    {
        thread = std::thread(thread_main, this);
        thread.detach();
        cpu.unhalt();
    }

    void machine::thread_main(machine* m)
    {
        printf("%s Entered virtual machine main loop\n", DEBUG_INFO);
        while (true)
        {
            if (m->cpu.halted()) { continue; }
            m->cpu.step();
            //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    void machine::update_pixels()
    {
        bus.convert_vram(_vid_pixels, (uint8_t*)(bus.data() + bus.devices[DEV_VRAM].address));
        _vid_texture.update((uint8_t*)_vid_pixels);
    }

    void machine::display_pixels()
    {
        sf::Sprite spr(_vid_texture);
        spr.setScale(4, 4);
        window::handle()->draw(spr);
    }
}