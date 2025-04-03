#ifndef SOL_STATE_H
#define SOL_STATE_H

#include <gff/manager.h>

typedef struct sol_combat_round_stats_s {
    int8_t move;
    int8_t initiative;
    int8_t attack_num;
    int8_t has_cast;
    int8_t has_melee;
    int8_t has_ranged;
} sol_combat_round_stats_t;

typedef struct sol_innate_attack_s {
    uint8_t number;
    uint8_t num_dice;
    uint8_t sides;
    uint8_t bonus;
    uint32_t special;
} sol_innate_attack_t;

typedef struct sol_saving_throws_s {
    uint8_t paralysis;
    uint8_t wand;
    uint8_t petrify;
    uint8_t breath;
    uint8_t spell;
} sol_saving_throws_t;

typedef struct sol_slots_s {
    uint16_t amt, max;
} sol_slots_t;

typedef struct sol_stats_s {
    uint8_t str;
    uint8_t dex;
    uint8_t con;
    uint8_t intel;
    uint8_t wis;
    uint8_t cha;
    int16_t hp;
    int16_t high_hp;
    int16_t psp;
    int16_t high_psp;
    int8_t base_ac; // before applying any modifiers.
    int8_t base_move;
    int8_t base_thac0;
    int8_t ds1_status;
    uint8_t special_attack;
    uint8_t special_defense;
    sol_slots_t wizard[10];
    sol_slots_t priest[10];
    uint8_t magic_resistance;
    sol_saving_throws_t saves;
} sol_stats_t;


/*
typedef struct sol_psi_abilities_s {
    unsigned int psychokinesis    : 1;
    unsigned int psychometabolism : 1;
    unsigned int telepathy        : 1;
    unsigned int clairsentient    : 1; // future
    unsigned int psychoportive    : 1; // future
    unsigned int metapsionic      : 1; // future
    unsigned int detonate              : 1;
    unsigned int disintegrate          : 1;
    unsigned int ballistic_attack      : 1;
    unsigned int control_body          : 1;
    unsigned int iniertial_barrier     : 1;
    unsigned int annimal_affinity      : 1;
    unsigned int energy_containment    : 1;
    unsigned int life_draining         : 1;
    unsigned int absorb_disease        : 1;
    unsigned int adrenalin_control     : 1;
    unsigned int biofeedback           : 1;
    unsigned int body_weaponry         : 1;
    unsigned int cell_adjustment       : 1;
    unsigned int displacement          : 1;
    unsigned int enhanced_strength     : 1;
    unsigned int flesh_armor           : 1;
    unsigned int graft_weapon          : 1;
    unsigned int lend_health           : 1;
    unsigned int share_strength        : 1;
    unsigned int domination            : 1;
    unsigned int mas_domination        : 1;
    unsigned int psycic_crush          : 1;
    unsigned int superior_invisibility : 1;
    unsigned int tower_of_iron_will    : 1;
    unsigned int ego_whip              : 1;
} sol_psi_abilities_t;
*/

typedef struct sol_psin_s {
    uint8_t types[7];
} sol_psin_t;

typedef struct sol_psionic_list_s {
    uint8_t psionics[34];
} sol_psionic_list_t;


#define MAX_REGIONS (256)
#define MAX_DUDES_PER_REGION (1024)

typedef struct sol_ability_set_s {
    //unsigned int hunt             : 1; unsigned int attackable       : 1;
    //unsigned int talkable         : 1;
    //unsigned int must_go          : 1; // given order to go to x,y
    //sol_psi_abilities_t psi;
    //struct spell_abilities_s spells; // NOT shown
    //struct class_abilities_s class;  // Not shown
    union {
        struct {
            uint16_t x, y;
        } pos;
    } args;
} sol_ability_set_t;

typedef struct sol_class_s {
    uint32_t current_xp;
    uint32_t high_xp;    // for level drain.
    int8_t class;
    int8_t level;
    uint8_t high_level; // for level drain.
    //sol_psi_abilities_t psi;
} sol_class_t;

typedef struct sol_item_state_s {
    uint16_t quantity;
    uint16_t value;
    uint16_t charges;
    uint8_t  special;
    uint8_t  slot;
    int8_t   bonus;
    uint8_t  weapon_type;
    uint16_t damage_type;
    uint8_t  weight;
    uint8_t  material;
    uint8_t  placement;
    uint8_t  range;// Need to confirm
    uint8_t  num_attacks;
    uint8_t  sides;
    uint8_t  dice;
    int8_t   mod;
    uint8_t  flags;
    uint16_t legal_class;
    int8_t   base_AC;
} sol_item_state_t;

typedef struct sol_combat_state_s {
    sol_innate_attack_t attacks[3];
    sol_combat_round_stats_t combat;
} sol_combat_state_t;

#define SOL_NAME_MAX (40)

typedef struct sol_entity_s {
    /*
    char *name;
    int16_t ds_id;     // This is the darksun/GPL id
    uint8_t size;
    uint8_t race;
    uint8_t gender;
    uint8_t alignment;
    int8_t allegiance;
    int8_t object_flags;
    int8_t map_flags;
    int8_t direction; // Left, Right, Up, Down.
    uint16_t region;
    uint16_t mapx;      // object's x position in the region
    uint16_t mapy;      // object's y position in the region
    int16_t mapz;       // object's z position in the region
    uint16_t sound_fx;
    uint16_t attack_sound;
    uint16_t combat_status;
    //animate_sprite_t anim;
    //struct sol_entity_animation_list_s actions;
    sol_effect_node_t *effects; // anything currently affecting the entity.
    sol_item_t *inv; // NULL means that there is no inventory on this entity (IE: some monsters.)
    uint32_t cp;
    //spell_list_t *spells;
    //psionic_list_t *psionics;
    */
    char name[SOL_NAME_MAX];
    uint16_t x, y, z, ds_id;
    uint16_t bmp, script;
    int16_t icon;
    uint8_t ds1_flags;
    sol_class_t class[3];
    sol_stats_t stats;
    sol_ability_set_t abilities;
    union {
        sol_item_state_t item;
        sol_combat_state_t combat;
    };
} sol_entity_t;


typedef uint32_t sol_entity_handle_t;

// For the lolz
typedef sol_entity_t sol_dude_t;
typedef sol_entity_handle_t sol_dude_handle_t;

typedef struct sol_region_s {
    gff_region_t gff_reg;
    uint16_t dude_num;
    sol_dude_handle_t dudes[MAX_DUDES_PER_REGION];
} sol_region_t;

typedef struct soloscuro_gui_window_entry_s {
    int32_t    win_id;
} soloscuro_gui_window_entry_t;

typedef struct soloscuro_gui_state_s {
    int32_t    win_pos;
    int32_t    last_selection;
    soloscuro_gui_window_entry_t win_stack[8];
    //orxOBJECT *ebox;
} soloscuro_gui_state_t;

typedef struct soloscuro_state_s {
    soloscuro_gui_state_t gui;
    gff_manager_t man;
    sol_region_t regions[MAX_REGIONS];
} soloscuro_state_t;

extern soloscuro_state_t* soloscuro_state_create();
extern void               soloscuro_state_free(soloscuro_state_t *state);
extern int                sol_load_ds1(soloscuro_state_t *state, const char *path);
extern int                sol_init();

typedef struct sol_id_list_s {
    int valid;
    uint32_t *ids;
    uint32_t len;
} sol_id_list_t;

extern void               sol_list_free(sol_id_list_t *list);
extern sol_id_list_t      sol_state_combat_ids(soloscuro_state_t *state);
extern sol_id_list_t      sol_state_item_ids(soloscuro_state_t *state);

#endif
