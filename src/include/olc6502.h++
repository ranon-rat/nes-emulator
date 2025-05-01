#ifndef C502
#define C502
#include <cstdint>
#include <string>
#include <vector>
class Bus;
class Olc6502
{
public:
    Olc6502();
    ~Olc6502();

public:
    enum FLAG6502
    {
        C = (1 << 0), // carry bit
        Z = (1 << 1), // zero
        I = (1 << 2), // disable interrupters
        D = (1 << 3), // decimal mode(no lo implementaremos)
        B = (1 << 4), // break
        U = (1 << 5), // unused
        V = (1 << 6), // unused
        N = (1 << 7), // negativo

    };

    uint8_t acc_r = 0x00;      // accumulator
    uint8_t x_r = 0x00;      // x register
    uint8_t y_r = 0x00;      // y register
    uint8_t stkp_r = 0x00;   // points to location on bus
    uint16_t pc_r = 0x0000;  // program counter register
    uint8_t status_r = 0x00; // status register

public:
    void ConnectBus(Bus *n)
    {
        m_bus = n;
    };

    // addressing modes

    uint8_t IMP();	uint8_t IMM();	
	uint8_t ZP0();	uint8_t ZPX();	
	uint8_t ZPY();	uint8_t REL();
	uint8_t ABS();	uint8_t ABX();	
	uint8_t ABY();	uint8_t IND();	
	uint8_t IZX();	uint8_t IZY();

    // opocodes
    uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

    // illegal opcodes

    uint8_t XXX();
    void clock();
    void reset();
    // interrupt request signal
    void irq();
    /// non maskable integral request signal?
    void nmi();

    uint8_t fetch();
    uint8_t fetched = 0x00;// it seems that it  passes information to the ALU?
    uint16_t addr_abs = 0x0000; //   used addresses end up here?
    uint16_t addr_rel = 0x00; // absolute address following a branch?
    uint8_t opcode = 0x00; // instruction byte
    uint8_t cycles = 0; // the number of cycles it will take an instruction to execute
public: // utils

    void general_branching();
    void general_interrupt_request(uint16_t stack_checking,uint8_t cy);

private:
    Bus *m_bus = nullptr;
    uint8_t read(uint16_t a);
    void write(uint16_t a, uint8_t d);
    // convenience functions to access status register
    uint8_t GetFlag(FLAG6502 f);
    void SetFlag(FLAG6502 f, bool v);
    struct  INSTRUCTION{
        std::string name;
        uint8_t (Olc6502::*operate)(void)=nullptr;
        uint8_t (Olc6502::*addrmode)(void)=nullptr;
        uint8_t cycles=0;
    };
    std::vector<INSTRUCTION> m_lookup;
};
#endif