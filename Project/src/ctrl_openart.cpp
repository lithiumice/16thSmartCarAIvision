#include "headfile.h"
#include "rtthread.h"
#include "main.h"

using namespace rtthread;
#define DATA_CMD_END '\n'
#define ONE_DATA_MAXLEN 50
#define FORK_REGCO_METHOD 2

uint8 openart_rx_buffer;
lpuart_transfer_t openart_receivexfer;
lpuart_handle_t openart_g_lpuartHandle;
char data[ONE_DATA_MAXLEN];

void cmd_parse()
{
    if (sscanf(data, "%d,%d", &f1.rev_num, &f1.rev_conf) > 0 && f2.sch_sm_state == regco)
    {
        rt_kprintf("parse sucess: %d,%d\n", f1.rev_num, f1.rev_conf);
        f1.rev_ed_msg = 1;

        // if (f1.rev_type == art_rects)
        // {
        //     f1.fruit_pos_num = f1.rev_num;
        //     f1.finish_regco_flag = 1;

        //     for (int i = 9; i >= 1; i--)
        //         f1.friut_pos_list[i] = f1.friut_pos_list[i - 1];
        //     f1.friut_pos_list[0] = f1.fruit_pos_num;

        //     return;
        // }

        if (!f1.finish_regco_flag)
        {
            // for (int i = 9; i >= 1; i--)
            //     f1.rev_num_list[i] = f1.rev_num_list[i - 1];
            // f1.rev_num_list[0] = f1.rev_num;
            push_int_pull(f1.rev_num_list, 10, f1.rev_num);

            // if (f1.rev_num_list[0] == f1.rev_num_list[1] &&
            //     f1.rev_num_list[0] == f1.rev_num_list[2])
            // {
                // if (f1.rev_type == art_qrcode)
                //     f1.aptg_regco_num = f1.rev_num_list[0];
                // else if (f1.rev_type == art_fork_num)
                //     f1.fork_regco_num = f1.rev_num_list[0];
                // else if (f1.rev_type == art_anifuit)
                //     f1.anifruit_regco_num = f1.rev_num_list[0];
                f1.regco_num = f1.rev_num_list[0];
                f1.finish_regco_flag = 1;
            // }
        }
    }
    else if (strcmp("ok\n", data) == 0)
    {
        f1.rev_ok_flag = 1;
        rt_kprintf("received ack from openart.\n");
    }
    else
    {
        rt_kprintf("data parse failed:%s\n", data);
    }
}

void openart_callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    static char i = 0;
    if (kStatus_LPUART_RxIdle == status)
    {
        if (openart_rx_buffer == DATA_CMD_END)
        {
            data[i++] = '\0';
            cmd_parse();
            i = 0;
            goto exit;
        }
        i = (i >= ONE_DATA_MAXLEN - 1) ? ONE_DATA_MAXLEN - 1 : i;
        data[i++] = openart_rx_buffer;
    }

exit:
    handle->rxDataSize = openart_receivexfer.dataSize;
    handle->rxData = openart_receivexfer.data;
}

void init_openart()
{
    uart_init(USART_3, 115200, UART3_TX_C8, UART3_RX_C9);
    openart_receivexfer.dataSize = 1;
    openart_receivexfer.data = &openart_rx_buffer;
    uart_set_handle(USART_3, &openart_g_lpuartHandle, openart_callback, NULL, 0, openart_receivexfer.data, 1);
    NVIC_SetPriority(LPUART3_IRQn, 5); //设置串口中断优先级 范围0-15 越小优先级越高
    uart_rx_irq(USART_3, 1);
}
