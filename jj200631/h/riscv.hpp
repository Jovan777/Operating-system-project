#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_RISCV_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_RISCV_HPP

#include "../lib/hw.h"
#include "MemoryAllocator.hpp"
#include "tcb.hpp"
#include "Scheduler.hpp"
#include "syscall_c.h"
#include "../lib/mem.h"
#include "sem.h"


class Riscv {
public:

    /*
    Eksplicitno naglasavam da sam iskoristio deo koda sa snimaka vezbi u
    kojima je radjena sinhrona i asinhrona promena konteksta
     */

    // read register scause
    static uint64 r_scause();

    // write register scause
    static void w_scause(uint64 scause);

    // read register sepc
    static uint64 r_sepc();

    // write register sepc
    static void w_sepc(uint64 sepc);

    // read register stvec
    static uint64 r_stvec();

    // write register stvec
    static void w_stvec(uint64 stvec);

    // read register stval
    static uint64 r_stval();

    // write register stval
    static void w_stval(uint64 stval);

    enum BitMaskSip
    {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9),
    };

    // mask set register sip
    static void ms_sip(uint64 mask);

    // mask clear register sip
    static void mc_sip(uint64 mask);

    // read register sip
    static uint64 r_sip();

    // write register sip
    static void w_sip(uint64 sip);

    enum BitMaskSstatus
    {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    // mask set register sstatus
    static void ms_sstatus(uint64 mask);

    // mask clear register sstatus
    static void mc_sstatus(uint64 mask);

    // read register sstatus
    static uint64 r_sstatus();

    // write register sstatus
    static void w_sstatus(uint64 sstatus);

    // supervisor trap
    static void supervisorTrap();

    static void popSppSpie();

    static uint64 read_a0();
    static void write_a0(uint64 source);

    static uint64 read_a1();
    static void write_a1(uint64 source);

    static uint64 read_a2();
    static void write_a2(uint64 source);

    static uint64 read_a3();
    static void write_a3(uint64 source);

    static uint64 read_a4();
    static void write_a4(uint64 source);

    static uint64 read_a5();
    static void write_a5(uint64 source);

    static uint64 read_a6();
    static void write_a6(uint64 source);

    static uint64 read_a7();
    static void write_a7(uint64 source);

private:

    // supervisor trap handler
    static void handleSupervisorTrap();

};




inline uint64 Riscv::r_scause()
{
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void Riscv::w_scause(uint64 scause)
{
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 Riscv::r_sepc()
{
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void Riscv::w_sepc(uint64 sepc)
{
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 Riscv::r_stvec()
{
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void Riscv::w_stvec(uint64 stvec)
{
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 Riscv::r_stval()
{
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void Riscv::w_stval(uint64 stval)
{
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void Riscv::ms_sip(uint64 mask)
{
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sip(uint64 mask)
{
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sip()
{
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void Riscv::w_sip(uint64 sip)
{
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void Riscv::ms_sstatus(uint64 mask)
{
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void Riscv::mc_sstatus(uint64 mask)
{
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 Riscv::r_sstatus()
{
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void Riscv::w_sstatus(uint64 sstatus)
{
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

/////////////////////////////////

inline uint64 Riscv::read_a0()
{
    uint64 source;
    __asm__ volatile ("mv %[source], a0" : [source] "=r"((source)));
    return source;
}

inline void Riscv::write_a0(uint64 source) {
    __asm__ volatile ("mv a0, %[source]" : : [source] "r"((source)));
}

/////////////////////////////////

inline uint64 Riscv::read_a1()
{
    uint64 source;
    __asm__ volatile ("mv %[source], a1" : [source] "=r"((source)));
    return source;
}

inline void Riscv::write_a1(uint64 source) {
    __asm__ volatile ("mv a1, %[source]" : : [source] "r"((source)));
}

/////////////////////////////////

inline uint64 Riscv::read_a2()
{
    uint64 source;
    __asm__ volatile ("mv %[source], a2" : [source] "=r"((source)));
    return source;
}

inline void Riscv::write_a2(uint64 source) {
    __asm__ volatile ("mv a2, %[source]" : : [source] "r"((source)));
}

/////////////////////////////////


inline uint64 Riscv::read_a3()
{
    uint64 source;
    __asm__ volatile ("mv %[source], a3" : [source] "=r"((source)));
    return source;
}

inline void Riscv::write_a3(uint64 source) {
    __asm__ volatile ("mv a3, %[source]" : : [source] "r"((source)));
}

/////////////////////////////////


inline uint64 Riscv::read_a4()
{
    uint64 source;
    __asm__ volatile ("mv %[source], a4" : [source] "=r"((source)));
    return source;
}

inline void Riscv::write_a4(uint64 source) {
    __asm__ volatile ("mv a4, %[source]" : : [source] "r"((source)));
}

/////////////////////////////////


inline uint64 Riscv::read_a5()
{
    uint64 source;
    __asm__ volatile ("mv %[source], a5" : [source] "=r"((source)));
    return source;
}

inline void Riscv::write_a5(uint64 source) {
    __asm__ volatile ("mv a5, %[source]" : : [source] "r"((source)));
}

/////////////////////////////////


inline uint64 Riscv::read_a6()
{
    uint64 source;
    __asm__ volatile ("mv %[source], a6" : [source] "=r"((source)));
    return source;
}

inline void Riscv::write_a6(uint64 source) {
    __asm__ volatile ("mv a6, %[source]" : : [source] "r"((source)));
}

/////////////////////////////////


inline uint64 Riscv::read_a7()
{
    uint64 source;
    __asm__ volatile ("mv %[source], a7" : [source] "=r"((source)));
    return source;
}

inline void Riscv::write_a7(uint64 source) {
    __asm__ volatile ("mv a7, %[source]" : : [source] "r"((source)));
}


#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_RISCV_HPP