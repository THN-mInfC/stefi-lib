#pragma once
#include <stdint.h>

typedef enum {
    WWDG_DISABLED = 0, 
    WWDG_ENABLED = 1
} wwdg_status_t;

void wwdg_set_mode(const wwdg_status_t mode);
void wwdg_feed_timer(const uint8_t value);
void wwdg_clear_interrupt();
void wwdg_set_interrupt_mode(const wwdg_status_t mode);
void wwdg_set_upper_bound(const uint8_t value);
