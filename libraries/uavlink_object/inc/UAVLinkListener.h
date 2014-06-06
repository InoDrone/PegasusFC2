#ifndef UAVLINKLISTENR_H_
#define UAVLINKLISTENR_H_

class UAVLinkListener {
public:
	virtual int32_t transmitData(uint8_t *data, int32_t length) = 0;
};

#endif
