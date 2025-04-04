//#include "soloscuro/combat.h"
//#include "soloscuro/debug.h"
#include <soloscuro/alignment.h>
#include <soloscuro/item.h>
//#include "gpl.h"
#include <soloscuro/entity.h>
//#include "innate.h"
//#include "soloscuro/rules.h"
//#include "port.h"
//#include "gfftypes.h"
//#include "gff-map.h"
//#include "region-manager.h"

#include <gff/gff.h>
#include <gff/gfftypes.h>
#include <gff/region.h>

#include <string.h>

#define MAX_ENTITIES (65536)

static uint32_t entity_pos = 0;
static sol_dude_t entities[MAX_ENTITIES];

extern int sol_entities_init() {
    memset(entities, 0x0, sizeof(sol_dude_t) * MAX_ENTITIES);
    entity_pos = 0;
    return EXIT_SUCCESS;
}

extern int sol_entity_valid(sol_dude_handle_t dude) {
    return (dude < MAX_ENTITIES && entities[dude].ds_id != 0);
}

static int is_alloc(sol_dude_handle_t dude) {
    return entities[dude].ds_id != 0;
}

static void allocate_dude(sol_dude_handle_t dude) {
    entities[dude].ds_id = -1;
}

static sol_dude_handle_t create_valid_handle() {
    while (is_alloc(entity_pos) && entity_pos < MAX_ENTITIES) {
        entity_pos++;
    }

    if (!is_alloc(entity_pos)) {
        allocate_dude(entity_pos);
        return entity_pos;
    }

    entity_pos = 0;

    while (entities[entity_pos].ds_id != 0 && entity_pos < MAX_ENTITIES) {
        entity_pos++;
    }

    if (!is_alloc(entity_pos)) {
        allocate_dude(entity_pos);
        return entity_pos;
    }

    return MAX_ENTITIES;
}

extern int sol_entity_free(sol_dude_handle_t dude) {
    if (!sol_entity_valid(dude)) {
        goto invalid;
    }

    // TODO: WARNING WARNING WARNING, need to clear all references!!!

    memset(entities + dude, 0x0, sizeof(sol_dude_t));

    return EXIT_SUCCESS;
    
invalid:
    return EXIT_FAILURE;
}

static char desc[1024];
extern const char* sol_entity_short_description(sol_dude_handle_t dh) {
    sol_dude_t *dude;
    desc[0] = '\0';

    if (!sol_entity_valid(dh)) {
        strcpy(desc, "Invalid entity");
        goto invalid;
    }

    dude = entities + dh;

    if (dude->class[0].class == REAL_CLASS_ITEM) {
       snprintf(desc, 1023, "%s: id: %d qty: %d, val: %d, charges: %d",
               dude->name,
               dude->ds_id,
               dude->item.quantity,
               dude->item.value,
               dude->item.charges
               //dude->item.special,
               //dude->item.slot,
               //dude->item.bonus,
               //dude->item.weapon_type,
               //dude->item.damage_type,
               //dude->item.weight,
               //dude->item.material,
               //dude->item.placement,
               //dude->item.range,
               //dude->item.num_attacks,
               //dude->item.sides,
               //dude->item.dice,
               //dude->item.mod,
               //dude->item.flags,
               //dude->item.legal_class,
               //dude->item.base_AC,
       );
       desc[1023] = '\0'; // guard
    } else {
       snprintf(desc, 1023, "%s: id: %d stats: %d,%d,%d,%d,%d,%d hp:%d, psp:%d",
               dude->name,
               dude->ds_id,
               dude->stats.str,
               dude->stats.dex,
               dude->stats.con,
               dude->stats.intel,
               dude->stats.wis,
               dude->stats.cha,
               dude->stats.hp,
               dude->stats.psp
       );
       desc[1023] = '\0'; // guard
    }

    // intentional fall through
    
invalid:
    return desc;
}

extern sol_dude_handle_t sol_entity_create_from_etab(soloscuro_state_t *state, int reg_id, int etab_id) {
    if (!state || reg_id < 0 || reg_id >= MAX_REGIONS || !state->regions[reg_id].gff_reg.etab) {
        return MAX_ENTITIES;
    }

    if (etab_id < 0 || etab_id >= state->regions[reg_id].gff_reg.num_objects) {
        return MAX_ENTITIES;
    }

    gff_ojff_t ojff;
    sol_dude_handle_t ret = create_valid_handle();
    sol_dude_t *dude = entities + (ret % MAX_ENTITIES);

    gff_etab_object_t *eo = state->regions[reg_id].gff_reg.etab + etab_id;

    //TODO: Account for different Games.
    gff_ojff_read(state->man.ds1.segobjex, eo->index, &ojff);
    dude->x = ojff.xpos;
    dude->y = ojff.ypos;
    dude->z = ojff.zpos;
    dude->bmp = ojff.bmp_id;
    dude->script = ojff.script_id;
    // Ignored:
    //uint16_t flags;
    //int16_t  xoffset;
    //int16_t  yoffset;
    //uint8_t  object_index;
    //printf("(%d, %d, %d) bmp: %d, script: %d\n", ojff.xpos, ojff.ypos, ojff.zpos, dude->bmp, dude->script);
    return ret;
}

extern char *strdup(const char *s); // Not in standard.

/*
static void apply_combat(sol_dude_t *dude, ds1_combat_t *combat) {
    // Not used from combat: char_index, id, read_item_index, weapon_index, pack_index, icon
    //                       ac, move, status, thac0, priority, flags
    dude->stats.hp = combat->hp;
    dude->stats.psp = combat->psp;
    dude->stats.special_defense = combat->special_defense;
    dude->stats.attacks[0].special = combat->special_attack;
    dude->stats.base_thac0 = combat->thac0;
    dude->stats.base_ac = combat->ac;
    dude->allegiance = combat->allegiance;
    dude->stats.str = combat->stats.str;
    dude->stats.dex = combat->stats.dex;
    dude->stats.con = combat->stats.con;
    dude->stats.intel = combat->stats.intel;
    dude->stats.wis = combat->stats.wis;
    dude->stats.cha = combat->stats.cha;
    //if (dude->name) { free (dude->name); }
    //dude->name = strdup(combat->name);
    strncpy(dude->name, combat->name, 32);
    dude->name[31] = '\0';
}
*/

/*
static void apply_character(sol_dude_t *dude, ds_character_t *ch) {
    // Not used from ch: id, legal_class, num_blows, spell_group, psi_group, palette.
    dude->class[0].current_xp = ch->current_xp;
    dude->class[0].high_xp = ch->high_xp;
    dude->stats.hp = ch->base_hp;
    dude->stats.high_hp = ch->high_hp;
    dude->stats.high_psp = ch->base_psp;// TODO: run some tests to make sure this is right.
    dude->race = ch->race;
    dude->gender = ch->gender;
    dude->alignment = ch->alignment;
    dude->stats.str = ch->stats.str;
    dude->stats.dex = ch->stats.dex;
    dude->stats.con = ch->stats.con;
    dude->stats.intel = ch->stats.intel;
    dude->stats.wis = ch->stats.wis;
    dude->stats.cha = ch->stats.cha;
    for (int i = 0; i < 3; i++) {
        dude->class[i].class = ch->real_class[i];
        dude->class[i].level = ch->level[i];
        dude->class[i].high_level = ch->high_level[i];
        dude->stats.attacks[i].number = ch->num_attacks[i];
        dude->stats.attacks[i].num_dice = ch->num_dice[i];
        dude->stats.attacks[i].sides = ch->num_sides[i];
        dude->stats.attacks[i].bonus = ch->num_bonuses[i];
    }
    dude->stats.base_ac = ch->base_ac;
    dude->stats.base_move = ch->base_move;
    dude->stats.magic_resistance = ch->magic_resistance;
    dude->stats.saves.paralysis = ch->saving_throw.paral;
    dude->stats.saves.wand = ch->saving_throw.wand;
    dude->stats.saves.petrify = ch->saving_throw.petr;
    dude->stats.saves.breath = ch->saving_throw.breath;
    dude->stats.saves.spell = ch->saving_throw.spell;
    for (int i = 0; i < 3; i++) {
        dude->stats.attacks[i].number = ch->num_attacks[i];
        dude->stats.attacks[i].num_dice = ch->num_dice[i];
        dude->stats.attacks[i].sides = ch->num_sides[i];
        dude->stats.attacks[i].bonus = ch->num_bonuses[i];
    }
    dude->allegiance = ch->allegiance;
    dude->size = ch->size;
    dude->sound_fx = ch->sound_fx;
    dude->attack_sound = ch->attack_sound;
}
*/

/*
extern int sol_entity_create_default_human(sol_entity_t **ret) {
    sol_entity_t *dude = malloc(sizeof(sol_entity_t));
    if (!dude) { return EXIT_FAILURE; }

    memset(dude, 0x0, sizeof(sol_entity_t));
    dude->stats.hp = 4;
    dude->stats.high_hp = 4;
    dude->race = RACE_HUMAN;
    dude->gender = GENDER_MALE;
    dude->alignment = TRUE_NEUTRAL;
    dude->stats.str = dude->stats.dex = dude->stats.con = dude->stats.intel = dude->stats.wis = dude->stats.cha = 10;
    dude->stats.base_ac = 10;
    dude->stats.base_move = 12;
    dude->stats.saves.paralysis = 20;
    dude->stats.saves.wand = 20;
    dude->stats.saves.petrify = 20;
    dude->stats.saves.breath = 20;
    dude->stats.saves.spell = 20;
    dude->stats.attacks[0].number = 1;
    dude->stats.attacks[0].num_dice = 1;
    dude->stats.attacks[0].sides = 4;
    dude->stats.attacks[1].number = 1;
    dude->stats.attacks[1].num_dice = 1;
    dude->stats.attacks[1].sides = 4;
    dude->class[0].level = -1;
    dude->class[1].level = -1;
    dude->class[2].level = -1;
    //dude->anim.spr = SPRITE_ERROR;
    printf("Need to set default size!\n");
    dude->size = 0;
    sol_inventory_create(&dude->inv);

    *ret = dude;
    return EXIT_SUCCESS;
}
*/

/*
extern int sol_entity_create(const int add_inventory, sol_entity_t **ret) {
    sol_entity_t *dude = malloc(sizeof(sol_entity_t));
    if (!dude) { return EXIT_FAILURE; }
    memset(dude, 0x0, sizeof(sol_entity_t));
    dude->stats.str = dude->stats.dex = dude->stats.con = dude->stats.intel = dude->stats.wis = dude->stats.cha = 10;
    dude->stats.base_ac = 10;
    dude->stats.base_move = 12;
    dude->stats.saves.paralysis = 20;
    dude->stats.saves.wand = 20;
    dude->stats.saves.petrify = 20;
    dude->stats.saves.breath = 20;
    dude->stats.saves.spell = 20;
    //dude->anim.spr = SPRITE_ERROR;
    if (add_inventory) {
        sol_inventory_create(&dude->inv);
    }

    *ret = dude;
    return EXIT_SUCCESS;
}

extern int sol_entity_create_from_objex(const int id, sol_entity_t **ret) {
    sol_dude_t *dude = malloc(sizeof(sol_dude_t));
    char *buf = NULL;
    rdff_disk_object_t *rdff = NULL;
    disk_object_t dobj;
    size_t rdff_pos = 0;

    if (!dude) { return EXIT_FAILURE; }
    memset(dude, 0x0, sizeof(sol_dude_t));
    //dude->anim.spr = SPRITE_ERROR;

    gff_chunk_header_t chunk = gff_find_chunk_header(OBJEX_GFF_INDEX, GFF_RDFF, -1 * id);

    buf = malloc(chunk.length);
    if (!buf) { goto error; }
    if (!gff_read_chunk(OBJEX_GFF_INDEX, &chunk, buf, chunk.length)) {
        printf("unable to get obj from id: %d\n", id);
        goto error;
    }

    chunk = gff_find_chunk_header(OBJEX_GFF_INDEX, GFF_OJFF, -1 * id);
    if (!gff_read_chunk(OBJEX_GFF_INDEX, &chunk, &(dobj), sizeof(disk_object_t))) {
        printf("unable to get obj from id: %d\n", id);
        goto error;
    }

    dude->object_flags = dobj.flags;
    //dude->anim.bmp_id = dobj.bmp_id;
    //dude->anim.xoffset = dobj.xoffset;
    //dude->anim.yoffset = dobj.yoffset;
    dude->mapx = dobj.xpos;
    dude->mapy = dobj.ypos;
    dude->mapz = dobj.zpos;
    dude->ds_id = id;

    rdff = (rdff_disk_object_t*) (buf + rdff_pos);

    while (rdff->load_action != RDFF_END) {
        switch(rdff->type) {
            case GFF_ITEM_OBJECT:
                warn("Item loading from rdff not implemented.\n");
                break;
            case GFF_COMBAT_OBJECT:
                sol_combat_set_scmd(dude, COMBAT_SCMD_STAND_DOWN);
                apply_combat(dude, (ds1_combat_t *) (rdff + 1));
                break;
            case GFF_CHAR_OBJECT:
                apply_character(dude, (ds_character_t*) (rdff + 1));
                break;
            case GFF_ITEM1R_OBJECT:
                warn("Item1r loading from rdff not implemented.\n");
                break;
            case GFF_MINI_OBJECT:
                warn("mini loading from rdff not implemented.\n");
                break;
        }

        rdff_pos += sizeof(rdff_disk_object_t) + rdff->len;
        rdff = (rdff_disk_object_t*) (buf + rdff_pos);
    }

    if (buf) { free(buf); }

    *ret = dude;
    return EXIT_SUCCESS;
error:
    if (dude) { free(dude); }
    if (buf) { free(buf); }
    return EXIT_FAILURE;
}
*/

/*
extern int sol_entity_create_clone(sol_entity_t *clone, sol_entity_t **dude) {
    int status;
    sol_entity_t *ret;
    if ((status = sol_entity_create_from_objex(-1 * abs(clone->ds_id), &ret)) != EXIT_SUCCESS) {
        return status;
    }
    if (!ret) { return EXIT_FAILURE; }

    ret->abilities = clone->abilities;
    ret->mapx = clone->mapx;
    ret->mapy = clone->mapy;
    ret->mapz = clone->mapz;

    *dude = ret;
    return EXIT_SUCCESS;
}
*/

//TODO: Flesh this out. It is currently used for player, but will be use for future entitys;
/*
extern int sol_entity_load_from_object(sol_entity_t *entity, const char *data) {
    sol_item_t *inv = entity->inv;
    //sol_effect_node_t *effects = entity->effects;
    memcpy(entity, data, sizeof(sol_entity_t));
    entity->inv = inv;
    //entity->effects = effects;

    if (!entity->inv) {
        sol_inventory_create(&entity->inv);
    }

    return EXIT_SUCCESS;
}

#define BUF_MAX (1<<14)
extern int sol_entity_load_from_gff(soloscuro_state_t *state, sol_entity_t *entity, const int player, const int res_id) {
    //error("sol_entity_load_from_gff not implemented\n");
    //return EXIT_FAILURE;
    char buf[BUF_MAX];
    gff_rdff_header_t *rdff;
    size_t offset = 0;
    int num_items;
    //ds1_item_t *pc_items = (ds1_item_t*)(entity->inventory);
    gff_chunk_header_t chunk;
   
    gff_find_chunk_header(state->man->ds1.charsave, &chunk, GFF_CHAR, res_id);
    if (gff_read_chunk(state->man->ds1.charsave, &chunk, &buf, sizeof(buf)) < 34) { return EXIT_FAILURE; }

    rdff = (gff_rdff_header_t*) (buf);
    num_items = rdff->blocknum - 2;
    offset += sizeof(gff_rdff_header_t);
    apply_combat(entity, (ds1_combat_t*)(buf + offset));
    offset += rdff->len;

    rdff = (gff_rdff_header_t*) (buf + offset);
    offset += sizeof(gff_rdff_header_t);
    apply_character(entity, (ds_character_t*)(buf + offset));
    offset += rdff->len;

    if (!entity->inv) {
        sol_inventory_create(&entity->inv);
    }

    for (int i = 0; i < num_items; i++) {
        rdff = (gff_rdff_header_t*) (buf + offset);
        offset += sizeof(gff_rdff_header_t);
        int slot = ((ds1_item_t*)(buf + offset))->slot;
        sol_item_convert_from_ds1(state, entity->inv + slot, (ds1_item_t*)(buf + offset));
        //memcpy(pc_items + slot, buf + offset, sizeof(ds1_item_t));
        offset += rdff->len;
    }

    gff_find_chunk_header(state->man->ds1.charsave, &chunk, GFF_PSIN, res_id);
    //if (!gff_read_chunk(gff_idx, &chunk, ds_player_get_psi(player), sizeof(psin_t))) { return; }

    gff_find_chunk_header(state->man->ds1.charsave, &chunk, GFF_SPST, res_id);
    //if (!gff_read_chunk(gff_idx, &chunk, ds_player_get_spells(player), sizeof(spell_list_t))) { return; }

    gff_find_chunk_header(state->man->ds1.charsave, &chunk, GFF_PSST, res_id);
    //if (!gff_read_chunk(gff_idx, &chunk, ds_player_get_psionics(player), sizeof(psionic_list_t))) { return; }
    return EXIT_SUCCESS;
}
*/

/*
*/

extern int sol_entity_copy_item(sol_entity_t *entity, sol_item_t *item, const size_t slot) {
    if (!entity || !item) { return EXIT_FAILURE; }
    if (slot < 0 || slot >= ITEM_SLOT_MAX) {return EXIT_FAILURE;}
    //TODO: Take care of effects!
    //memcpy(entity->inv + slot, item, sizeof(sol_item_t));

    return EXIT_FAILURE;
}

extern int sol_entity_clear_item(sol_entity_t *entity, const size_t slot) {
    if (!entity) { return EXIT_FAILURE; }
    if (slot < 0 || slot >= ITEM_SLOT_MAX) {return EXIT_FAILURE;}
    //memset(entity->inv + slot, 0x0, sizeof(sol_item_t));
    //entity->inv[slot].anim.spr = SPRITE_ERROR;
    return EXIT_FAILURE;
}

extern int sol_entity_get_total_exp(sol_entity_t *entity, int32_t *exp) {
    if (!entity) { return EXIT_FAILURE; }
    uint32_t total_exp = 0;

    for (int i = 0; i < 3; i++) {
        if (entity->class[i].level > -1) {
            total_exp += entity->class[i].current_xp;
        }
    }

    *exp = total_exp;
    return EXIT_SUCCESS;
}

extern int sol_entity_has_class(const sol_entity_t *dude, const uint16_t class) {
    if (!dude) { return EXIT_FAILURE; }
    return (dude &&
        (dude->class[0].class == class
          || dude->class[1].class == class
          || dude->class[2].class == class
          )) ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*
extern int sol_entity_gui_free(sol_entity_t *entity) {
    if (!entity) { return EXIT_FAILURE; }

    if (entity->inv) {
        sol_item_free_inventory(entity->inv);
        entity->inv = NULL;
    }

    //if (entity->anim.spr != SPRITE_ERROR) {
        //sol_status_check(sol_sprite_free(entity->anim.spr), "Unable to free sprite.");
        //entity->anim.spr = SPRITE_ERROR;
    //}

    return EXIT_SUCCESS;
}
*/

    /*
extern int sol_entity_free(sol_entity_t *dude) {
    if (!dude) { return EXIT_FAILURE; }
    sol_region_t *reg;
    sol_entity_list_node_t *node;

    sol_region_manager_get_current(&reg);
    //sol_entity_animation_list_free(&(dude->actions));

    if (reg) {
        sol_entity_animation_list_remove_references(&reg->actions, dude);
        if (sol_entity_list_find(reg->entities, dude, &node) == EXIT_SUCCESS) {
            sol_entity_list_remove(reg->entities, node);
        }
    }

    sol_entity_gui_free(dude);

    if (dude->name) {
        free(dude->name);
        dude->name = NULL;
    }

    free(dude); // Dude has got to be free!
    sol_item_free_inventory(dude->inv);
    if (dude->inv) {
        // TODO: Once effects are added free the effects as well!
        free(dude->inv);
        dude->inv = NULL;
    }
    return EXIT_SUCCESS;
}
    */

/*
extern int sol_entity_update_scmd(sol_entity_t *entity) {
    return sol_sprite_set_frame(entity->anim.spr, entity->anim.scmd[entity->anim.pos].bmp_idx);
}
*/

extern int sol_entity_attempt_move(sol_dude_t *dude, const int xdiff, const int ydiff, const int speed) {
    /*
    sol_combat_region_t          *cr = NULL;
    enum sol_entity_action_e  action;
    sol_region_t             *region;
    sol_entity_t             *target;
    sol_entity_list_node_t   *node;
    int              status;

    if (!dude) { return EXIT_FAILURE; }
    action =
          (xdiff == 1 && ydiff == 1) ? EA_WALK_DOWNRIGHT
        : (xdiff == 1 && ydiff == -1) ? EA_WALK_UPRIGHT
        : (xdiff == -1 && ydiff == -1) ? EA_WALK_UPLEFT
        : (xdiff == -1 && ydiff == 1) ? EA_WALK_DOWNLEFT
        : (xdiff == 1) ? EA_WALK_RIGHT
        : (xdiff == -1) ? EA_WALK_LEFT
        : (ydiff == 1) ? EA_WALK_DOWN
        : (ydiff == -1) ? EA_WALK_UP
        : EA_NONE;

    // If we are in combat and it isn't our turn, do nothing.
    sol_region_manager_get_current(&region);
    status = sol_arbiter_combat_region(region, &cr);
    if (cr && (sol_combat_get_current(cr, &target) != EXIT_SUCCESS || target != dude)) {
        return EXIT_FAILURE;
    }

    printf("%d, %d, %d\n", xdiff, ydiff, sol_region_is_block(region, dude->mapy + ydiff, dude->mapx + xdiff));
    printf("%d, %d, %d\n", xdiff, ydiff, sol_region_location_blocked(region, dude->mapy + ydiff, dude->mapx + xdiff));
    if (sol_region_is_block(region, dude->mapy + ydiff, dude->mapx + xdiff) == EXIT_SUCCESS
        || sol_region_location_blocked(region, dude->mapx + xdiff, dude->mapy + ydiff) == EXIT_SUCCESS) {

        if (cr && (xdiff != 0 || ydiff != 0)) {
            if ((status = sol_combat_attempt_action(cr, dude)) != EXIT_SUCCESS) { return status; }
            sol_region_find_entity_by_location(
                    region, dude->mapx + xdiff, dude->mapy + ydiff, &target);
            if (target) {
                sol_combat_add_attack_animation(region, dude,
                    target, NULL, EA_MELEE);
            }
            // We did a combat action.
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    // We aren't moving...
    if (action == EA_NONE) {
        dude->anim.movex = dude->anim.movey = 0.0;
        if (dude->actions.head == NULL) {
            sol_entity_animation_list_add(&(dude->actions), EA_NONE, dude, NULL, NULL, 1);
        }
        return EXIT_SUCCESS;
    }

    if (sol_entity_animation_list_empty(&(dude->actions)) != EXIT_SUCCESS) {
        // We need to wait for the rest of my actions.
        return EXIT_FAILURE;
    }

    if (cr && (status = sol_combat_attempt_action(cr, dude)) != EXIT_SUCCESS) { return status; }

    sol_status_check(sol_entity_animation_list_add_speed(&(dude->actions), action, dude, NULL,
            NULL, settings_ticks_per_move(), speed, 0), "failed to add animation.");

    //dude->mapx += xdiff;
    //dude->mapy += ydiff;
    //dude->anim.destx += (xdiff * 32);
    //dude->anim.desty += (ydiff * 32);

    if (region) {
        if ((status = sol_entity_list_find(region->entities, dude, &node)) != EXIT_SUCCESS) {
            return status;
        }
        return sol_animate_shift_entity(region->entities, node);
    }

    */
    return EXIT_SUCCESS;
}

extern int sol_entity_has_wizard_slot(sol_entity_t *entity, const int level) {
    if (!entity) { return EXIT_FAILURE; }
    if (level < 0 || level > 10) { return EXIT_FAILURE; }
    return entity->stats.wizard[level].amt ? EXIT_SUCCESS : EXIT_FAILURE;
}

extern int sol_entity_has_priest_slot(sol_entity_t *entity, const int level) {
    if (!entity) { return EXIT_FAILURE; }
    if (level < 0 || level > 10) { return EXIT_FAILURE; }
    return entity->stats.priest[level].amt ? EXIT_SUCCESS : EXIT_FAILURE;
}

extern int sol_entity_take_wizard_slot(sol_entity_t *entity, const int level) {
    if (!entity) { return EXIT_FAILURE; }
    if (level < 0 || level > 10) { return EXIT_FAILURE; }
    if (entity->stats.wizard[level].amt < 1) { return EXIT_FAILURE; }
    entity->stats.wizard[level].amt--;
    return EXIT_SUCCESS;
}

extern int sol_entity_take_priest_slot(sol_entity_t *entity, const int level) {
    if (!entity) { return EXIT_FAILURE; }
    if (level < 0 || level > 10) { return EXIT_FAILURE; }
    if (entity->stats.priest[level].amt < 1) { return EXIT_FAILURE; }
    entity->stats.wizard[level].amt--;
    return EXIT_SUCCESS;
}

extern int sol_entity_get_level(sol_entity_t *entity, const int class, uint8_t *level) {
    int status = EXIT_FAILURE;
    if (!entity) { return EXIT_FAILURE; }
    if (class < 0 || class > 20) { return EXIT_FAILURE; }
    if (entity->class[0].class == class) { *level = entity->class[0].level; status = EXIT_SUCCESS; }
    if (entity->class[1].class == class) { *level = entity->class[1].level; status = EXIT_SUCCESS; }
    if (entity->class[2].class == class) { *level = entity->class[2].level; status = EXIT_SUCCESS; }
    return status;
}

extern int sol_entity_get_wizard_level(sol_entity_t *entity, uint8_t *level) {
    uint8_t max = 0, lvl;
    int status, ret_status = EXIT_FAILURE;

    if (!entity) { return EXIT_FAILURE; }

    if ((status = sol_entity_get_level(entity, REAL_CLASS_PRESERVER, &lvl)) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        ret_status = EXIT_SUCCESS;
    }
    if ((status = sol_entity_get_level(entity, REAL_CLASS_DEFILER, &lvl)) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        ret_status = EXIT_SUCCESS;
    }

    *level = max;
    return ret_status;
}

extern int sol_entity_get_priest_level(sol_entity_t *entity, uint8_t *level) {
    uint8_t max = 0, lvl;
    int status = EXIT_FAILURE;
    if (sol_entity_get_level(entity, REAL_CLASS_AIR_DRUID, &lvl) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        status = EXIT_SUCCESS;
    }
    if (sol_entity_get_level(entity, REAL_CLASS_EARTH_DRUID, &lvl) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        status = EXIT_SUCCESS;
    }
    if (sol_entity_get_level(entity, REAL_CLASS_FIRE_DRUID, &lvl) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        status = EXIT_SUCCESS;
    }
    if (sol_entity_get_level(entity, REAL_CLASS_WATER_DRUID, &lvl) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        status = EXIT_SUCCESS;
    }
    if (sol_entity_get_level(entity, REAL_CLASS_AIR_CLERIC, &lvl) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        status = EXIT_SUCCESS;
    }
    if (sol_entity_get_level(entity, REAL_CLASS_EARTH_CLERIC, &lvl) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        status = EXIT_SUCCESS;
    }
    if (sol_entity_get_level(entity, REAL_CLASS_FIRE_CLERIC, &lvl) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        status = EXIT_SUCCESS;
    }
    if (sol_entity_get_level(entity, REAL_CLASS_WATER_CLERIC, &lvl) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        status = EXIT_SUCCESS;
    }
    *level = max;
    return status;
}

extern int sol_entity_get_ranger_level(sol_entity_t *entity, uint8_t *level) {
    uint8_t max = 0, lvl;
    int status = EXIT_FAILURE;

    if(sol_entity_get_level(entity, REAL_CLASS_AIR_RANGER, &lvl) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        status = EXIT_SUCCESS;
    }
    if(sol_entity_get_level(entity, REAL_CLASS_EARTH_RANGER, &lvl) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        status = EXIT_SUCCESS;
    }
    if(sol_entity_get_level(entity, REAL_CLASS_FIRE_RANGER, &lvl) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        status = EXIT_SUCCESS;
    }
    if(sol_entity_get_level(entity, REAL_CLASS_WATER_RANGER, &lvl) == EXIT_SUCCESS) {
        max = lvl > max ? lvl : max;
        status = EXIT_SUCCESS;
    }
    *level = max;
    return status;
}

extern int sol_entity_create_fake(const int mapx, const int mapy, sol_entity_t **ret) {
    sol_dude_t *dude = calloc(1, sizeof(sol_dude_t));
    if (!dude) { return EXIT_FAILURE; }
    dude->x = mapx;
    dude->y = mapy;
    //dude->anim.spr = SPRITE_ERROR;
    *ret = dude;
    return EXIT_SUCCESS;
}

extern int sol_entity_is_fake(sol_entity_t *entity) {
    return (entity->ds_id == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

extern int sol_entity_distance(const sol_entity_t *source, const sol_entity_t *dest, int16_t *dist) {
    if (!source || !dest) { return EXIT_FAILURE; }

    int dx = abs(source->x - dest->x);
    int dy = abs(source->y - dest->y);

    *dist = dx > dy ? dx : dy;
    return EXIT_SUCCESS;
}

extern int sol_entity_go(sol_entity_t *dude, const uint16_t x, uint16_t y) {
    if (!dude) { return EXIT_FAILURE; }

    //dude->abilities.must_go = 1;
    dude->abilities.args.pos.x = x;
    dude->abilities.args.pos.y = y;
    //printf("%s given go order to (%d, %d)\n", dude->name, x, y);

    return EXIT_FAILURE;
}

extern int sol_entity_debug(sol_entity_t *dude) {
    if (!dude) { return EXIT_FAILURE; }
    //printf("entity ('%s): \n", dude->name);
    printf("entity (): \n");
    printf("    .ds_id = %d\n", dude->ds_id);
    //printf("    .size = %d\n", dude->size);
    //printf("    .race = %d\n", dude->race);
    //printf("    .gender = %d\n", dude->gender);
    //printf("    .alignment = %d\n", dude->alignment);
    //printf("    .allegiance = %d\n", dude->allegiance);
    //printf("    .object_flags = %x\n", dude->object_flags);
    //printf("    .direction = %d\n", dude->direction);
    //printf("    .region = %d\n", dude->region);
    //printf("    pos = (%d, %d, %d)\n", dude->mapx, dude->mapy, dude->mapz);
    printf("    pos = (%d, %d, %d)\n", dude->x, dude->y, dude->z);
    //printf("    .sound_fx = %d\n", dude->sound_fx);
    //printf("    .attack_sound = %d\n", dude->attack_sound);
    //printf("    .combat_status = %d\n", dude->combat_status);
    printf("    stats = (%d, %d %d, %d, %d, %d)\n", dude->stats.str, dude->stats.dex, dude->stats.con, dude->stats.intel, dude->stats.wis, dude->stats.cha);
    printf("    .class[0] = (current_xp = %d, high_xp = %d, class = %d, level = %d, high_level = %d\n",
            dude->class[0].current_xp,
            dude->class[0].high_xp,
            dude->class[0].class,
            dude->class[0].level,
            dude->class[0].high_level);
    printf("    .class[1] = (current_xp = %d, high_xp = %d, class = %d, level = %d, high_level = %d\n",
            dude->class[1].current_xp,
            dude->class[1].high_xp,
            dude->class[1].class,
            dude->class[1].level,
            dude->class[1].high_level);
    printf("    .class[0] = (current_xp = %d, high_xp = %d, class = %d, level = %d, high_level = %d\n",
            dude->class[2].current_xp,
            dude->class[2].high_xp,
            dude->class[2].class,
            dude->class[2].level,
            dude->class[2].high_level);
    //printf("    .anim = (spr = %d, scmd = %p)\n", dude->anim.spr, dude->anim.scmd);
    return EXIT_SUCCESS;
}

/*
static char* racetostr(int race) {
    switch (race) {
        case RACE_MONSTER: return "Monster";
        case RACE_HUMAN: return "Human";
        case RACE_DWARF: return "Dwarf";
        case RACE_ELF: return "Elf";
        case RACE_HALFELF: return "Half-Elf";
        case RACE_HALFGIANT: return "Half-Giant";
        case RACE_HALFLING: return "Halfling";
        case RACE_MUL: return "Mul";
        case RACE_THRIKREEN: return "Thrikreen";
    }
    return "UNKNOWN";
}
*/

extern int sol_dude_print(sol_dude_t *pc) {
    //printf("Dude '%s':\n", pc->name);
    //printf("    race: %s\n", racetostr(pc->race));
    //printf("    gender: %s\n", pc->gender == GENDER_MALE ? "Male" : "Female");
    printf("    str: %d dex: %d con: %d int: %d wis: %d cha: %d\n", pc->stats.str, pc->stats.dex, pc->stats.con, pc->stats.intel, pc->stats.wis, pc->stats.cha);
    return EXIT_SUCCESS;
}

static int ds1item_fill(soloscuro_state_t *state, sol_dude_handle_t dh, ds1_item_t *item) {
    sol_dude_t *dude;
    ds_item1r_t item1r;
    if (!sol_entity_valid(dh)) { return EXIT_FAILURE; }

    dude = entities + dh;

    dude->ds_id = item->id;
    dude->icon = item->icon;
    dude->class[0].class = REAL_CLASS_ITEM;
    if (gff_manager_ds1_read_name(&(state->man), item->name_idx, dude->name)) {
        printf("Error: Unable to read name for item %d\n", item->id);
    }
    dude->item.quantity = item->quantity;
    dude->item.value = item->value;
    dude->item.charges = item->charges;
    dude->item.special = item->special;
    dude->item.slot = item->slot;
    dude->item.bonus = item->bonus;

    // TODO: Are these needed from item?
    //int16_t  next;  // 4, for some internal book keeping.
    //int16_t  pack_index;
    //int16_t  item_index; // Correct, maps into it1r.
    //uint16_t priority;
    //int8_t   data0;

    if (gff_manager_ds1_read_item1r(&(state->man), item->item_index, &item1r)) {
        printf("Error: Unable to read item stats for %d\n", item->id);
    }
    dude->item.weapon_type = item1r.weapon_type;
    dude->item.damage_type = item1r.damage_type;
    dude->item.weight = item1r.weight;
    dude->item.material = item1r.material;
    dude->item.placement = item1r.placement;
    dude->item.range = item1r.range;
    dude->item.num_attacks = item1r.num_attacks;
    dude->item.sides = item1r.sides;
    dude->item.dice = item1r.dice;
    dude->item.mod = item1r.mod;
    dude->item.flags = item1r.flags;
    dude->item.legal_class = item1r.legal_class;
    dude->item.base_AC = item1r.base_AC;
    dude->stats.hp = dude->stats.high_hp = item1r.base_hp;

    return EXIT_SUCCESS;
}

static int ds1combat_fill(sol_dude_handle_t dh, ds1_combat_t *dc) {
    sol_dude_t *dude;
    if (!sol_entity_valid(dh)) { return EXIT_FAILURE; }

    dude = entities + dh;

    strncpy(dude->name, dc->name, SOL_NAME_MAX);
    dude->name[SOL_NAME_MAX-1] = '\0'; // guard
    dude->stats.hp = dc->hp;
    dude->stats.high_hp = dc->hp;
    dude->stats.psp = dc->psp;
    dude->stats.high_psp = dc->psp;
    dude->stats.str = dc->stats.str;
    dude->stats.dex = dc->stats.dex;
    dude->stats.con = dc->stats.con;
    dude->stats.intel = dc->stats.intel;
    dude->stats.wis = dc->stats.wis;
    dude->stats.cha = dc->stats.cha;
    dude->stats.base_ac = dc->ac;
    dude->stats.base_move = dc->move;
    dude->stats.base_thac0 = dc->thac0;
    dude->stats.ds1_status = dc->status;
    dude->stats.special_attack = dc->special_attack;
    dude->stats.special_defense = dc->special_defense;

    dude->ds_id = dc->id;
    dude->icon = dc->icon;
    dude->ds1_flags = dc->flags;

    //TODO: Do these have any useful purpose?
    //dc->priority 
    //dc->char_index; // 4, unconfirmed but looks right.
    //dc->ready_item_index; // 8, to be cleared.
    //dc->weapon_index; // 10, to be cleared
    //dc->pack_index;   // 12, to be cleared
    //dc->data_block[8]; // just to shift down 8 bytes.
    //dc->allegiance;
    //dc-> thac0; // 31, confirmed
    //dc->priority;

    return EXIT_SUCCESS;
}

extern sol_dude_handle_t sol_entity_create(soloscuro_state_t *state, uint32_t id) {
    gff_rdff_t rdff;
    sol_dude_handle_t dude;

    if (!state) {
        goto null_error;
    }

    if (gff_read_rdff(state->man.ds1.segobjex, id, &rdff)) {
        goto read_error;
    }

    if (!sol_entity_valid(dude = create_valid_handle())) {
        goto invalid_dude;
    }

    if (rdff.header.load_action == RDFF_OBJECT && rdff.header.type == GFF_COMBAT_OBJECT) {
        ds1combat_fill(dude, (ds1_combat_t*)rdff.data);
    }

    if (rdff.header.load_action == RDFF_OBJECT && rdff.header.type == GFF_ITEM_OBJECT) {
        ds1item_fill(state, dude, (ds1_item_t*)rdff.data);
    }

    return dude;

invalid_dude:
read_error:
null_error:
    return -1;
}
