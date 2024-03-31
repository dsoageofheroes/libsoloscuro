#ifndef SSI_OBJECT_H
#define SSI_OBJECT_H

#include <stdint.h>
#include <gff/object.h>
#include <soloscuro/ssi-item.h>
#include <soloscuro/ssi-scmd.h>

#define COMBAT_NAME_SIZE (18)

/*
typedef struct _ds_character_s {
    uint32_t current_xp; // confirmed
    uint32_t high_xp;    // probable: repsents how many more exp to go to next level?
    uint16_t base_hp;    // confirmed, natural top hp
    uint16_t high_hp;    // probable: maybe highest hp?
    uint16_t base_psp;   // confirmed
    uint16_t id;         // ? seems to be 1000 or 2000
    uint8_t data1[2];    // ? second byte is always 128 (maybe a negative?)
    uint16_t legal_class;// confirmed
    uint8_t data2[4];    //always 0....
    uint8_t race;        // confirmed
    uint8_t gender;      // confirmed
    uint8_t alignment;   // confirmed
    ds_stats_t stats;    // confirmed
    int8_t real_class[3]; // confirmed
    uint8_t level[3];      // confirmed
    int8_t base_ac;      //confirmed
    uint8_t base_move;      //confirmed
    uint8_t magic_resistance;      //confirmed
    uint8_t num_blows;     //confirmed, blow type?
    uint8_t num_attacks[3]; //confirmed, natural number of attacks
    uint8_t num_dice[3];    //confirmed, #dice for natural attacks
    uint8_t num_sides[3];   //confirmed, #sides for natural attacks
    uint8_t num_bonuses[3]; //confirmed, bonuses for natural attacks
    ds_saving_throw_t saving_throw; //confirmed
    uint8_t allegiance;    // ? always 1
    uint8_t size;          // ? always 0
    uint8_t spell_group;   // ? always 0
    uint8_t high_level[3]; // confirmed, highest possible level (for level drain.)
    uint16_t sound_fx;     // ? always 0
    uint16_t attack_sound;     // ? always 0
    uint8_t psi_group;     // ? always 0
    uint8_t palette;       // confirmed, 2 which is hardcoded
} __attribute__ ((__packed__)) ds_character_t;
*/

#endif
