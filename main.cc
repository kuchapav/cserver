#include <boost/asio.hpp>
#include "cserver.h"
#include "tracked_object.h"
#include "position.h"

int main(int argc, char* argv[])
{
  try
  {
    position init_pos;
    imr::STrackedObject trackedObj;
    trackedObj.valid = false;
    trackedObj.x = 0;
    trackedObj.y = 0;
    trackedObj.z = 0;
    trackedObj.pitch = 0;
    trackedObj.roll = 0;
    trackedObj.yaw = 0;
    trackedObj.pixel_ratio = 0;
    trackedObj.bw_ratio = 0;

    init_pos.trackedObj = trackedObj;
    init_pos.x = argc > 1 ? atoi(argv[1]) : 0;
    init_pos.y = argc > 2 ? atoi(argv[2]) : 0;
    init_pos.z = argc > 3 ? atoi(argv[3]) : 0;
    init_pos.pitch = argc > 4 ? atoi(argv[4]) : 0;
    init_pos.roll = argc > 5 ? atoi(argv[5]) : 0;
    init_pos.yaw = argc > 6 ? atoi(argv[6]) : 0;

    boost::asio::io_service io_service;
    cserver server(io_service, init_pos);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
