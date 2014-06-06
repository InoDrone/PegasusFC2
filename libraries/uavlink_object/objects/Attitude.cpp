#include "Attitude.h"

namespace obj {
    
    Attitude* Attitude::instance() {
        static Attitude* _instance = 0;
        if (_instance == 0) {
            _instance = new Attitude;
        }
        
        return _instance;
    }
    
    Attitude::Attitude() :
        UAVLinkObject("Attitude",ATTITUDE_ID, ATTITUDE_ISSETTINGS, sizeof(Datas)) {
        // TODO: using malloc
        memset(&datas, 0, sizeof(datas));
        setDefaultValues();
        // TODO: load data from (EEPROM or FLASH)
    }
    
    Attitude::Datas Attitude::get() const {
        return this->datas;
    }
    
    void Attitude::set(const Datas& data) {
        xSemaphoreTakeRecursive(mutex, portMAX_DELAY);
        this->datas = data;
        dispatchEvent(EVENT_UPDATED);
        xSemaphoreGiveRecursive(mutex);
    }
    
    void Attitude::setDefaultValues() {

        meta.flags = 
            0 << TELEMETRY_ACKED_SHIFT ||
            UPDATEMODE_PERIODIC << TELEMETRY_UPDATEON_SHIFT;

        meta.updatePeriod = 100;
    
    }
    
    void Attitude::toBytes(uint8_t *out) {
        memcpy(out, &datas, sizeof(Datas));
    }
    
    void Attitude::fromBytes(const uint8_t *in) {
        memcpy(&datas, in, sizeof(Datas));
    }
}
