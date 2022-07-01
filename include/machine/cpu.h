#pragma once
#include <stdint.h>
#include "machine/arch.h"

namespace lilsys
{
    class machine;

    class processing_unit
    {
        public:
            machine* host;
            VWORD    registers[(int)register_id::COUNT];

        private:
            bool _halted;

        public:
            void attach(machine* m);
            void init();
            void halt();
            void unhalt();
            void print_state();

        public:
            void step();
            void set_flags(register_id reg, bool neg);
            void set_flags(register_id reg);

        public:
            void  reg_wr(register_id reg, VWORD value);
            VWORD reg_rd(register_id reg);

        public:
            void reg_add(register_id reg, VWORD value);
            void reg_sub(register_id reg, VWORD value);
            void reg_mul(register_id reg, VWORD value);
            void reg_div(register_id reg, VWORD value);
            void reg_or(register_id reg, VWORD value);
            void reg_xor(register_id reg, VWORD value);
            void reg_and(register_id reg, VWORD value); 
            void reg_shl(register_id reg, VBYTE value);
            void reg_shr(register_id reg, VBYTE value);
            void reg_not(register_id reg);

        public:
            bool halted();
    };
}