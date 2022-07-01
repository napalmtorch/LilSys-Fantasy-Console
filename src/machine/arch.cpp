#include "machine/arch.h"
#include "machine/machine.h"
#include "machine/cpu.h"

#define _PC (int)lilsys::register_id::PC
#define _FL (int)lilsys::register_id::FL

namespace lilsys
{
    namespace isa_handlers
    {
        void NOP(machine* m) { }
        void HLT(machine* m) { m->cpu.halt(); }

        void ADD(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VWORD       v = m->bus.rdw(m->cpu.registers[_PC] + 2);
            m->cpu.reg_add(r, v);
        }

        void ADDR(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->cpu.reg_add(rx, m->cpu.reg_rd(ry));
        }

        void SUB(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VWORD       v = m->bus.rdw(m->cpu.registers[_PC] + 2);
            m->cpu.reg_sub(r, v);
        }

        void SUBR(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->cpu.reg_sub(rx, m->cpu.reg_rd(ry));
        }

        void MUL(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VWORD       v = m->bus.rdw(m->cpu.registers[_PC] + 2);
            m->cpu.reg_mul(r, v);
        }

        void MULR(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->cpu.reg_mul(rx, m->cpu.reg_rd(ry));
        }

        void DIV(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VWORD       v = m->bus.rdw(m->cpu.registers[_PC] + 2);
            m->cpu.reg_div(r, v);
        }

        void DIVR(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->cpu.reg_div(rx, m->cpu.reg_rd(ry));
        }

        void OR(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VWORD       v = m->bus.rdw(m->cpu.registers[_PC] + 2);
            m->cpu.reg_or(r, v);
        }

        void ORR(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->cpu.reg_or(rx, m->cpu.reg_rd(ry));
        }

        void XOR(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VWORD       v = m->bus.rdw(m->cpu.registers[_PC] + 2);
            m->cpu.reg_xor(r, v);
        }

        void XORR(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->cpu.reg_xor(rx, m->cpu.reg_rd(ry));
        }

        void AND(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VWORD       v = m->bus.rdw(m->cpu.registers[_PC] + 2);
            m->cpu.reg_and(r, v);
        }

        void ANDR(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->cpu.reg_and(rx, m->cpu.reg_rd(ry));
        }

        void NOT(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            m->cpu.reg_not(r);
        }

        void SHL(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VBYTE       v = m->bus.rdb(m->cpu.registers[_PC] + 2);
            m->cpu.reg_shl(r, v);
        }

        void SHRL(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->cpu.reg_shl(rx, m->cpu.reg_rd(ry));
        }

        void SHR(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VBYTE       v = m->bus.rdb(m->cpu.registers[_PC] + 2);
            m->cpu.reg_shr(r, v);
        }

        void SHRR(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->cpu.reg_shr(rx, m->cpu.reg_rd(ry));
        }

        void LD(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VWORD       v = m->bus.rdw(m->cpu.registers[_PC] + 2);
            m->cpu.reg_wr(r, v);
        }

        void LDR(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->cpu.reg_wr(rx, m->cpu.reg_rd(ry));
        }

        void LDB(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VWORD       a = m->bus.rdw(m->cpu.registers[_PC] + 2);
            m->cpu.reg_wr(r, m->bus.rdb(a));
        }

        void LDW(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VWORD       a = m->bus.rdw(m->cpu.registers[_PC] + 2);
            m->cpu.reg_wr(r, m->bus.rdw(a));
        }

        void LDRB(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->cpu.reg_wr(rx, m->bus.rdb(m->cpu.reg_rd(ry)));
        }
        
        void LDRW(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->cpu.reg_wr(rx, m->bus.rdw(m->cpu.reg_rd(ry)));
        }

        void STB(machine* m)
        {
            VWORD a = m->bus.rdw(m->cpu.registers[_PC] + 1);
            VBYTE v = m->bus.rdb(m->cpu.registers[_PC] + 3);
            m->bus.wrb(a, v);
        }

        void STW(machine* m)
        {
            VWORD a = m->bus.rdw(m->cpu.registers[_PC] + 1);
            VWORD v = m->bus.rdw(m->cpu.registers[_PC] + 3);
            m->bus.wrw(a, v);
        }

        void STRB(machine* m)
        {
            VWORD       a = m->bus.rdw(m->cpu.registers[_PC] + 1);
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 3);
            printf("STRB - ADDR: 0x%04X REG: 0x%02X\n", a, (int)r);
            m->bus.wrb(a, m->cpu.reg_rd(r) & 0xFF);
        }

        void STRW(machine* m)
        {
            VWORD       a = m->bus.rdw(m->cpu.registers[_PC] + 1);
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 3);
            m->bus.wrw(a, m->cpu.reg_rd(r));
        }

        void WRB(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VBYTE       v = m->bus.rdb(m->cpu.registers[_PC] + 2);
            m->bus.wrb(m->cpu.reg_rd(r), v);
        }

        void WRW(machine* m)
        {
            register_id r = m->bus.rdr(m->cpu.registers[_PC] + 1);
            VWORD       v = m->bus.rdw(m->cpu.registers[_PC] + 2);
            m->bus.wrw(m->cpu.reg_rd(r), v);
        }

        void WRRB(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->bus.wrb(m->cpu.reg_rd(rx), m->cpu.reg_rd(ry) & 0xFF);
        }

        void WRRW(machine* m)
        {
            register_id rx = m->bus.rdr(m->cpu.registers[_PC] + 1);
            register_id ry = m->bus.rdr(m->cpu.registers[_PC] + 2);
            m->bus.wrw(m->cpu.reg_rd(rx), m->cpu.reg_rd(ry));
        }
    }
}