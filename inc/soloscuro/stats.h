#ifndef SOL_STATS_H
#define SOL_STATS_H

#include <soloscuro/state.h>

// Keeping the old interface for now...
extern int8_t sol_dnd2e_melee_damage_mod(const sol_stats_t *stats);
extern int8_t sol_dnd2e_melee_hit_mod(const sol_stats_t *stats);
extern int8_t sol_dnd2e_range_damage_mod(const sol_stats_t *stats);
extern int8_t sol_dnd2e_range_hit_mod(const sol_stats_t *stats);
extern int8_t sol_dnd2e_reaction_mod(const sol_stats_t *stats);
extern int8_t sol_dnd2e_ac_mod(const sol_stats_t *stats);
extern int8_t sol_dnd2e_hp_mod(const sol_stats_t *stats);
extern int8_t sol_dnd2e_shock_mod(const sol_stats_t *stats);
extern int8_t sol_dnd2e_res_mod(const sol_stats_t *stats);
extern int8_t sol_dnd2e_poison_mod(const sol_stats_t *stats);
extern int8_t sol_dnd2e_regen_mod(const sol_stats_t *stats);
extern int8_t sol_dnd2e_stats_valid(const sol_stats_t *stats);

#endif
