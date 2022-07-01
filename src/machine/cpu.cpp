#include "machine/cpu.h"
#include "machine/machine.h"
#include "app/debug.h"

namespace lilsys
{
    void processing_unit::attach(machine* m) { host = m; }

    void processing_unit::init()
    {
        memset(registers, 0, sizeof(registers));

        uint32_t p_offset = host->bus.devices[DEV_PORTS].address;

        ports.push_back(port_t(PORT_KB_CMD, p_offset, false));  p_offset += 2;
        ports.push_back(port_t(PORT_KB_DATA, p_offset, false)); p_offset += 2;
        ports.push_back(port_t(PORT_MS_CMD, p_offset, false));  p_offset += 2;
        ports.push_back(port_t(PORT_MS_DATA, p_offset, false)); p_offset += 2;

        halt();
        printf("%s Initialized processing unit\n", DEBUG_OK);
    }

    void processing_unit::halt()
    {
        _halted = true;
        printf("%s Halted processing unit\n", DEBUG_INFO);
    }

    void processing_unit::unhalt()
    {
        _halted = false;
        printf("%s Unhalted processing unit\n", DEBUG_INFO);
    }

    void processing_unit::print_state()
    {
        VBYTE op = host->bus.rdb(registers[(VBYTE)register_id::PC]);

        const instr_t* instr = isa::get_byop(op);
        if (instr != NULL)
        {
            try
            {
                printf("- Disassembly - { %s ", instr->mnemonic.c_str());
                VWORD offset = registers[(VBYTE)register_id::PC] + 1;
                for (size_t i = 0; i < instr->arguments.length(); i++)
                {
                    if (instr->arguments[i] == 'r') { printf("%s", regname(host->bus.rdb(offset))); offset += 1; }
                    if (instr->arguments[i] == 'b') { printf("0x%02X", host->bus.rdb(offset)); offset += 1; }
                    if (instr->arguments[i] == 'w') { printf("0x%04X", host->bus.rdb(offset)); offset += 2; }
                    if (i < instr->arguments.length() - 1) { printf(", "); }
                }
                printf(" }\n");
            }
            catch(int err) { exit(0); }
        }

        printf("R0:0x%04X R1:0x%04X R2:0x%04X R3:0x%04X R4:0x%04X R5:0x%04X\n", registers[0x0], registers[0x1], registers[0x2], registers[0x3], registers[0x4], registers[0x5]);
        printf("R6:0x%04X R7:0x%04X R8:0x%04X R9:0x%04X RA:0x%04X RB:0x%04X\n", registers[0x6], registers[0x7], registers[0x8], registers[0x9], registers[0xA], registers[0xB]);
        printf("RC:0x%04X RD:0x%04X RE:0x%04X RF:0x%04X SP:0x%04X BP:0x%04X\n", registers[0xC], registers[0xD], registers[0xE], registers[0xF], registers[0x10], registers[0x11]);
        printf("PC:0x%04X FL:0x%04X\n", registers[0x12], registers[0x13]);
    }

    void processing_unit::step()
    {
        VWORD pc = registers[(VBYTE)register_id::PC];

        if (pc >= host->bus.devices[DEV_VRAM].address) { printf("%s Program counter has entered non-executable region of memory\n", DEBUG_ERROR); print_state(); exit(0); return; }
        VBYTE op = host->bus.rdb(pc);

        for (size_t i = 0; i < isa::COUNT; i++)
        {
            if (isa::instructions[i]->opcode == op)
            {
                isa::instructions[i]->handler(host);
                print_state();
                if (isa::instructions[i]->increment) { registers[(VBYTE)register_id::PC] += isa::instructions[i]->bytes; }
                return;
            }
        }

        printf("%s Invalid opcode 0x%02X\n", DEBUG_ERROR, op);
        print_state();
        exit(0);
    }

    void processing_unit::set_flags(register_id reg, bool neg)
    {
        bool z = (reg_rd(reg) == 0);
        registers[(VBYTE)register_id::FL] = (VWORD)((z << 1) | neg);
    }

    void processing_unit::set_flags(register_id reg)
    {
        bool neg = registers[(VBYTE)register_id::FL] & 0b00000001;
        set_flags(reg, neg);
    }

    void processing_unit::stack_push(VWORD value)
    {
        registers[(VBYTE)register_id::SP] -= 2;
        host->bus.wrw(registers[(VBYTE)register_id::SP], value);
    }

    VWORD processing_unit::stack_pop()
    {
        return host->bus.rdw(registers[(VBYTE)register_id::SP]);
        registers[(VBYTE)register_id::SP] += 2;
    }

    void processing_unit::reg_wr(register_id reg, VWORD value)
    {
        if (reg >= register_id::COUNT) { printf("%s Invalid register id 0x%02X\n", DEBUG_ERROR, (VBYTE)reg); print_state(); exit(0); }
        registers[(VBYTE)reg] = value;
    }

    VWORD processing_unit::reg_rd(register_id reg)
    {
        if (reg >= register_id::COUNT) { printf("%s Invalid register id 0x%02X\n", DEBUG_ERROR, (VBYTE)reg); print_state(); exit(0); }
        return registers[(VBYTE)reg];
    }

    void processing_unit::reg_add(register_id reg, VWORD value) { reg_wr(reg, reg_rd(reg) + value);  set_flags(reg); }
    void processing_unit::reg_sub(register_id reg, VWORD value) { reg_wr(reg, reg_rd(reg) - value);  set_flags(reg); }
    void processing_unit::reg_mul(register_id reg, VWORD value) { reg_wr(reg, reg_rd(reg) * value);  set_flags(reg); }
    void processing_unit::reg_div(register_id reg, VWORD value) { reg_wr(reg, reg_rd(reg) / value);  set_flags(reg); }
    void processing_unit::reg_or(register_id reg,  VWORD value) { reg_wr(reg, reg_rd(reg) | value);  set_flags(reg); }
    void processing_unit::reg_xor(register_id reg, VWORD value) { reg_wr(reg, reg_rd(reg) ^ value);  set_flags(reg); }
    void processing_unit::reg_and(register_id reg, VWORD value) { reg_wr(reg, reg_rd(reg) & value);  set_flags(reg); }
    void processing_unit::reg_shl(register_id reg, VBYTE value) { reg_wr(reg, reg_rd(reg) << value); set_flags(reg); }
    void processing_unit::reg_shr(register_id reg, VBYTE value) { reg_wr(reg, reg_rd(reg) >> value); set_flags(reg); }
    void processing_unit::reg_not(register_id reg)              { reg_wr(reg, ~reg_rd(reg));         set_flags(reg); }

    bool processing_unit::halted() { return _halted; }
    
}