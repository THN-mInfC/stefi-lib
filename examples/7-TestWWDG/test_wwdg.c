#include <libstefi/wwdg.h>
#include <libstefi/util.h>
#include "../../src/internal/rcc_internal.h"
#include "board.h"
#include "button.h"
#include "stdio.h"
#include "ssd1306.h"

uint8_t mode = 1;
char buffer[10];

void s0_action() {
    ssd1306_clear_screen();

    if (mode) {
        sprintf(buffer, "WWDG: off");
        wwdg_set_mode(WWDG_DISABLED);
        ssd1306_putstring( 5, 0, buffer);
        ssd1306_update_screen();
        mode--;
    }
    else {
        sprintf(buffer, "WWDG: on");
        wwdg_set_mode(WWDG_ENABLED);
        ssd1306_putstring( 5, 0, buffer);
        ssd1306_update_screen();
        mode++;
    }
}

void setup() {
    board_init();
    RCC->APB1ENR1 |= BIT(11);
    wwdg_set_upper_bound(0x3F);
    button_interrupt_init(BUTTON_S0, s0_action);
}

void main() {
    setup();

    sprintf(buffer, "WWDG: reset");
    ssd1306_putstring( 5, 0, buffer);
    ssd1306_update_screen();

    while(1) {
        if(mode){
            wwdg_feed_timer(0x3F);
        }
    }
}
