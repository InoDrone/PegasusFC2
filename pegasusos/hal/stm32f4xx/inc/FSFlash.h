/*
 * FSFlash.h
 *
 *  Created on: 11 juin 2014
 *      Author: alienx
 */

#ifndef FSFLASH_H_
#define FSFLASH_H_

#include <stm32f4xx.h>
#include <stm32f4xx_flash.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <cstring>


#define FS_BANK_BASE    0x080C0000      //BANK 10 and 11
#define FS_BANK_SIZE    0x00040000      //BANK SIZE 128 * 2 * 1024 -> 256 Kbytes
#define FS_SECTOR_SIZE  0x00020000      //SECTOR SIZE    128 Kbytes
#define FS_SLOT_SIZE    0x00000100      // 256bytes per slot
#define FS_ARENA_SIZE   0x00020000      // Arena * Slot = 512 * 256 = 128Kbytes
#define FS_COUNT_SLOT   0x00000200      // 512 slot per bank
#define FS_MAGIC        0x12345678

namespace os
{
    namespace hal
    {

        class FSFlash
        {
            public:
                enum STATE {
                    FOUND = 0,
                    SUCCESS = 0,
                    NOT_FOUND = -1,
                    ERROR     = -2,
                    FS_FULL   = -3,
                    IO_ERROR  = -4
                };

                static bool init();
                static STATE save(uint32_t id, uint8_t *datas, uint16_t size);
                static STATE read(uint32_t id, uint8_t *datas, uint16_t size);

            private:
                // Definitions
                enum SlotState {
                    SLOT_STATE_EMPTY    = 0xFFFFFFFF,
                    SLOT_STATE_RESERVED = 0xFEFEFEFE,
                    SLOT_STATE_ACTIVE   = 0xFDFDFDFD,
                    SLOT_STATE_OBSOLETE = 0x00000000
                };

                enum ArenaState {
                    ARENA_STATE_ERASED    = 0xFFFFFFFF,
                    ARENA_STATE_RESERVED  = 0xFEFEFEFE,
                    ARENA_STATE_ACTIVE    = 0xFAFAFAFA,
                    ARENA_STATE_OBSOLETE  = 0x00000000
                };

                struct fsBank {
                    uint32_t start;
                    uint32_t size;
                    uint16_t sector;
                };

                struct SlotHeader {
                        SlotState state;
                        uint32_t id;
                        uint16_t size;
                } __attribute__((packed));

                struct ArenaHeader {
                        uint32_t magic;
                        ArenaState state;
                } __attribute__((packed));

                struct FsState {
                        uint16_t freeSlots;
                        uint16_t activeSlots;
                        bool mounted;
                        uint8_t actArena;
                };

                // Variables
                static fsBank flashBanks[];
                static xSemaphoreHandle mutex;
                static FsState state;

                // Methodes
                static bool lockFlash();
                static bool unlockFlash();

                // ARENA Methodes
                static bool eraseArena(uint8_t arena);
                static bool reserveArena(uint8_t arena);
                static bool activeArena(uint8_t arena);
                static bool obsoleteArena(uint8_t arena);
                static bool mountArena(uint8_t arena);
                static void umountArena();
                static STATE switchBank();
                static int32_t findActiveArena();

                // SLOT Methods
                static uintptr_t getSlotAddr(uint16_t slotId, uint8_t arenaId);
                static STATE reserveFreeSlot(uint32_t id, uint16_t size, SlotHeader* shdr, uint16_t *cslotID);

                // OBJECTS Methods
                static STATE findObject(uint32_t id, SlotHeader* shdr, uint16_t *cslot);
                static STATE deleteObject(uint32_t id);
                static STATE writeObject(uint32_t id, uint8_t *datas, uint16_t size);

                // FLASH Methods
                static STATE readData(uint32_t addr, uint8_t* datas, uint16_t size);
                static STATE writeData(uint32_t addr, uint8_t* datas, uint16_t size);
                static bool getSectorInfo(uint32_t address, uint8_t *sectorNumber, uint32_t *sectorStart, uint32_t *sectorSize);
                static bool copyBytes(uint32_t srcAddr, uint16_t size, uint32_t dstAddr);
        };

    } /* namespace hal */
} /* namespace os */

#endif /* FSFLASH_H_ */
