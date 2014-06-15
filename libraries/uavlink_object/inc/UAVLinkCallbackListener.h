/*
 * UAVLinkCallbackListener.h
 *
 *  Created on: 11 juin 2014
 *      Author: alienx
 */

#ifndef UAVLINKCALLBACKLISTENER_H_
#define UAVLINKCALLBACKLISTENER_H_

class UAVLinkCallbackListener {
    public:
        virtual void uavlinkHandle(void *parameters) = 0;
};


#endif /* UAVLINKCALLBACKLISTENER_H_ */
