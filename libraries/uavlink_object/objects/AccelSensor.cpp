#include "AccelSensor.h"

namespace obj {
    
    AccelSensor* AccelSensor::instance() {
        static AccelSensor* _instance = 0;
        if (_instance == 0) {
            _instance = new AccelSensor;
        }
        
        return _instance;
    }
    
    AccelSensor::AccelSensor() :
        UAVLinkObject("AccelSensor",ACCELSENSOR_ID, ACCELSENSOR_ISSETTINGS, sizeof(Datas)) {
        // TODO: using malloc
        memset(&datas, 0, sizeof(datas));
        setDefaultValues();
        
        /* Load from Flash */
        if (ACCELSENSOR_ISSETTINGS) {
	        os::hal::FSFlash::read(ACCELSENSOR_ID, (uint8_t *)&datas, sizeof(Datas));
	    }
    }
    
    AccelSensor::Datas AccelSensor::get() const {
        return this->datas;
    }
    
    void AccelSensor::set(const Datas& data) {
        xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
        this->datas = data;
        dispatchEvent(EVENT_UPDATED);
        xSemaphoreGiveRecursive(mutex);
    }
    
    void AccelSensor::setDefaultValues() {

        meta.flags = 
            0 << TELEMETRY_ACKED_SHIFT |
            UPDATEMODE_PERIODIC << TELEMETRY_UPDATEON_SHIFT;

        meta.updatePeriod = 10000;
        
        // Defaults Values
    
    }
    
    void AccelSensor::toBytes(uint8_t *out) {
        memcpy(out, &datas, sizeof(Datas));
    }
    
    void AccelSensor::fromBytes(const uint8_t *in) {
        memcpy(&datas, in, sizeof(Datas));
        /* Save to from Flash */
        if (ACCELSENSOR_ISSETTINGS) {
	        os::hal::FSFlash::save(ACCELSENSOR_ID, (uint8_t *)&datas, sizeof(Datas));
	    }
    }
}
