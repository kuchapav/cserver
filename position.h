#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <string>
#include "tracked_object.h"

/// Structure to hold information about a single stock.
struct position
{

  imr::STrackedObject trackedObj;
  unsigned long time, TOid;
  float x, y, z, pitch, roll, yaw;

template<class Archive>
    void serialize(Archive & ar, const unsigned int version);

  void printData();
};

#endif // POSITION_H