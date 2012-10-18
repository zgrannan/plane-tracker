#ifndef VIDEORECIEVERINTERFACE_H
#define VIDEORECIEVERINTERFACE_H

class IplImage;

class VideoRecieverInterface : ListeningInterface {
	void sendImage(IplImage* image);
};

#endif
