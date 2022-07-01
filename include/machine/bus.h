#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include "machine/arch.h"

namespace lilsys
{
    class bus_device
    {
        public:
            VWORD address;
            VLONG size;
            bool  read_only;

        public:
            bus_device() : address(0), size(0), read_only(false) { }
            bus_device(VWORD addr, VLONG size, bool ro) : address(addr), size(size), read_only(ro) { }
    };

    class bus_controller
    {
        public:
            machine*   host;
            bus_device devices[4];

        private:
            uint8_t* _data;

        public:
            void attach(machine* m);
            void init();

        public:
            void fill(VBYTE value);
            void fill(VWORD addr, VBYTE value, VLONG size);

        public:
            void wrb(VWORD addr, VBYTE val);
            void wrw(VWORD addr, VWORD val);
            void wrl(VWORD addr, VLONG val);
            void wr(VWORD addr, void* data, VLONG size);

        public:
            register_id rdr(VWORD addr);
            VBYTE rdb(VWORD addr);
            VWORD rdw(VWORD addr);
            VLONG rdl(VWORD addr);
            void  rd(VWORD addr, void* dest, VLONG size);

        public:
            bus_device* device_at(VWORD addr);
            bool        can_write(VWORD addr);

        public:
            uint8_t* data();

        public:
            void convert_vram(uint32_t* dest, uint8_t* src);
    };
}