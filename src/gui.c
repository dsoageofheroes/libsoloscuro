#include <gff/gfftypes.h>
#include <gff/gff.h>
#include <soloscuro/gui.h>

extern int sol_win_get_list(soloscuro_state_t *state, sol_id_list_t *list) {
    uint32_t *ids;
    uint32_t len;

    if (!state || !list) {
        goto null_failure;
    }

    if (gff_load_id_list(state->man.ds1.resource, GFF_WIND, &ids, &len) == EXIT_FAILURE) {
        goto ds1_load_failure;
    }

    list->ids = ids;
    list->len = len;

    return EXIT_SUCCESS;

ds1_load_failure:
null_failure:
    return EXIT_FAILURE;
}

extern int sol_win_get(soloscuro_state_t *state, sol_window_t *win) {
    gff_window_t *gff_win;

    if (!state || !win) {
        goto null_failure;
    }

    if (gff_manager_load_window(&(state->man.ds1), win->id, &gff_win) != EXIT_SUCCESS) {
        goto win_load_failure;
    }

    win->x = gff_win->x;
    win->y = gff_win->y;
    win->w = gff_win->frame.width;
    win->h = gff_win->frame.height;

    printf("count: %d\n", gff_win->itemCount);
    uint8_t *buf = (uint8_t*)gff_win;
    for (int i = 0; i < gff_win->itemCount; i++) {
        gff_gui_item_t *item = (gff_gui_item_t*)(buf + sizeof(gff_window_t) + 12 + i *(sizeof(gff_gui_item_t)));
        //args.item = item;
        switch (item->type) {
            case GFF_ACCL:
                //printf("ACCL\n");
                break;
            case GFF_APFM:
                //printf("APFM\n");
                break;
            case GFF_BUTN:
                //printf("BUTN\n");
                break;
            case GFF_EBOX:
                //printf("EBOX\n");
                break;
            default:
                printf("UNKNOWN TYPE IN WINDOW: %d\n", item->type);
        }
    }

    return EXIT_SUCCESS;

win_load_failure:
null_failure:
    return EXIT_FAILURE;
}
