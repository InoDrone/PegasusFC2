#ifndef ACCELSENSOR_H
#define ACCELSENSOR_H

#include "UAVLinkObject.h"
#include <cstring>

#define ACCELSENSOR_ID 0x9AB7D469
#define ACCELSENSOR_ISSETTINGS false

namespace obj {
    class AccelSensor: public UAVLinkObject
    {
        public:
            // Field Type Definitions


            // Field Datas           
            struct Datas {
                   float x;
                   float y;
                   float z;
                   float temperature;

            } __attribute__((packed));
            
            // Functions
            
            static AccelSensor* instance();
            Datas get() const;
            void set(const Datas& data);
            
        protected:
            void toBytes(uint8_t *out);
            void fromBytes(const uint8_t *in);
            
        private:
            /*alignas(4)*/ Datas datas;
            
            AccelSensor();
            void setDefaultValues();
    };
}

#endif
