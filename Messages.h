#ifndef MESSAGES_H
#define MESSAGES_H

class ImageMessage{
	public:
		IplImage* image;
		ImageMessage(IplImage* image): image(image){}
};

class GPSDataMessage{
	public:
		double lat, lon, alt;
		GPSDataMessage(double lat, double lon, double alt): lat(lat),lon(lon),alt(alt){}
};

class GPSLostMessage;
class VideoLostMessage;

class PositionMessage{
	public:
		double pan, tilt;
		PositionMessage(double pan, double tilt): pan(pan), tilt(tilt){}
};

class RelativePositionMessage: PositionMessage;
class AbsolutePositionMessage: PositionMessage;


#endif
