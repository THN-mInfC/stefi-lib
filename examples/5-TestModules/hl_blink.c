#include "pin.h"

void main() {
    pin_init(A5, PIN_OUT);
    pin_set_value(A5, HIGH);
}