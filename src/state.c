#include <soloscuro/state.h>
#include <soloscuro/entity.h>
#include <stdlib.h>

#include <gff/gff.h>
#include <gff/gfftypes.h>

extern soloscuro_state_t* soloscuro_state_create() {
    return calloc(1, sizeof(soloscuro_state_t));
}

extern void soloscuro_state_free(soloscuro_state_t *state) {
}

extern int sol_load_ds1(soloscuro_state_t *state, const char *path) {
    if (state == NULL) { return EXIT_FAILURE; }

    return gff_manager_load_ds1(&(state->man), path);
}

extern int sol_init() {
    if (sol_entities_init()) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static sol_id_list_t get_ids(soloscuro_state_t *state, int rdff_type) {
    uint32_t *tmp = NULL, *rids = NULL;
    uint32_t len = 0;
    uint32_t capacity = 64;
    gff_rdff_t rdff;
    sol_id_list_t ret = {.valid = EXIT_FAILURE, .ids = NULL, len = 0};

    if (state == NULL) { goto null_error; }

    ret.ids = malloc(sizeof(uint32_t) * capacity);
    if (!ret.ids) { goto memory_error; }

    // First load the DS1
    gff_file_t *gff = state->man.ds1.segobjex;
    if (gff) {
        for (int i = 0; i < gff->num_types; i++) {
            gff_chunk_entry_t *entry = gff->chunks[i];
            if (entry->chunk_type == GFF_RDFF) {
                if (gff_load_id_list(gff, entry->chunk_type, &rids, &len)) {
                    continue;
                }

                for (int j = 0; j < len; j++) {
                    if (gff_read_rdff(gff, rids[j], &rdff)) {
                        continue;
                    }
                    if (rdff.header.load_action == RDFF_OBJECT && rdff.header.type == rdff_type) {
                        if (ret.len == capacity) {
                            tmp = realloc(ret.ids, sizeof(uint32_t) * capacity * 1.5);
                            if (!tmp) {
                                goto memory_error;
                            }
                            ret.ids = tmp;
                            capacity *= 1.5;
                        }
                        ret.ids[ret.len++] = rids[j];
                    }
                }

                free(rids);
            }
        }
    }

    if ((tmp = realloc(ret.ids, sizeof(uint32_t) * ret.len))) {
        ret.ids = tmp;
    }

    ret.valid = EXIT_SUCCESS;

    goto success;

memory_error:
    if (ret.ids) {
        free(ret.ids);
        ret.ids = NULL;
    }
null_error:
success:
    return ret;
}

extern sol_id_list_t sol_state_combat_ids(soloscuro_state_t *state) {
    return get_ids(state, GFF_COMBAT_OBJECT);
}

extern sol_id_list_t sol_state_item_ids(soloscuro_state_t *state) {
    return get_ids(state, GFF_ITEM_OBJECT);
}

extern void sol_list_free(sol_id_list_t *list) {
    if (list && list->valid == EXIT_SUCCESS && list->ids) {
        free(list->ids);
        list->ids = NULL;
        list->len = 0;
        list->valid = EXIT_FAILURE;
    }
}
