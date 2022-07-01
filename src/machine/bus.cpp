#include "machine/bus.h"
#include "machine/machine.h"
#include "app/debug.h"

namespace lilsys
{
    void bus_controller::attach(machine* m) { host = m; }

    void bus_controller::init()
    {
        // create memory array
        _data = new uint8_t[BUS_SIZE];
        fill(0);

        // ports
        devices[DEV_PORTS] = bus_device(BUS_SIZE - PORTS_SIZE, PORTS_SIZE, false);
        devices[DEV_VRAM]  = bus_device(devices[DEV_PORTS].address - VRAM_SIZE, VRAM_SIZE, false);
        devices[DEV_ROM]   = bus_device(0, 0, false);
        devices[DEV_RAM]   = bus_device(0, devices[DEV_VRAM].address, false);

        // finished
        printf("%s Finished initializing bus controller\n", DEBUG_OK);
    }

    void bus_controller::fill(VBYTE value) { memset(_data, value, BUS_SIZE); }

    void bus_controller::fill(VWORD addr, VBYTE value, VLONG size)
    {
        if (addr + size >= BUS_SIZE) { printf("%s Memory fill out of bounds\n", DEBUG_ERROR); exit(0); }
        memset(&_data[addr], value, size);
    }

    void bus_controller::wrb(VWORD addr, VBYTE val)
    {
        if (addr >= BUS_SIZE) { printf("%s Memory write out of bounds\n", DEBUG_ERROR); exit(0); }
        if (!can_write(addr)) { printf("%s Memory write to read-only region\n", DEBUG_ERROR); exit(0); }
        _data[addr] = val;
    }

    void bus_controller::wrw(VWORD addr, VWORD val)
    {
        wrb(addr + 0, (val & 0x00FF));
        wrb(addr + 1, (val & 0xFF00) >> 8);
    }

    void bus_controller::wrl(VWORD addr, VLONG val)
    {
        wrb(addr + 0, (val & 0x000000FF));
        wrb(addr + 1, (val & 0x0000FF00) >> 8);
        wrb(addr + 2, (val & 0x00FF0000) >> 16);
        wrb(addr + 3, (val & 0xFF000000) >> 24);
    }

    void bus_controller::wr(VWORD addr, void* data, VLONG size)
    {
        if (addr + size > BUS_SIZE) { printf("%s Memory write out of bounds\n", DEBUG_ERROR); exit(0); }
        memcpy(_data, data, size);
    }

    register_id bus_controller::rdr(VWORD addr) { return (register_id)rdb(addr); }

    VBYTE bus_controller::rdb(VWORD addr)
    {
        if (addr >= BUS_SIZE) { printf("%s Memory 8-bit read out of bounds\n", DEBUG_ERROR); exit(0); }
        return _data[addr];
    }

    VWORD bus_controller::rdw(VWORD addr)
    {
        return (rdb(addr + 1) << 8) | rdb(addr);
    }
    
    VLONG bus_controller::rdl(VWORD addr)
    {
        return (rdb(addr + 3) << 24) | (rdb(addr + 2) << 16) | (rdb(addr + 1) << 8) | rdb(addr);
    }

    void bus_controller::rd(VWORD addr, void* dest, VLONG size)
    {
        if (addr + size > BUS_SIZE) { printf("%s Memory read out of bounds\n", DEBUG_ERROR); exit(0); }
        if (dest == NULL) { printf("%s Tried to read from bus into null array\n", DEBUG_ERROR); exit(0); }
        memcpy(dest, &_data[addr], size);
    }

    bus_device* bus_controller::device_at(VWORD addr)
    {
        for (size_t i = 0; i < 4; i++)
        {
            if (devices[i].size == 0) { continue; }
            if (addr >= devices[i].address && addr < devices[i].address + devices[i].size) { return &devices[i]; }
        }
        return NULL;
    }

    bool bus_controller::can_write(VWORD addr)
    {
        bus_device* dev = device_at(addr);
        if (dev->read_only) { return false; }
        return true;
    }

    uint8_t* bus_controller::data() { return _data; }

    void bus_controller::convert_vram(uint32_t* dest, uint8_t* src)
    {
        static const uint32_t _palette[] = { 0xFF000000, 0xFF7F0000, 0xFF007F00, 0xFF7F7F00, 0xFF00007F, 0xFF7F007F, 0xFF007F7F, 0xFFAFAFAF, 0xFF7F7F7F, 0xFFFF0000, 0xFF00FF00, 0xFFFFFF00, 0xFF0000FF, 0xFFFF00FF, 0xFF00FFFF, 0xFFFFFFFF };
        for (size_t i = 0; i < 160 * 120; i++) { dest[i] = _palette[src[i]]; }
    }
}