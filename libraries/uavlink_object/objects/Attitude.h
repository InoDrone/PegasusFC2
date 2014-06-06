#ifndef ATTITUDE_H
#define ATTITUDE_H

#include "UAVLinkObject.h"
#include <cstring>

#define ATTITUDE_ID 0x6FAA51F6
#define ATTITUDE_ISSETTINGS false

namespace obj {
    class Attitude: public UAVLinkObject
    {
        public:
            struct Datas {
                   float q1;
                   float q2;
                   float q3;
                   float q4;
                   float Roll;
                   float Pitch;
                   float Yaw;

            };// __attribute__((packed)) Datas;
            
            // Functions
            
            static Attitude* instance();
            Datas get() const;
            void set(const Datas& data);
            
        protected:
            void toBytes(uint8_t *out);
            void fromBytes(const uint8_t *in);
            
        private:
            Datas datas;
            
            Attitude();
            void setDefaultValues();
    };
}

#endif
