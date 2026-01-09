#include <libstefi/wwdg.h>
#include <libstefi/util.h>
#include "internal/wwdg_internal.h"

wwdg_t *wwdg = WWDG_BASE;


void wwdg_set_mode(const wwdg_status_t mode)
{
    wwdg->CR |= (mode << 7);
    wwdg->CR &= (mode << 7);
}

void wwdg_feed_timer(const uint8_t value)
{
    uint8_t upperBound = wwdg->CFR & 0x3F;
    uint8_t lowerBound = 0xC0;
    assert(value <= upperBound);
    assert(value >= lowerBound);
    wwdg->CR = value | BIT(7);
}

void wwdg_set_upper_bound(const uint8_t value)
{
    assert(value >= 0x3F);
    assert(value <= 0xC0);
    wwdg->CFR = (wwdg->CFR | BIT(7) | BIT(8) | BIT(9)) | value;
}

void wwdg_clear_interrupt()
{
    wwdg->SR && ~BIT(0);
}

void wwdg_set_interrupt_mode(const wwdg_status_t mode){
    wwdg->CFR |= (mode << 7);
    wwdg->CFR &= (mode << 7);
}
