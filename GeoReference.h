#ifndef GEOREFERENCE_H
#define GEOREFERENCE_H
#include <cv.h>
#include <vector>
using namespace std;
// Source code taken from http://www.gpsy.com/gpsinfo/geotoutm/
class Ellipsoid
{
  public:
    Ellipsoid(){};
    Ellipsoid(int Id, char* name, double radius, double ecc)
    {
      id = Id; ellipsoidName = name; 
      EquatorialRadius = radius; eccentricitySquared = ecc;
    }

    int id;
    char* ellipsoidName;
    double EquatorialRadius; 
    double eccentricitySquared;  
};

class GeoReference {

  public:
    static void runTests();
    //:static vector<string> latLonToDMS(double lat, double lon);

    static double GPStoMeters(double gps);
    static double metersToGPS(double meters);

    static string getHeadingString( double angle );
    static double distanceBetweenGPS(double lat1, double lon1, double lat2, double lon2);

    static cv::Mat EulerAngles(bool transpose, cv::Mat Orig_Vector, double Roll, double Pitch, double Yaw);
    static bool getGPS(double plane_latitude, double plane_longitude, double plane_altitude, double plane_roll, double plane_pitch, double plane_heading, double gimbal_roll, double gimbal_pitch, double gimbal_yaw, 
        double target_x, double target_y, double zoom, double & Target_Latitude, double & Target_Longitude, double & Target_Height);
    //static void getTargetGPS(Database::CandidateRowData * data, double & centerLatitude, double & centerLongitude, double & centerAltitude );
    //static void getTargetGPS(Database::UnverifiedRowData * data, double & centerLatitude, double & centerLongitude, double & centerAltitude );
    //static void getTargetOrientation(Database::UnverifiedRowData * data, double & orientation);
    static void reverseGeoreference(double plane_latitude, double plane_longitude, double plane_altitude, double plane_roll, double plane_pitch, double plane_heading, 
        double Target_Latitude, double Target_Longitude, double Target_Height, double & gimbal_roll, double & gimbal_pitch);
    static bool forwardGeoreferencing(double plane_latitude, double plane_longitude, double plane_altitude, double plane_roll, double plane_pitch, double plane_heading, double gimbal_roll, double gimbal_pitch, double gimbal_yaw, 
        double target_x, double target_y, double zoom, double & Target_Latitude, double & Target_Longitude, double & Target_Height);

  private:
    static string matToString(cv::Mat in);
    static cv::Mat Quaternion(double theta, double X, double Y, double Z);
    static cv::Mat Quaternion_Transform(cv::Mat Orig_Vector, cv::Mat Quat);
    static cv::Mat ECEF_to_NED(cv::Mat ECEF, double Latitude, double Longitude);
    static cv::Mat NED_to_ECEF(cv::Mat NED, double Latitude, double Longitude);
    static cv::Mat ECEF_to_GEO(cv::Mat ECEF, double flatness, double eccentricity, double semi_major_axis);
    static cv::Mat EulerAngles_Plane(cv::Mat Orig_Vector, double Roll, double Pitch, double Yaw);

};

// Using NTSC camera - pixel aspect ratio is 11:10
// Using Sony FCB EX 11D, focal length linear from 4.2mm (zoom level 1)
// to 42.0mm (zoom level 10)*/

#endif
