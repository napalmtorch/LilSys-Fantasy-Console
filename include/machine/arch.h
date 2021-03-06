#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string>

typedef unsigned char  VBYTE;
typedef unsigned short VWORD;
typedef unsigned int   VLONG;
typedef unsigned short VPORT;

#define VID_WIDTH 160
#define VID_HEIGHT 120

#define DEV_ROM   0x0
#define DEV_RAM   0x1
#define DEV_VRAM  0x2
#define DEV_PORTS 0x3

#define BUS_SIZE   0x10000
#define PORTS_SIZE 0x200
#define VRAM_SIZE  (VID_WIDTH * VID_HEIGHT)

#define PORT_KB_CMD  0x60
#define PORT_KB_DATA 0x61
#define PORT_MS_CMD  0x70
#define PORT_MS_DATA 0x71

namespace lilsys
{
    class machine;

    enum class register_id : VBYTE { R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, RA, RB, RC, RD, RE, RF, SP, BP, PC, FL, COUNT };

    struct port_t
    {
        public:
            VPORT id;
            VWORD address;
            bool  read_only;

        public:
            port_t() : id(0), address(0), read_only(false) { }
            port_t(VPORT id, VWORD addr, bool ro) : id(id), address(addr), read_only(ro) { }
    };

    typedef void (*instr_handler_t)(machine*);

    struct instr_t
    {
        public:
            std::string     mnemonic;
            std::string     arguments;
            uint8_t         opcode;
            uint8_t         bytes;
            bool            increment;
            instr_handler_t handler;

        public:
            instr_t() : mnemonic(""), arguments(""), opcode(0), bytes(0), increment(false), handler(NULL) { }
            instr_t(const char* mnem, const char* args, uint8_t op, uint8_t bytes, bool inc, instr_handler_t handler) : mnemonic(mnem), arguments(args), opcode(op), bytes(bytes), increment(inc), handler(handler) { }
    };

    namespace isa_handlers
    {
        void NOP(machine* m);   
        void HLT(machine* m);

        void ADD(machine* m);
        void ADDR(machine* m);
        void SUB(machine* m);
        void SUBR(machine* m);
        void MUL(machine* m);
        void MULR(machine* m);
        void DIV(machine* m);
        void DIVR(machine* m);
        void OR(machine* m);
        void ORR(machine* m);
        void XOR(machine* m);
        void XORR(machine* m);
        void AND(machine* m);
        void ANDR(machine* m);
        void NOT(machine* m);
        void SHL(machine* m);
        void SHRL(machine* m);
        void SHR(machine* m);
        void SHRR(machine* m);

        void LD(machine* m);
        void LDR(machine* m);
        void LDB(machine* m);
        void LDW(machine* m);
        void LDRB(machine* m);
        void LDRW(machine* m);

        void STB(machine* m);
        void STW(machine* m);
        void STRB(machine* m);
        void STRW(machine* m);
        void WRB(machine* m);
        void WRW(machine* m);
        void WRRB(machine* m);
        void WRRW(machine* m);

        void JMP(machine* m);
        void JMPR(machine* m);
        void CALL(machine* m);
        void CALLR(machine* m);
        void PUSH(machine* m);
        void PUSHR(machine* m);
        void POP(machine* m);
        void POPR(machine* m);
        void RET(machine* m);
    }

    namespace isa
    {
        static const instr_t NOP = instr_t("NOP", "", 0x00, 1, true,  isa_handlers::NOP);
        static const instr_t HLT = instr_t("HLT", "", 0xFF, 1, false, isa_handlers::HLT);

        static const instr_t ADD  = instr_t("ADD",  "rw", 0x01, 4, true, isa_handlers::ADD);
        static const instr_t ADDR = instr_t("ADDR", "rr", 0x02, 3, true, isa_handlers::ADDR);
        static const instr_t SUB  = instr_t("SUB",  "rw", 0x03, 4, true, isa_handlers::SUB);
        static const instr_t SUBR = instr_t("SUBR", "rr", 0x04, 3, true, isa_handlers::SUBR);
        static const instr_t MUL  = instr_t("MUL",  "rw", 0x05, 4, true, isa_handlers::MUL);
        static const instr_t MULR = instr_t("MULR", "rr", 0x06, 3, true, isa_handlers::MULR);
        static const instr_t DIV  = instr_t("DIV",  "rw", 0x07, 4, true, isa_handlers::DIV);
        static const instr_t DIVR = instr_t("DIVR", "rr", 0x08, 3, true, isa_handlers::DIVR);

        static const instr_t OR   = instr_t("OR",   "rw", 0x09, 4, true, isa_handlers::OR);
        static const instr_t ORR  = instr_t("ORR",  "rr", 0x0A, 3, true, isa_handlers::ORR);
        static const instr_t XOR  = instr_t("XOR",  "rw", 0x0B, 4, true, isa_handlers::XOR);
        static const instr_t XORR = instr_t("XORR", "rr", 0x0C, 3, true, isa_handlers::XORR);
        static const instr_t AND  = instr_t("AND",  "rw", 0x0D, 4, true, isa_handlers::AND);
        static const instr_t ANDR = instr_t("ANDR", "rr", 0x0E, 3, true, isa_handlers::ANDR);
        static const instr_t NOT  = instr_t("NOT",  "r",  0x0F, 2, true, isa_handlers::NOT);

        static const instr_t SHL  = instr_t("SHL",  "rb", 0x10, 3, true, isa_handlers::SHL);
        static const instr_t SHRL = instr_t("SHRL", "rr", 0x11, 3, true, isa_handlers::SHRL);
        static const instr_t SHR  = instr_t("SHR",  "rb", 0x12, 3, true, isa_handlers::SHR);
        static const instr_t SHRR = instr_t("SHRR", "rr", 0x13, 3, true, isa_handlers::SHRR);

        static const instr_t LD   = instr_t("LD",   "rw", 0x14, 4, true, isa_handlers::LD);
        static const instr_t LDR  = instr_t("LDR",  "rr", 0x15, 3, true, isa_handlers::LDR);
        static const instr_t LDB  = instr_t("LDB",  "rw", 0x16, 4, true, isa_handlers::LDB);
        static const instr_t LDW  = instr_t("LDW",  "rw", 0x17, 4, true, isa_handlers::LDW);
        static const instr_t LDRB = instr_t("LDRB", "rr", 0x18, 3, true, isa_handlers::LDRB);
        static const instr_t LDRW = instr_t("LDRW", "rr", 0x19, 3, true, isa_handlers::LDRW);

        static const instr_t STB  = instr_t("STB",  "wb", 0x1A, 4, true, isa_handlers::STB);
        static const instr_t STW  = instr_t("STW",  "ww", 0x1B, 5, true, isa_handlers::STW);
        static const instr_t STRB = instr_t("STRB", "wr", 0x1C, 4, true, isa_handlers::STRB);
        static const instr_t STRW = instr_t("STRW", "wr", 0x1D, 4, true, isa_handlers::STRW);
        static const instr_t WRB  = instr_t("WRB",  "rb", 0x1E, 3, true, isa_handlers::WRB);
        static const instr_t WRW  = instr_t("WRW",  "rw", 0x1F, 4, true, isa_handlers::WRW);
        static const instr_t WRRB = instr_t("WRRB", "rr", 0x20, 3, true, isa_handlers::WRRB);
        static const instr_t WRRW = instr_t("WRRW", "rr", 0x21, 3, true, isa_handlers::WRRW);

        static const instr_t JMP   = instr_t("JMP",   "w", 0x22, 3, false, isa_handlers::JMP);
        static const instr_t JMPR  = instr_t("JMPR",  "r", 0x23, 2, false, isa_handlers::JMPR);
        static const instr_t CALL  = instr_t("CALL",  "w", 0x24, 3, false, isa_handlers::CALL);
        static const instr_t CALLR = instr_t("CALLR", "r", 0x25, 2, false, isa_handlers::CALLR);
        static const instr_t PUSH  = instr_t("PUSH",  "w", 0x26, 3, true,  isa_handlers::PUSH);
        static const instr_t PUSHR = instr_t("PUSHR", "r", 0x27, 2, true,  isa_handlers::PUSHR);
        static const instr_t POP   = instr_t("POP",   "",  0x28, 1, true,  isa_handlers::POP);
        static const instr_t POPR  = instr_t("POPR",  "r", 0x29, 2, true,  isa_handlers::POPR);
        static const instr_t RET   = instr_t("RET",   "",  0x2A, 1, false, isa_handlers::RET);

        static const instr_t* instructions[] = 
        {
            &NOP,
            &HLT,

            &ADD,
            &ADDR,
            &SUB,
            &SUBR,
            &MUL,
            &MULR,
            &DIV,
            &DIVR,

            &OR,
            &ORR,
            &XOR,
            &XORR,
            &AND,
            &ANDR,
            &NOT,

            &SHL,
            &SHRL,
            &SHR,
            &SHRR,
            
            &LD,
            &LDR,
            &LDB,
            &LDW,
            &LDRB,
            &LDRW,

            &STB,
            &STW,
            &STRB,
            &STRW,
            &WRB,
            &WRW,
            &WRRB,
            &WRRW,

            &JMP,
            &JMPR,
            &CALL,
            &CALLR,
            &PUSH,
            &PUSHR,
            &POP,
            &POPR,
            &RET,
        };
        static const size_t COUNT = sizeof(instructions) / sizeof(instr_t*);

        static inline const instr_t* get_byop(VBYTE op)
        {
            for (size_t i = 0; i < COUNT; i++)
            {
                if (instructions[i]->opcode == op) { return instructions[i]; }
            }
            return NULL;
        }
    }

    static inline uint32_t malign(uint32_t num, uint32_t align)
    {
        uint32_t out = num;
        out &= (0xFFFFFFFF - (align - 1));
        if (out < num) { out += align; }
        return out;
    }

    static inline const char* regname(VBYTE reg)
    {
        switch (reg)
        {
            case 0x00: { return "R0"; }
            case 0x01: { return "R1"; }
            case 0x02: { return "R2"; }
            case 0x03: { return "R3"; }
            case 0x04: { return "R4"; }
            case 0x05: { return "R5"; }
            case 0x06: { return "R6"; }
            case 0x07: { return "R7"; }
            case 0x08: { return "R8"; }
            case 0x09: { return "R9"; }
            case 0x0A: { return "RA"; }
            case 0x0B: { return "RB"; }
            case 0x0C: { return "RC"; }
            case 0x0D: { return "RD"; }
            case 0x0E: { return "RE"; }
            case 0x0F: { return "RF"; }
            case 0x10: { return "SP"; }
            case 0x11: { return "BP"; }
            case 0x12: { return "PC"; }
            case 0x13: { return "FL"; }
        }
        return "\0";
    }
}