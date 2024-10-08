/**
 * @file shell_port.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-02-22
 *
 * @copyright (c) 2019 Letter
 *
 */

#include "includes.h"
#include "shell.h"
#include "typedef.h"

Shell g_shell;
char g_shellHistroyBuffer[200];

/**
 * @brief 用户shell写
 *
 * @param data 数据
 */
void userShellWrite(char data)
{
    USART_print_byte(DBG_UART, data);
}

/**
 * @brief 用户shell读
 *
 * @param data 数据
 * @return char 状态
 */
BOOLEAN userShellRead(char *data)
{
    if (USART_received(DBG_UART)) {
        *data = USART_read(DBG_UART);
        return true;
    }
    return false;
}

/**
 * @brief 用户shell初始化
 *
 */
void userShellInit(void)
{
    g_shell.write = userShellWrite;
    g_shell.read = userShellRead;
    shellInit(&g_shell, g_shellHistroyBuffer, sizeof(g_shellHistroyBuffer));
}

static int cmd_Beep(int argc, char *argv[])
{
    int beepNum;

    if (argc == 2) {
        sscanf(argv[1], "%d", &beepNum);
    }
    Beep_Mode((INT8U)beepNum);
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN) | SHELL_CMD_DISABLE_RETURN, beep, cmd_Beep, beep);

static INT8U EEPROM_Buf[1024];

static void eeUsage(void)
{
    //        ee  r/w   addr    len/data
    SHELL_DEBUG(("ee 0 1 100: read from addr = 1, len = 100\n"));
    SHELL_DEBUG(("ee 1 2 0x10 0x20 0x30 : write to addr = 2, data [0x10 0x20 0x30]\n"));
}
#define  EEPROM_TITLE_ROWS_LEN  16
static int cmd_EEPROM(int argc, char *argv[])
{
    INT32U IsRW_, SubAddr, len;

    if (argc >= 2) {
        sscanf(argv[1], "%d,", &IsRW_);
    }
    if (argc >= 3) {
        sscanf(argv[2], "%d,", &SubAddr);
    }
    if (argc >= 4) {
        sscanf(argv[3], "%d,", &len);
    }
    if (len > sizeof(EEPROM_Buf)){
        len = sizeof(EEPROM_Buf);
    }
    switch (IsRW_) {
    case 0:
    case 'r':
    case 'R':
        if (argc != 4){
            SHELL_DEBUG(("para error\n"));
            eeUsage();
            return 0;
        }
        EEP_ReadData(SubAddr, EEPROM_Buf, len);
        // print head row
        SHELL_DEBUG(("       "));
        // 0 1 2 3 ` 15
        for (INT32U i = 0; i < EEPROM_TITLE_ROWS_LEN; i++){
            SHELL_DEBUG(("%o ", i));
        }
        //SHELL_DEBUG(("\r\n"));
        for (INT32U i = 0; i < len; i++){
            // print head column
            if ((i == 0) ||(i+SubAddr) % EEPROM_TITLE_ROWS_LEN == 0){
                INT32U column = (i+SubAddr + 1) / EEPROM_TITLE_ROWS_LEN;
                SHELL_DEBUG(("\r\n%o    ", column));
                RTC_DelayXms(10);
            }
            if (i == 0){ // if not from 0,need alignment
                for (INT32U j = 0; j < SubAddr % EEPROM_TITLE_ROWS_LEN; j++){
                    SHELL_DEBUG(("    ")); // place holder
                }
            }
            SHELL_DEBUG(("%o ", EEPROM_Buf[i]));
        }
        break;
    case 1:
    case 'w':
    case 'W':
        if (argc < 4){
            SHELL_DEBUG(("para error\n"));
            eeUsage();
            return 0;
        }
        for (INT32 i = 0; i < len; i++){
            INT32 tmp;
            sscanf(argv[4 + i], "%d", &tmp);
            EEPROM_Buf[i] = tmp;
        }
        if (EEP_WriteData(SubAddr, EEPROM_Buf, len) == TRUE){
            SHELL_DEBUG(("write success\r\n"));
        }else{
            SHELL_DEBUG(("write failed\r\n"));
        }
        break;
    }
    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN) | SHELL_CMD_DISABLE_RETURN, ee, cmd_EEPROM, read write eeprom);
