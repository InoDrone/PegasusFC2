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
  };

  struct Instance {
    uint8_t  		*rxBuffer;
    uint8_t  		*txBuffer;
    UAVLinkListener*    listener;
    xSemaphoreHandle  	lock;
    xSemaphoreHandle    transactionLock;
    xSemaphoreHandle    ackSema;
    Input 		input;
  };

  static Instance* init(UAVLinkListener*);
  static RxStats receive(Instance* inst, uint8_t byte);

private:
  static RxStats processInput(Instance* inst, uint8_t byte);
};

#endif /* UAVLINK_H_ */
