#ifndef UAVLINKLISTENR_H_
#define UAVLINKLISTENR_H_

namespace obj {

	class UAVLinkListener {
	public:
		virtual void UAVLinkHandle(UAVLinkEvent*);
	};
}

#endif
