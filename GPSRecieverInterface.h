#ifndef GPSRECIEVER_INTERFACE_H 
#define GPSRECIEVER_INTERFACE_H 

class GPSRecieverInterface: ListeningInterface {
private: 
	void sendGPS(double lat, double lon, double alt);
};

#endif
