UAV Tracker Readme
------------------

***Usage***

**Overview**

This software uses video and GPS information to instruct the tracker hardware. By default,
this program will analyze video from the HDMI input to a connected Decklink capture card. GPS
information will be received using a USB-serial connection to any GPS transmitter that
implements the NMEA protocol. Tracker commands are sent via a USB-serial connection to the 
Arduino microcontroller connected to the tracker hardware.

In addition to live tracking, the software has some additional features. For example, the 
tracker can record the video input, saving the result as a sequence of JPEGs that can later
be encoded into a movie if desired. It is also possible to analyze a video or still image for
debugging purposes. It is also possible to draw a line from the center of the image to the 
plane, to ensure that CV is working properly.

To run this software, first ensure that the serial connections to the GPS receiver and the 
Arduino are established, and that the Decklink capture card is plugged into the computer. Then
run the `tracker` executable from the command line (e.g `./tracker`). A video window should 
appear on the screen mirroring the camera's output. For more information on how to set 
the configuration for the tracker, read the section on _Options_ below, or run 
`./tracker --help`


**Options**

--debug: Enables debug output. This outputs LOTS of information to the terminal window,
         including blobs that were detected, GPS information coming in, and commands that
         are being sent to the Arduino. It is advisable to pipe the output to grep if you want
         specific information.

--extras: Displays the image at each step of visual processing. Note that enabling this option
          could degrade performance. Useful for debugging CV.

--line: Draws a red line from the center of the frame to the plane when the plane is
        detected.

--record: Records the video output to the specified directory. Each frame in the video gets 
          saved as a JPEG image. Because performance requirements may mandate frame-skipping,
          be aware that skipped frames will not be recorded.

--lat: Specifies that latitude of the tracker in GPS degrees.

--lon: Specifies the longitude of the tracker in GPS degrees.

--alt: Specifies the altitude of the tracker in meters.

--gps-port: Specifies the serial port that the GPS receiver is connected to

--arduino-port: Specifies the serial port that the Arduino is connected to

--gps-baud: Specifies the baud rate for the serial connection to the GPS receiver

--arduino-baud: Specifies the baud rate for the serial connection to the arduino

--video: Uses the specified video file for computer vision instead of the Decklink interface.
         Useful for testing purposes. 

--image: Uses the specified image file for computer vision instead of the Decklink interface.
         Useful for testing purposes.

--blind: Do not use computer vision. Use this mode if there is no camera attached or you do not
         have the Decklink drivers installed.

--no-gps: Do not use GPS.

--help: Display a help message.

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
