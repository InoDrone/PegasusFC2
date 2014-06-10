/*
 * UAVLink.cpp
 *
 *  Created on: 6 juin 2014
 *      Author: alienx
 */

#include "UAVLink.h"
#include "Trace.h"

#define STX1_VAL 0x55	// 'U'
#define STX2_VAL 0x4C	// 'L'

#define TYPE_MASK	    0x18
#define TYPE_VERSION	0x08
#define TYPE_OBJ	    (TYPE_VERSION | 0x00)
#define TYPE_OBJ_ACK    (TYPE_VERSION | 0x01)
#define TYPE_ACK	    (TYPE_VERSION | 0x02)
#define TYPE_NACK	    (TYPE_VERSION | 0x03)
#define TYPE_META	    (TYPE_VERSION | 0x04)

#define HEADER_SIZE	9

using namespace obj;

UAVLink::Instance* UAVLink::init(UAVLinkListener* listener) {

  Instance* inst = new Instance;

  inst->listener = listener;
  inst->lock = xSemaphoreCreateRecursiveMutex();
  inst->transactionLock = xSemaphoreCreateRecursiveMutex();
  inst->ackSema = xSemaphoreCreateBinary();
  inst->rxBuffer = new uint8_t[512];
  inst->txBuffer = new uint8_t[512];
  inst->input.rxPacketLength = 0;


  return inst;
}

UAVLink::RxStats UAVLink::receive(Instance* inst, uint8_t byte) {
  UAVLink::RxStats stats = processInput(inst, byte);

  if (stats == STATE_COMPLETED) {
      receiveObject(inst, inst->input.type, inst->input.id, inst->rxBuffer);
  }

  return stats;
}


UAVLink::RxStats UAVLink::processInput(Instance* inst, uint8_t byte) {
  UAVLinkObject* object;
  Input* in = &inst->input;

  if (in->state == STATE_ERROR || in->state == STATE_COMPLETED) {
      in->state = STATE_STX1;
  }

  in->rxPacketLength++;

  switch (in->state) {
    case STATE_STX1:
      if (byte != STX1_VAL) {
              break;
      }

      in->state = STATE_STX2;
      in->crc = 0;
      in->crc ^= byte;
      in->rxPacketLength = 1;
      break;
    case STATE_STX2:
      if (byte != STX2_VAL) {
          in->state = STATE_STX1;
          break;
      }

      in->crc ^= byte;
      in->rxCount = 0;
      in->state = STATE_SIZE;
      break;
    case STATE_SIZE:
      in->crc ^= byte;

      if (in->rxCount == 0) {
          in->psize = byte;
          in->rxCount++;
          break;
      }

      in->psize += byte << 8;
      in->rxCount = 0;

      if (in->psize < HEADER_SIZE) {
          in->state = STATE_ERROR;
          break;
      }

      in->id = 0;
      in->state = STATE_TYPE;
      break;
    case STATE_TYPE:

      if ((byte & TYPE_MASK) != TYPE_VERSION) {
          in->state = STATE_STX1;
          break;
      }

      in->crc ^= byte;
      in->type = byte;
      in->state = STATE_OBJID;
      break;
    case STATE_OBJID:
      in->crc ^= byte;

      in->id += byte << (8 * (in->rxCount++));
      if (in->rxCount < 4) {
          break;
      }
      in->rxCount = 0;


      object = UAVLinkObject::getById(in->id);
      if (object == NULL) {
          in->state = STATE_ERROR;
          break;
      }

      if (in->type == TYPE_ACK || in->type == TYPE_NACK) {
        in->length = 0;
      } else {
        in->length = object->length();
      }

      if ((in->rxPacketLength + in->length) != in->psize ) {
          in->state = STATE_ERROR;
          break;
      }


      if (in->length == 0) {
          in->state = STATE_CRC;
      } else {
          in->state = STATE_DATAS;
      }
    break;
    case STATE_DATAS:
      in->crc ^= byte;
      inst->rxBuffer[in->rxCount++] = byte;
      if (in->rxCount < in->length) {
          break;
      }
      in->rxCount = 0;
      in->state = STATE_CRC;
      break;
    case STATE_CRC:
      if (in->crc != byte) {
         in->state = STATE_ERROR;
         break;
      }

      in->state = STATE_COMPLETED;

      break;
    default:
      in->state = STATE_ERROR;
      break;
  }

  return in->state;
}

bool UAVLink::receiveObject(Instance* inst, uint8_t type, uint32_t id, uint8_t *data)
{
    UAVLinkObject* object;
    bool error = false;

    xSemaphoreTakeRecursive(inst->lock, portMAX_DELAY);
    object = UAVLinkObject::getById(id);

    switch(type) {
    case TYPE_OBJ:
        if (object) {
            object->unpack(data);
            updateAck(inst, type, id);
        } else {
            error = true;
        }
        break;
    case TYPE_OBJ_ACK:
        if (object) {
            object->unpack(data);
            send(inst, TYPE_ACK, object);
        } else {
            error = true;
        }
        break;
    case TYPE_ACK:
        if (object) {
            updateAck(inst, type, id);
        } else {
            error = true;
        }
        break;
    }

    xSemaphoreGiveRecursive(inst->lock);

    return error;
}

bool UAVLink::sendObject(Instance* inst, obj::UAVLinkObject* obj, bool acked, uint32_t timeout)
{
  if (acked) {
      return startObjectTransaction(inst, obj, TYPE_OBJ_ACK, timeout);
  } else {
      return startObjectTransaction(inst, obj, TYPE_OBJ, timeout);
  }
}

bool UAVLink::startObjectTransaction(Instance* inst, obj::UAVLinkObject* obj, uint8_t type, uint32_t timeout)
{
   bool success = false;
   int32_t ackReceived;
   if (type == TYPE_OBJ_ACK) {
       xSemaphoreTakeRecursive(inst->transactionLock, portMAX_DELAY);
       xSemaphoreTakeRecursive(inst->lock, portMAX_DELAY);

       success = send(inst, type, obj);
       xSemaphoreGiveRecursive(inst->lock);
       inst->response.id = obj->getId();
       inst->response.type = TYPE_ACK; // ORD TYPE_OBJ
       if (success) {
           // Wait for receive ACK
           ackReceived = xSemaphoreTake(inst->ackSema, timeout / portTICK_RATE_MS);
       }

       if (ackReceived == pdTRUE) {
           xSemaphoreGiveRecursive(inst->transactionLock);
           success = true;
       } else {
           xSemaphoreGiveRecursive(inst->transactionLock);
           success = false;
       }
   }else if (type == TYPE_OBJ) {
         xSemaphoreTakeRecursive(inst->lock, portMAX_DELAY);
         success = send(inst, type, obj);
         xSemaphoreGiveRecursive(inst->lock);
   }

   return success;
}

bool UAVLink::send(Instance* inst, uint8_t type, obj::UAVLinkObject* obj)
{
  if (inst == 0) {
      return false;
  }

  uint16_t headerLength;
  inst->txBuffer[0] = STX1_VAL;
  inst->txBuffer[1] = STX2_VAL;
  //inst->txBuffer[2] = size1
  //inst->txBuffer[3] = size2

  inst->txBuffer[4] = type;

  uint32_t id = obj->getId();
  inst->txBuffer[5] = (uint8_t)(id & 0xFF);
  inst->txBuffer[6] = (uint8_t)((id >> 8) & 0xFF);
  inst->txBuffer[7] = (uint8_t)((id >> 16) & 0xFF);
  inst->txBuffer[8] = (uint8_t)((id >> 24) & 0xFF);
  headerLength = 9;

  uint16_t length;
  if (type == TYPE_ACK || type == TYPE_NACK) {
      length = 0;
  } else {
      length = obj->length();
      obj->pack(&inst->txBuffer[headerLength]);
  }

  inst->txBuffer[2] = (uint8_t)((headerLength + length) & 0xFF);
  inst->txBuffer[3] = (uint8_t)(((headerLength + length) >> 8) & 0xFF);

  inst->txBuffer[headerLength + length] = generateCRC(inst->txBuffer, headerLength + length );
  uint16_t msgLength = headerLength + length + 1;
  int32_t size = inst->listener->transmitData(inst->txBuffer, msgLength);

  if (size != msgLength) {
      return false;
  }

  return true;
}

void UAVLink::updateAck(Instance* inst, uint8_t type, uint32_t id)
{
    if (inst->response.id == id && inst->response.type == type) {
            // Release ACK Semaphore lock
            xSemaphoreGive(inst->ackSema);
            inst->response = {0, 0};
    }
}

uint8_t UAVLink::generateCRC(const uint8_t* datas, uint16_t length) {
  uint8_t crc = 0;
  const uint8_t* ptr = datas;

  while (length--) {
      crc ^= *ptr++;
  }

  return crc;
}
