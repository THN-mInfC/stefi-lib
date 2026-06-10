#pragma once

/*
 * System clock frequency assumed by the SysTick tick macros
 * (see src/internal/systick_internal.h). Formerly libstefi/board.h;
 * renamed to avoid confusion with the STefi board definition in
 * boards/board.h.
 */
#define SYSCLK (4000000U)
