/******************************************************************************
*						    BT_encodedData
*
*  Author: Marco Esquivel Basaldua (https://github.com/MarcoEsquivelBasaldua)
*
*  Brief: Since Bluetooth commands are managed by Serial, this header only
*         encodes the received data from the Bluetooino app
******************************************************************************/
#ifndef BT_encodedData_H
#define BT_encodedData_H

//-------- BlueTooth Encoded Data --------//
#define BT_FORWARD         (49u)
#define BT_RIGHT           (50u)
#define BT_LEFT            (51u)
#define BT_BACKWARD        (52u)
#define BT_STOP            (53u)
#define BT_FORWARD_LEFT    (54u)
#define BT_FORWARD_RIGHT   (55u)
#define BT_BACKWARD_RIGHT  (56u)
#define BT_BACKWARD_LEFT   (57u)

#define BT_A  (101u)
#define BT_B  (99u)
#define BT_C  (103u)
#define BT_D  (97u)

#define BT_NO_DATA (53u)

////////////////////////////////////////////

#endif
