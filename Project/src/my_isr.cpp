#include "main.h"
#include "zf_pit.h"

// void PIT_IRQHandler(void)
// {
//     rt_interrupt_leave();
//     if (PIT_FLAG_GET(PIT_CH0))
//     {
//         PIT_FLAG_CLEAR(PIT_CH0);
//     }

//     if (PIT_FLAG_GET(PIT_CH1))
//     {
//         PIT_FLAG_CLEAR(PIT_CH1);
//     }

//     if (PIT_FLAG_GET(PIT_CH2))
//     {
//         PIT_FLAG_CLEAR(PIT_CH2);
//     }

//     if (PIT_FLAG_GET(PIT_CH3))
//     {
//         PIT_FLAG_CLEAR(PIT_CH3);
//     }

//     __DSB();
//     rt_interrupt_enter();
// }