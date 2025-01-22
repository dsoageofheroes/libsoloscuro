#ifndef SOL_GUI_H
#define SOL_GUI_H

#include <stdlib.h>

#include <soloscuro/state.h>

typedef struct sol_id_list_s {
    uint32_t *ids;
    uint32_t len;
} sol_id_list_t;

typedef struct sol_window_s {
    uint32_t id;
    uint32_t x, y, w, h;
} sol_window_t;

extern int sol_win_get_list(soloscuro_state_t *state, sol_id_list_t *list);
extern int sol_win_get(soloscuro_state_t *state, sol_window_t *win);

#endif
