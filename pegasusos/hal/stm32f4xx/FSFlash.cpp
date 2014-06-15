/*
 * FSFlash.cpp
 *
 *  Created on: 11 juin 2014
 *      Author: alienx
 */

#include "inc/FSFlash.h"


namespace os
{
    namespace hal
    {
        FSFlash::fsBank FSFlash::flashBanks[] = {
                { 0x08000000, 16 * 1024, FLASH_Sector_0},
                { 0x08004000, 16 * 1024, FLASH_Sector_1},
                { 0x08008000, 16 * 1024, FLASH_Sector_2},
                { 0x0800C000, 16 * 1024, FLASH_Sector_3},
                { 0x08010000, 64 * 1024, FLASH_Sector_4},
                { 0x08020000, 128 * 1024, FLASH_Sector_5},
                { 0x08040000, 128 * 1024, FLASH_Sector_6},
                { 0x08060000, 128 * 1024, FLASH_Sector_7},
                { 0x08080000, 128 * 1024, FLASH_Sector_8},
                { 0x080A0000, 128 * 1024, FLASH_Sector_9},
                { 0x080C0000, 128 * 1024, FLASH_Sector_10},
                { 0x080E0000, 128 * 1024, FLASH_Sector_11}
        };

        xSemaphoreHandle FSFlash::mutex;
        FSFlash::FsState FSFlash::state;


        bool FSFlash::init()
        {
            bool ret = true;
            state.mounted = false;
            mutex = xSemaphoreCreateMutex();

            if (!unlockFlash()) {
                return false;
            }

            int8_t arenaId = -1;
            bool found = false;
            arenaId = findActiveArena();
            if (arenaId >= 0) {
                found = true;
            } else {
                if (!eraseArena(0)) {
                    ret = false;
                    goto goto_exit;
                }

                if (!activeArena(0)) {
                    ret = false;
                    goto goto_exit;
                }

                found = true;
                arenaId = 0;
            }

            if (!found) {
                ret = false;
                goto goto_exit;
            }

            if (!mountArena(arenaId)) {
                ret = false;
                goto goto_exit;
            }

goto_exit:
            lockFlash();

            return ret;
        }

        /**
         * Save Object to Flash
         */
        FSFlash::STATE FSFlash::save(uint32_t id, uint8_t *datas, uint16_t size)
        {
            STATE ret = SUCCESS;

            if (!unlockFlash()) return ERROR;

            if (!deleteObject(id)) {
                ret = ERROR;
                goto goto_exit;
            }

            if (state.freeSlots == 0) {
                if (switchBank() != SUCCESS) {
                    ret = ERROR;
                    goto goto_exit;
                }

                if (state.freeSlots == 0) {
                    ret = FS_FULL;
                    goto goto_exit;
                }
            }

            if (writeObject(id, datas, size) != SUCCESS) {
                ret = ERROR;
                goto goto_exit;
            }

goto_exit:
            lockFlash();

            return ret;

        }


        FSFlash::STATE FSFlash::read(uint32_t id, uint8_t *datas, uint16_t size)
        {
            STATE ret = SUCCESS;

            if (!unlockFlash()) return ERROR;

            uint16_t slotId = 0;
            SlotHeader shdr;
            if (findObject(id, &shdr, &slotId) != SUCCESS) {
                ret = ERROR;
                goto goto_exit;
            }

            if (shdr.size != size) {
                ret = ERROR;
                goto goto_exit;
            }

            if (size > 0) {
                uintptr_t addr = getSlotAddr(slotId, state.actArena);
                if (readData(addr + sizeof(shdr), (uint8_t *)datas, size) != SUCCESS) {
                    ret = IO_ERROR;
                    goto goto_exit;
                }
            }

goto_exit:
            lockFlash();

            return ret;
        }

        /**
         * Get slot addr
         */
        uintptr_t FSFlash::getSlotAddr(uint16_t slotId, uint8_t arenaId) {

            return FS_BANK_BASE + (arenaId * FS_ARENA_SIZE) + (slotId * FS_SLOT_SIZE);
        }

        /**
         * Read data in flash
         */
        FSFlash::STATE FSFlash::readData(uint32_t addr, uint8_t* datas, uint16_t size) {
            uint8_t sectorNumber;
            uint32_t sectorStart;
            uint32_t sectorSize;

            /* Ensure that the base address is in a valid sector */
            if (!getSectorInfo(addr,
                                 &sectorNumber,
                                 &sectorStart,
                                 &sectorSize)) {
                /* We're asking for an invalid flash address */
                return ERROR;
            }

            /* Ensure that the entire read occurs within the same sector */
            if ((uintptr_t)addr + size > sectorStart + sectorSize) {
                /* Read crosses the end of the sector */
                return ERROR;
            }

            /* Read the data into the buffer directly */
            memcpy(datas, (void *)addr, size);

            return SUCCESS;
        }

        /**
         * Write data in flash
         */
        FSFlash::STATE FSFlash::writeData(uint32_t addr, uint8_t* datas, uint16_t size)
        {
            uint8_t sectorNumber;
            uint32_t sectorStart;
            uint32_t sectorSize;

            /* Ensure that the base address is in a valid sector */
            if (!getSectorInfo(addr,
                                 &sectorNumber,
                                 &sectorStart,
                                 &sectorSize)) {
                /* We're asking for an invalid flash address */
                return ERROR;
            }

            /* Ensure that the entire read occurs within the same sector */
            if ((uintptr_t)addr + size > sectorStart + sectorSize) {
                /* Read crosses the end of the sector */
                return ERROR;
            }

            FLASH_Status status;
            for (uint16_t i = 0; i < size / 4; i++) {
                uint32_t data_word = *(uint8_t *)(datas + i * 4);
                status = FLASH_ProgramWord(addr + i * 4, data_word);
                if (status != FLASH_COMPLETE) {
                    return ERROR;
                }
            }

            /* Write the data */
            for (uint16_t i = size - size % 4; i < size; i++) {
                status = FLASH_ProgramByte(addr + i, datas[i]);
                if (status != FLASH_COMPLETE) {
                    return ERROR;
                }
            }

            return SUCCESS;
        }
        /**
         * Find valid object
         */
        FSFlash::STATE FSFlash::findObject(uint32_t id, SlotHeader* shdr, uint16_t *cslot) {
            if (*cslot == 0) {
                *cslot = 1;
            }

            for (uint16_t slotId = *cslot; slotId < FS_COUNT_SLOT; slotId++) {

                uintptr_t slotAddr = getSlotAddr(slotId, state.actArena);

                if (readData(slotAddr, (uint8_t *)shdr, sizeof(*shdr)) != SUCCESS) {
                    return ERROR;
                }

                if (shdr->state == SLOT_STATE_EMPTY) {
                    break;
                }

                if (shdr->state == SLOT_STATE_ACTIVE &&
                        shdr->id == id) {
                    *cslot = slotId;
                    return FOUND;
                }
            }

            return NOT_FOUND;
        }

        FSFlash::STATE FSFlash::deleteObject(uint32_t id)
        {
            SlotHeader shdr;
            uint16_t slotId = 0;

            if (findObject(id, &shdr, &slotId) == FOUND) {
                shdr.state = SLOT_STATE_OBSOLETE;
                uintptr_t addr = getSlotAddr(slotId, state.actArena);
                if (writeData(addr, (uint8_t *)&shdr, sizeof(shdr)) != SUCCESS) {
                    return ERROR;
                }

                return SUCCESS;
            }

            return NOT_FOUND;
        }

        FSFlash::STATE FSFlash::writeObject(uint32_t id, uint8_t *datas, uint16_t size) {
            SlotHeader shdr;
            uint16_t slotId;

            if (reserveFreeSlot(id, size, &shdr, &slotId) != SUCCESS) {
                return ERROR;
            }

            uintptr_t addr = getSlotAddr(slotId, state.actArena);
            uintptr_t sOffset = sizeof(shdr);
            uint8_t retries = 0;
            STATE status;
            do {
                status = writeData(addr + sOffset, datas, size);
                retries++;
            } while (status != SUCCESS && retries < 5);

            if (retries == 5) {
                return IO_ERROR;
            }

            shdr.state = SLOT_STATE_ACTIVE;
            if (writeData(addr, (uint8_t*)&shdr, sizeof(shdr)) != SUCCESS) {
                return IO_ERROR;
            }

            state.activeSlots++;

            return SUCCESS;
        }

        FSFlash::STATE FSFlash::reserveFreeSlot(uint32_t id, uint16_t size, SlotHeader* shdr, uint16_t *csSlotId)
        {
            if (state.freeSlots < 1) {
                return FS_FULL;
            }

            if ( size > (FS_SLOT_SIZE - sizeof(shdr))) {
                return ERROR;
            }

            uint16_t sSlotId = FS_COUNT_SLOT - state.freeSlots;
            uintptr_t addr = getSlotAddr(sSlotId, state.actArena);

            if (readData(addr, (uint8_t *)shdr, sizeof(*shdr)) != SUCCESS) {
                return IO_ERROR;
            }

            if (shdr->state != SLOT_STATE_EMPTY) {
                return ERROR;
            }

            //shdr->state = SLOT_STATE_RESERVED;
            shdr->id = id;
            shdr->size = size;

            if (writeData(addr, (uint8_t*)shdr, sizeof(*shdr)) != SUCCESS) {
                return IO_ERROR;
            }

            state.freeSlots--;
            *csSlotId = sSlotId;

            return SUCCESS;
        }

        /**
         * Get bank informations
         */
        bool FSFlash::getSectorInfo(uint32_t address, uint8_t *sectorNumber, uint32_t *sectorStart, uint32_t *sectorSize)
        {
            uint16_t length = sizeof(flashBanks) / sizeof((flashBanks)[0]);
            for (uint8_t i = 0; i < length; i++) {
                fsBank *banks = &flashBanks[i];
                if ((address >= banks->start) &&
                    (address < (banks->start + banks->size))) {
                    /* address lies within this sector */
                    *sectorNumber = banks->sector;
                    *sectorStart  = banks->start;
                    *sectorSize   = banks->size;
                    return true;
                }
            }

            return false;
        }

        /**
         * Copy data to next bank et mark ACTIVE
         */
        FSFlash::STATE FSFlash::switchBank()
        {
            uint8_t src = state.actArena;
            uint8_t dst = ( state.actArena + 1 ) % (FS_BANK_SIZE / FS_ARENA_SIZE);

            /* Erase dst bank */
            if (!eraseArena(dst)) {
                return ERROR;
            }

            /* Reserve DST BANK */
            if (!reserveArena(dst)) {
                return ERROR;
            }

            /* Copy active slot to destination arena */
            uint16_t dstSlotId = 1;
            for (uint16_t srcSlotId = 1; srcSlotId < FS_COUNT_SLOT; srcSlotId++) {
                SlotHeader shdr;
                uintptr_t srcAddr = getSlotAddr(srcSlotId, src);
                if (readData(srcAddr, (uint8_t*)&shdr, sizeof(shdr)) != SUCCESS) {
                    return IO_ERROR;
                }

                if (shdr.state == SLOT_STATE_ACTIVE) {
                    uintptr_t dstAddr = getSlotAddr(dstSlotId, dst);
                    if (!copyBytes(srcAddr, sizeof(shdr) + shdr.size, dstAddr)) {
                        return ERROR;
                    }
                    dstSlotId++;
                }
            }

            /* Active bank */
            if (!activeArena(dst)) {
                return ERROR;
            }

            /* Umount act bank */
            umountArena();

            /* Mark obsolete */
            if (!obsoleteArena(src)) {
                return ERROR;
            }

            /* Mount new bank */
            if (!mountArena(dst)) {
                return ERROR;
            }

            return SUCCESS;

        }

        bool FSFlash::eraseArena(uint8_t arena) {
               uintptr_t arenaAddr = FS_BANK_BASE + (arena * FS_ARENA_SIZE);

               uint8_t sectorNumber;
               uint32_t sectorStart;
               uint32_t sectorSize;

               /* Ensure that the base address is in a valid sector */
               if (!getSectorInfo(arenaAddr,
                                    &sectorNumber,
                                    &sectorStart,
                                    &sectorSize)) {
                   /* We're asking for an invalid flash address */
                   return false;
               }

               if (FLASH_EraseSector(sectorNumber, VoltageRange_3) != FLASH_COMPLETE) {
                   return false;
               }

               ArenaHeader ahdr = {
                       FS_MAGIC,
                       ARENA_STATE_ERASED
               };

               if (writeData(arenaAddr, (uint8_t*)&ahdr, sizeof(ahdr)) != SUCCESS) {
                   return false;
               }

               return true;
        }

        /**
         *
         */
        bool FSFlash::reserveArena(uint8_t arena) {
               uintptr_t arenaAddr = FS_BANK_BASE + (arena * FS_ARENA_SIZE);

               uint8_t sectorNumber;
               uint32_t sectorStart;
               uint32_t sectorSize;

               /* Ensure that the base address is in a valid sector */
               if (!getSectorInfo(arenaAddr,
                                    &sectorNumber,
                                    &sectorStart,
                                    &sectorSize)) {
                   /* We're asking for an invalid flash address */
                   return false;
               }

               ArenaHeader ahdr;
               if (readData(arenaAddr, (uint8_t*)&ahdr, sizeof(ahdr)) != SUCCESS) {
                   return false;
               }

               ahdr.state = ARENA_STATE_RESERVED;

               if (writeData(arenaAddr, (uint8_t*)&ahdr, sizeof(ahdr)) != SUCCESS) {
                   return false;
               }

               return true;
        }

        /**
         *
         */
        bool FSFlash::activeArena(uint8_t arena) {
               uintptr_t arenaAddr = FS_BANK_BASE + (arena * FS_ARENA_SIZE);

               uint8_t sectorNumber;
               uint32_t sectorStart;
               uint32_t sectorSize;

               /* Ensure that the base address is in a valid sector */
               if (!getSectorInfo(arenaAddr,
                                    &sectorNumber,
                                    &sectorStart,
                                    &sectorSize)) {
                   /* We're asking for an invalid flash address */
                   return false;
               }

               ArenaHeader ahdr;
               if (readData(arenaAddr, (uint8_t*)&ahdr, sizeof(ahdr)) != SUCCESS) {
                   return false;
               }

               if ( (ahdr.state != ARENA_STATE_ERASED) && (ahdr.state != ARENA_STATE_RESERVED)) {
                   return false;
               }

               ahdr.state = ARENA_STATE_ACTIVE;

               if (writeData(arenaAddr, (uint8_t*)&ahdr, sizeof(ahdr)) != SUCCESS) {
                   return false;
               }

               return true;
        }

        /**
         *
         */
        void FSFlash::umountArena()
        {
            state.activeSlots = 0;
            state.freeSlots = 0;
            state.mounted = false;
        }

        /**
         * Mount Arena
         */
        bool FSFlash::mountArena(uint8_t arena) {
            state.activeSlots = 0;
            state.freeSlots = 0;
            state.actArena = arena;

            for (uint16_t slotId = 1; slotId < FS_COUNT_SLOT; slotId++) {
                SlotHeader shdr;
                uintptr_t addr = getSlotAddr(slotId, arena);
                if (readData(addr, (uint8_t*)&shdr, sizeof(shdr)) != SUCCESS) {
                    return false;
                }

                if (shdr.state == SLOT_STATE_EMPTY) {
                    state.freeSlots++;
                } else if( shdr.state == SLOT_STATE_ACTIVE) {
                    state.activeSlots++;
                }
            }

            state.mounted = true;

            return true;
        }

        /**
         *
         */
        bool FSFlash::obsoleteArena(uint8_t arena) {
               uintptr_t arenaAddr = FS_BANK_BASE + (arena * FS_ARENA_SIZE);

               uint8_t sectorNumber;
               uint32_t sectorStart;
               uint32_t sectorSize;

               /* Ensure that the base address is in a valid sector */
               if (!getSectorInfo(arenaAddr,
                                    &sectorNumber,
                                    &sectorStart,
                                    &sectorSize)) {
                   /* We're asking for an invalid flash address */
                   return false;
               }

               ArenaHeader ahdr;
               if (readData(arenaAddr, (uint8_t*)&ahdr, sizeof(ahdr)) != SUCCESS) {
                   return false;
               }

               if (ahdr.state != ARENA_STATE_ACTIVE) {
                   return false;
               }

               ahdr.state = ARENA_STATE_OBSOLETE;

               if (writeData(arenaAddr, (uint8_t*)&ahdr, sizeof(ahdr)) != SUCCESS) {
                   return false;
               }

               return true;
        }

        int32_t FSFlash::findActiveArena() {
            for (uint8_t arenaId = 0; arenaId < (FS_BANK_SIZE / FS_ARENA_SIZE); arenaId++) {
                uintptr_t arenaAddr = FS_BANK_BASE + (arenaId * FS_ARENA_SIZE);

                ArenaHeader ahdr;
                if (readData(arenaAddr, (uint8_t*)&ahdr, sizeof(ahdr)) != SUCCESS) {
                    return IO_ERROR;
                }

                if (ahdr.state == ARENA_STATE_ACTIVE && ahdr.magic == FS_MAGIC) {
                    return arenaId;
                }
            }

            return ERROR;
        }

        bool FSFlash::copyBytes(uint32_t srcAddr, uint16_t size, uint32_t dstAddr)
        {
#define RAW_SIZE 16
            uint8_t datas[RAW_SIZE];

            while (size) {
                uint16_t blkSize;
                if (size >= RAW_SIZE) {
                    blkSize = RAW_SIZE;
                } else {
                    blkSize = size;
                }

                if (readData(srcAddr, datas, blkSize) != SUCCESS) {
                    return false;
                }

                if (writeData(dstAddr, datas, blkSize) != SUCCESS) {
                    return false;
                }

                size -= blkSize;
                srcAddr += blkSize;
                dstAddr += blkSize;
            }

            return true;
        }

        bool FSFlash::lockFlash()
        {

            if (xSemaphoreGive(mutex) != pdTRUE) {
               return false;
            }

            FLASH_Lock();

            return true;
        }

        bool FSFlash::unlockFlash()
        {
            if (xSemaphoreTake(mutex, portMAX_DELAY) != pdTRUE) {
                return false;
            }

            FLASH_Unlock();
            FLASH->SR = FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR;

            return true;
        }

    } /* namespace hal */
} /* namespace os */
