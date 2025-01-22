#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdlib.h>
#include <soloscuro/ssi-scmd.h>
#include <soloscuro/ssi-item.h>
//#include <soloscuro/gff-map.h>
#include <soloscuro/psionic.h>
#include <soloscuro/item.h>
#include <soloscuro/stats.h>
#include <soloscuro/state.h>
//#include <soloscuro/entity-animation.h>

extern int sol_entities_init();

extern sol_dude_handle_t sol_entity_create_from_etab(soloscuro_state_t *state, int reg_id, int etab_id);

extern int sol_entity_create_default_human(sol_entity_t **ret);
extern int sol_entity_create(const int add_inventory, sol_entity_t **ret);
extern int sol_entity_create_from_objex(const int id, sol_entity_t **ret);
//extern int sol_entity_create_from_etab(gff_map_object_t *entry_table, uint32_t id, sol_entity_t **ret);
extern int sol_entity_create_clone(sol_entity_t *clone, sol_entity_t **ret);
extern int sol_entity_create_fake(const int mapx, const int mapy, sol_entity_t **ret);
extern int sol_entity_is_fake(sol_entity_t *entity);
extern int sol_entity_free(sol_entity_t *entity);
extern int sol_entity_gui_free(sol_entity_t *entity);
extern int sol_entity_load_from_gff(sol_entity_t *entity, const int gff_idx, const int player, const int res_id);
extern int sol_entity_copy_item(sol_entity_t *entity, sol_item_t *item, const size_t slot);
extern int sol_entity_clear_item(sol_entity_t *entity, const size_t slot);
extern int sol_entity_load_from_object(sol_entity_t *entity, const char *data);
extern int sol_entity_get_total_exp(sol_entity_t *entity, int32_t *exp);
extern int sol_entity_attempt_move(sol_dude_t *dude, const int xdiff, const int ydiff, const int speed);
extern int sol_entity_has_class(const sol_entity_t *entity, const uint16_t class);
extern int sol_entity_get_level(sol_entity_t *entity, const int class, uint8_t *level);
extern int sol_entity_get_wizard_level(sol_entity_t *entity, uint8_t *level);
extern int sol_entity_get_priest_level(sol_entity_t *entity, uint8_t *level);
extern int sol_entity_get_ranger_level(sol_entity_t *entity, uint8_t *level);
extern int sol_entity_has_wizard_slot(sol_entity_t *entity, const int slot);
extern int sol_entity_has_priest_slot(sol_entity_t *entity, const int slot);
extern int sol_entity_take_wizard_slot(sol_entity_t *entity, const int slot);
extern int sol_entity_take_priest_slot(sol_entity_t *entity, const int slot);
extern int sol_entity_distance(const sol_entity_t *source, const sol_entity_t *dest, int16_t *dist);
extern int sol_entity_go(sol_entity_t *dude, const uint16_t x, uint16_t y);
extern int sol_entity_debug(sol_entity_t *dude);
extern int sol_entity_update_scmd(sol_entity_t *entity);
extern int sol_item_set_starting(struct sol_entity_s *pc);
extern int sol_give_ds1_item(struct sol_entity_s *pc, const int slot, const int item_index, const int id);

#endif
