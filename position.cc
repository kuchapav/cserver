
#include "position.h"

template<class Archive>
    void position::serialize(Archive & ar, const unsigned int version) {
        ar & TOid;
        ar & time;
        ar & x;
        ar & y;
        ar & z;
        ar & pitch;
        ar & roll;
        ar & yaw;
        ar & trackedObj.valid;
        ar & trackedObj.x;
        ar & trackedObj.y;
        ar & trackedObj.y;
        ar & trackedObj.pitch;
        ar & trackedObj.roll;
        ar & trackedObj.yaw;
        ar & trackedObj.pixel_ratio;
        ar & trackedObj.bw_ratio;
    }

void position::printData() {
    std::cout << "id: " << TOid << std::endl
              << "time: " << time << std::endl
              << "X: " << x << std::endl
              << "Y: " << y << std::endl
              << "Z: " << z << std::endl
              << "pitch: " << pitch << std::endl
              << "roll: " << roll << std::endl
              << "yaw: " << yaw << std::endl
              << "Tracked Object valid: " << trackedObj.valid << std::endl
              << "Tracked Object X: " << trackedObj.x << std::endl
              << "Tracked Object Y: " << trackedObj.y << std::endl
              << "Tracked Object Z: " << trackedObj.y << std::endl
              << "Tracked Object pitch: " << trackedObj.pitch << std::endl
              << "Tracked Object roll: " << trackedObj.roll << std::endl
              << "Tracked Object yaw: " << trackedObj.yaw << std::endl
              << "Tracked Object pixel_ratio: " << trackedObj.pixel_ratio << std::endl
              << "Tracked Object bw_ratio: " << trackedObj.bw_ratio << std::endl;
  }