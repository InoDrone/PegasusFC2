/*
 * UAVLink.cpp
 *
 *  Created on: 6 juin 2014
 *      Author: alienx
 */

#include "UAVLink.h"

#define STX1_VAL 0x55	// 'U'
#define STX2_VAL 0x4C	// 'L'

#define TYPE_MASK	0x0C
#define TYPE_VERSION	0x08
#define TYPE_OBJ	(TYPE_VERSION | 0x00)
#define TYPE_ACK	(TYPE_VERSION | 0x01)
#define TYPE_NACK	(TYPE_VERSION | 0x02)

#define HEADER_SIZE	3

using namespace obj;

UAVLink::Instance* UAVLink::init(UAVLinkListener* listener) {

  Instance* inst = new Instance;

  inst->listener = listener;
  inst->lock = xSemaphoreCreateRecursiveMutex();
  inst->transactionLock = xSemaphoreCreateRecursiveMutex();
  inst->ackSema = xSemaphoreCreateBinary();
  inst->rxBuffer = new uint8_t[512];
  inst->txBuffer = new uint8_t[512];


  return inst;
}

UAVLink::RxStats UAVLink::receive(Instance* inst, uint8_t byte) {
  UAVLink::RxStats stats = processInput(inst, byte);

  if (stats == STATE_COMPLETED) {
      // TODO: update OBJECT
  }

  return stats;
}
UAVLink::RxStats UAVLink::processInput(Instance* inst, uint8_t byte) {
  UAVLinkObject* object;
  Input* in = &inst->input;

  if (in->state == STATE_ERROR || in->state == STATE_COMPLETED) {
      in->state = STATE_STX1;
  }

  switch (in->state) {
    case STATE_STX1:
      if (byte != STX1_VAL) {
	  break;
      }
      in->state = STATE_STX2;
      break;
    case STATE_STX2:
      if (byte != STX2_VAL) {
	  in->state = STATE_STX1;
	  break;
      }

      in->crc = 0;
      in->rxCount = 0;
      in->state = STATE_TYPE;
      break;
    case STATE_TYPE:
      if ((byte & TYPE_MASK) != TYPE_VERSION) {
	  in->state = STATE_STX1;
	  break;
      }

      in->crc ^= byte;
      in->type = byte;
      in->psize = 0;
      in->state = STATE_SIZE;
      break;
    case STATE_SIZE:
      in->crc ^= byte;

      if (in->rxCount == 0) {
	  in->psize = byte;
	  break;
      }

      in->psize += byte << 8;
      in->rxCount = 0;
      in->id = 0;
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

      if (in->psize - HEADER_SIZE != in->length ) {
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
