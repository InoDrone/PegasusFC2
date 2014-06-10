/*
 * UAVLink.h
 *
 *  Created on: 4 juin 2014
 *      Author: alienx
 */

#ifndef UAVLINK_H_
#define UAVLINK_H_


#include "UAVLinkObject.h"
#include "UAVLinkListener.h"
#include "UAVLinkPeriodic.h"
#include "UAVLinkObjectsInclude.h"

//typedef int32_t (*UAVLinkOutputStream)(uint8_t* datas, int32_t length);

class UAVLink {
public:

  enum RxStats {
    STATE_ERROR = 0,
    STATE_STX1,
    STATE_STX2,
    STATE_TYPE,
    STATE_SIZE,
    STATE_OBJID,
    STATE_DATAS,
    STATE_CRC,
    STATE_COMPLETED
  };

  struct Input {
    uint8_t  type;
    uint16_t psize;
    uint16_t length;
    uint32_t id;
    uint8_t crc;
    RxStats state;
    uint32_t rxCount;
    uint32_t rxPacketLength;
  };

  struct ResponseType {
          uint8_t type;
          uint32_t id;
  };

  struct Instance {
    uint8_t  		*rxBuffer;
    uint8_t  		*txBuffer;
    UAVLinkListener*    listener;
    xSemaphoreHandle  	lock;
    xSemaphoreHandle    transactionLock;
    xSemaphoreHandle    ackSema;
    Input 		input;
    ResponseType    response;
  };

  static Instance* init(UAVLinkListener*);
  static RxStats receive(Instance* inst, uint8_t byte);
  static bool sendObject(Instance* inst, obj::UAVLinkObject* obj, bool acked, uint32_t timeout);

  static void updateAck(Instance* inst, uint8_t type, uint32_t id);

private:
  static RxStats processInput(Instance* inst, uint8_t byte);
  static bool startObjectTransaction(Instance* inst, obj::UAVLinkObject* obj, uint8_t type, uint32_t timeout);
  static bool send(Instance* inst, uint8_t type, obj::UAVLinkObject* obj);
  static bool receiveObject(Instance* inst, uint8_t type, uint32_t id, uint8_t *data);

  static uint8_t generateCRC(const uint8_t*, uint16_t length);
};

#endif /* UAVLINK_H_ */
