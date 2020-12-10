#ifndef _ALTERA_VGA_SUBSYSTEM_H_
#define _ALTERA_VGA_SUBSYSTEM_H_

/*
 * This file was automatically generated by the swinfo2header utility.
 * 
 * Created from SOPC Builder system 'Computer_System' in
 * file 'Computer_System.sopcinfo'.
 */

/*
 * This file contains macros for module 'VGA_Subsystem' and devices
 * connected to the following master:
 *   pixel_dma_master
 * 
 * Do not include this header file and another header file created for a
 * different module or master group at the same time.
 * Doing so may result in duplicate macro names.
 * Instead, use the system header file which has macros with unique names.
 */

/*
 * Macros for device 'SDRAM', class 'altera_avalon_new_sdram_controller'
 * The macros are prefixed with 'SDRAM_'.
 * The prefix is the slave descriptor.
 */
#define SDRAM_COMPONENT_TYPE altera_avalon_new_sdram_controller
#define SDRAM_COMPONENT_NAME SDRAM
#define SDRAM_BASE 0x0
#define SDRAM_SPAN 67108864
#define SDRAM_END 0x3ffffff
#define SDRAM_CAS_LATENCY 3
#define SDRAM_CONTENTS_INFO 
#define SDRAM_INIT_NOP_DELAY 0.0
#define SDRAM_INIT_REFRESH_COMMANDS 2
#define SDRAM_IS_INITIALIZED 1
#define SDRAM_POWERUP_DELAY 100.0
#define SDRAM_REFRESH_PERIOD 15.625
#define SDRAM_REGISTER_DATA_IN 1
#define SDRAM_SDRAM_ADDR_WIDTH 25
#define SDRAM_SDRAM_BANK_WIDTH 2
#define SDRAM_SDRAM_COL_WIDTH 10
#define SDRAM_SDRAM_DATA_WIDTH 16
#define SDRAM_SDRAM_NUM_BANKS 4
#define SDRAM_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_SDRAM_ROW_WIDTH 13
#define SDRAM_SHARED_DATA 0
#define SDRAM_SIM_MODEL_BASE 1
#define SDRAM_STARVATION_INDICATOR 0
#define SDRAM_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_T_AC 5.5
#define SDRAM_T_MRD 3
#define SDRAM_T_RCD 20.0
#define SDRAM_T_RFC 70.0
#define SDRAM_T_RP 20.0
#define SDRAM_T_WR 14.0
#define SDRAM_MEMORY_INFO_DAT_SYM_INSTALL_DIR SIM_DIR
#define SDRAM_MEMORY_INFO_GENERATE_DAT_SYM 1
#define SDRAM_MEMORY_INFO_MEM_INIT_DATA_WIDTH 16

/*
 * Macros for device 'Onchip_SRAM', class 'altera_avalon_onchip_memory2'
 * The macros are prefixed with 'ONCHIP_SRAM_'.
 * The prefix is the slave descriptor.
 */
#define ONCHIP_SRAM_COMPONENT_TYPE altera_avalon_onchip_memory2
#define ONCHIP_SRAM_COMPONENT_NAME Onchip_SRAM
#define ONCHIP_SRAM_BASE 0x8000000
#define ONCHIP_SRAM_SPAN 262144
#define ONCHIP_SRAM_END 0x803ffff
#define ONCHIP_SRAM_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_SRAM_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_SRAM_CONTENTS_INFO ""
#define ONCHIP_SRAM_DUAL_PORT 1
#define ONCHIP_SRAM_GUI_RAM_BLOCK_TYPE AUTO
#define ONCHIP_SRAM_INIT_CONTENTS_FILE Computer_System_Onchip_SRAM
#define ONCHIP_SRAM_INIT_MEM_CONTENT 1
#define ONCHIP_SRAM_INSTANCE_ID NONE
#define ONCHIP_SRAM_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_SRAM_RAM_BLOCK_TYPE AUTO
#define ONCHIP_SRAM_READ_DURING_WRITE_MODE DONT_CARE
#define ONCHIP_SRAM_SINGLE_CLOCK_OP 1
#define ONCHIP_SRAM_SIZE_MULTIPLE 1
#define ONCHIP_SRAM_SIZE_VALUE 262144
#define ONCHIP_SRAM_WRITABLE 1
#define ONCHIP_SRAM_MEMORY_INFO_DAT_SYM_INSTALL_DIR SIM_DIR
#define ONCHIP_SRAM_MEMORY_INFO_GENERATE_DAT_SYM 1
#define ONCHIP_SRAM_MEMORY_INFO_GENERATE_HEX 1
#define ONCHIP_SRAM_MEMORY_INFO_HAS_BYTE_LANE 0
#define ONCHIP_SRAM_MEMORY_INFO_HEX_INSTALL_DIR QPF_DIR
#define ONCHIP_SRAM_MEMORY_INFO_MEM_INIT_DATA_WIDTH 32
#define ONCHIP_SRAM_MEMORY_INFO_MEM_INIT_FILENAME Computer_System_Onchip_SRAM


#endif /* _ALTERA_VGA_SUBSYSTEM_H_ */
