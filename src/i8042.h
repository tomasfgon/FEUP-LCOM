#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{ i8042
 */

#define DELAY_US    20000

#define KBC_IRQ     1 /**< kbc irq */
#define MOUSE_IRQ   12 /**< mouse irq */

#define KBC_REG     0x64 /**< kbc register */
#define OUT_BUF     0x60 /**< output buffer */

#define PAR_ERR     BIT(7) /**< parity error */
#define TO_ERR      BIT(6) /**< timeout error */
#define AUX         BIT(5) /**< aux flag */
#define INHIBIT     BIT(4) /**< inhibit flag */
#define IB_FULL     BIT(1) /**< input buffer full */
#define OB_FULL     BIT(0) /**< output buffer full */

#define ESC_BC      0x81 /**< esc key */
#define O_BC        0x98 /**< o breakcode */
#define O_MC        0x18 /**< o key */
#define P_BC        0x99 /**< p breakcode */
#define L_BC        0xA6 /**< l breakcode */
#define Q_BC        0x10 /**< q breakcode */
#define A_MC        0x1E /**< a makecode */
#define A_BC        0x9E /**< a breakcode */
#define D_MC        0x20 /**< d makecode */
#define D_BC        0xA0 /**< d breakcode */
#define W_MC        0x11 /**< w makecode */
#define W_BC        0x91 /**< w breakcode */
#define K_MC        0x25 /**< k makecode */
#define K_BC        0xA5 /**< k breakcode */
#define Ç_MC        0x27 /**< ç makecode */
#define Ç_BC        0xA7 /**< ç breakcode */
#define E_MC        0x12 /**< e makecode */
#define E_BC        0x92 /**< e breakcode */
#define I_MC        0x17 /**< i makecode */
#define I_BC        0x97 /**< i breakcode */

// Mouse

#define LB              BIT(0) /**< mouse left button */
#define RB              BIT(1) /**< mouse right button */
#define MB              BIT(2) /**< mouse middle button */
#define X_DELTA         BIT(4) /**< mouse x delta */
#define Y_DELTA         BIT(5) /**< mouse y delta */
#define X_OVF          BIT(6) /**< x overflow */
#define Y_OVF          BIT(7) /**< y overflow */

#define WRITE_TO_MOUSE  0xD4 /**< write to mouse */
#define DIS_DATA_REP    0xF5 /**< disable data report */
#define ENA_DATA_REP    0xF4 /**< enable data report */
#define STREAM_MODE     0xEA /**< mouse stream mode */
#define READ_DATA       0xEB /**< read data */

#define ACK             0xFA /**< ack flag */
#define NACK            0xFE /**< nack flag */
#define ERROR           0xFC /**< error */

/**@}*/

#endif /* _LCOM_I8042_H */
/** @} end of Keyboard */
