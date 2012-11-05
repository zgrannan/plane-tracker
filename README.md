UAV Tracker Readme
------------------


***Usage***

***Architecture***

**Vision Components**

*VideoReceiverInterface.cpp*

This component is in charge of getting image data for `FrameAnalyzerActor` to process. It can 
access data from the BlackMagic capture card, or from a video file. 


*FrameAnalyzerActor.cpp*

This actor receives images from the `VideoReceiverInterface` and uses `Vision` to calculate
the location of the plane in the image (if possible). If the plane was detected in the image,
it calculates the necessary pan and tilt values for the gimbal to point to the plane, and sends
that information to the `MultimodalActor`. If the plane could not be detected in the image, 
this informs `MultimodalActor` that the tracker has lost visual contact with the plane. In
addition, this actor is also responsible for sending the frame back to the UI so that it can be
displayed to the user.


*Vision.cpp*

This component is responsible for performing the actual CV operations used to detect the 
plane. It is capable of finding the centroid and size of the plane in an image.


**Georeferencing Components**

*GPSReceiverInterface.cpp*

This component is responsible for downloading GPS data from the plane. Whenever it receives a GPS
packet, this interface uses `Protocol` to parse the packet and sends the plane's GPS 
coordinates to the `GeoReferencingActor`.


*GeoReferencingActor.cpp*

The `GeoReferencingActor` is responsible for receiving GPS data from the `GPSReceiverInterface`
and using `GeoReference` to calculate the necessary pan and tilt to point the tracker towards 
the UAV. The resulting pan and tilt values are sent to the `MultimodalActor` to perform the 
actual pointing. If the GPS position of the plane is unavailable, this actor will inform the 
`MultimodalActor` that the tracker has lost the GPS position.


*GeoReference.cpp*

`GeoReference` is responsible for performing the calculations for determining the pan
and tilt of the gimbal based on the plane's GPS position. It uses the Haversine formula to
calculate the pan.

*Protocol.cpp*

This component implements the NMEA protocol that the GPS uses to send GPS data to the computer.


**Communications Components**

*Messages.cpp*

This component defines containers for the messages that are sent between the various
components in the system. These include the internal messages that are sent between the 
interfaces and the actors, as well as messages that are serialzed to be sent over the serial 
interface to the arduino.


**Operations Components**

*MultimodalActor.cpp*

The MultimodalActor is responsible for receiving the position messages from the 
`GeoReferencingActor` and the `FrameAnalyzerActor` and sending the appropriate commands to 
the arduino. As long as the plane can be visually identified, the multimodal actor will not
forward position commands sent from the `GeoReferencingActor`. If visual identification fails,
the `MultimodalActor` will use `GeoReferencingActor`'s position messages. If both visual and 
GPS identification is impossible, then the `MultiModalActor` will tell the arduino to use
RSSI. 

*tracker.cpp*

This is the main driver for the program. It instantiates all of the actors and components. 
Additionally, it allows the user to view the airplane in a window, and record the flight.
