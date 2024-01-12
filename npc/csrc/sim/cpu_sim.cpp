#include <sim/cpu_sim.hpp>

#include <util/debug.hpp>
#include <util/sim_tool.hpp>
#include <cpu/cpu_mem.hpp>
#include <monitor/cpu_sdb.hpp>
#include <verilated.h>
#include <Vtop.h>
#include <Vtop___024root.h>

npc_state_t npc_state = {1, NPC_STOP, MEM_BASE_ADDR};

void sim()
{
#ifdef NV_SIM
    panic("do not support nvboard");
#else
    reset(3, CYCLE);
    sdb_mainloop();
#endif
}

#define vlSelf top->rootp
word_t pc_in()
{
    return ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__full_adder_2__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__full_adder_2__DOT__s_1))
             << 0x1fU) |
            ((
                 ((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__30__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__30__KET____DOT__full_adder_i__DOT__s_1))
                 << 0x1eU) |
             ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__29__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__29__KET____DOT__full_adder_i__DOT__s_1))
               << 0x1dU) |
              ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__28__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__28__KET____DOT__full_adder_i__DOT__s_1))
                << 0x1cU) |
               ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__27__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__27__KET____DOT__full_adder_i__DOT__s_1))
                 << 0x1bU) |
                ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__26__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__26__KET____DOT__full_adder_i__DOT__s_1))
                  << 0x1aU) |
                 ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__25__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__25__KET____DOT__full_adder_i__DOT__s_1))
                   << 0x19U) |
                  ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__24__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__24__KET____DOT__full_adder_i__DOT__s_1))
                    << 0x18U) |
                   ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__23__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__23__KET____DOT__full_adder_i__DOT__s_1))
                     << 0x17U) |
                    ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__22__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__22__KET____DOT__full_adder_i__DOT__s_1))
                      << 0x16U) |
                     ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__21__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__21__KET____DOT__full_adder_i__DOT__s_1))
                       << 0x15U) |
                      ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__20__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__20__KET____DOT__full_adder_i__DOT__s_1))
                        << 0x14U) |
                       ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__19__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__19__KET____DOT__full_adder_i__DOT__s_1))
                         << 0x13U) |
                        ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__18__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__18__KET____DOT__full_adder_i__DOT__s_1))
                          << 0x12U) |
                         ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__17__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__17__KET____DOT__full_adder_i__DOT__s_1))
                           << 0x11U) |
                          ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__16__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__16__KET____DOT__full_adder_i__DOT__s_1))
                            << 0x10U) |
                           ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__15__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__15__KET____DOT__full_adder_i__DOT__s_1))
                             << 0xfU) |
                            ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__14__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__14__KET____DOT__full_adder_i__DOT__s_1))
                              << 0xeU) |
                             ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__13__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__13__KET____DOT__full_adder_i__DOT__s_1))
                               << 0xdU) |
                              ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__12__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__12__KET____DOT__full_adder_i__DOT__s_1))
                                << 0xcU) |
                               ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__11__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__11__KET____DOT__full_adder_i__DOT__s_1))
                                 << 0xbU) |
                                ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__10__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__10__KET____DOT__full_adder_i__DOT__s_1))
                                  << 0xaU) |
                                 ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__9__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__9__KET____DOT__full_adder_i__DOT__s_1))
                                   << 9U) |
                                  ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__8__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__8__KET____DOT__full_adder_i__DOT__s_1))
                                    << 8U) |
                                   ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__7__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__7__KET____DOT__full_adder_i__DOT__s_1))
                                     << 7U) |
                                    ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__6__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__6__KET____DOT__full_adder_i__DOT__s_1))
                                      << 6U) |
                                     ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__5__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__5__KET____DOT__full_adder_i__DOT__s_1))
                                       << 5U) |
                                      ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__4__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__4__KET____DOT__full_adder_i__DOT__s_1))
                                        << 4U) |
                                       ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__3__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__3__KET____DOT__full_adder_i__DOT__s_1))
                                         << 3U) |
                                        ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__2__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__2__KET____DOT__full_adder_i__DOT__s_1))
                                          << 2U) |
                                         ((((IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT____Vcellinp__genblk1__BRA__1__KET____DOT__full_adder_i__cin) ^ (IData)(vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc__DOT__genblk1__BRA__1__KET____DOT__full_adder_i__DOT__s_1))
                                           << 1U) |
                                          ((0U !=
                                            (4U ^ (IData)(vlSelf->cpu__DOT__inst_num))) &
                                           (vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc_a ^ vlSelf->cpu__DOT__exu_1__DOT__exu_pc_1__DOT__adder_pc_b)))))))))))))))))))))))))))))))));
}
