#include <soloscuro/state.h>
#include <soloscuro/entity.h>
#include <stdlib.h>

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
