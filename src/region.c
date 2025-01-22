#include <soloscuro/entity.h>
#include <soloscuro/region.h>

static int region_loaded(gff_region_t *reg) {
    if (!reg) { return 0; }

    return reg->tile_ids != NULL;
}

extern int sol_region_load(soloscuro_state_t *state, uint32_t id) {
    if (!state || id >= MAX_REGIONS) { return EXIT_FAILURE; }

    sol_region_t *reg = state->regions + id;

    if (region_loaded(&(reg->gff_reg))) { return EXIT_FAILURE; }

    // TODO: Account for different games.
    if (id > DS1_MAX_REGIONS) { return EXIT_FAILURE; }
    if (gff_region_read(state->man.ds1.regions[id], &(reg->gff_reg))) {
        goto gff_read_failure;
    }

    for (int i = 0; i < reg->gff_reg.num_objects; i++) {
        reg->dudes[reg->dude_num++] = sol_entity_create_from_etab(state, id, i);
        if (reg->dude_num >= MAX_DUDES_PER_REGION) {
            printf("ERROR loading region!\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;

gff_read_failure:
    return EXIT_FAILURE;
}
