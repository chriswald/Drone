#ifndef __ROUTE_H__
#define __ROUTE_H__

#define DONT_CARE_LAT -1
#define DONT_CARE_LON -1
#define DONT_CARE_ALT -1

struct Location {
    float lat;
    float lon;
    float alt;
};

struct Waypoint;
struct Waypoint {
    Waypoint(Location l, Waypoint* next);
    Location location;
    Waypoint* next;
};

class Route
{
public:
    Route();
    ~Route();

    void navigate();

    void setHomeLocation(Location loc);
    bool addLocation(Location l);
    bool useNextLocation();
    Location getDestination();
    void clearRoute();
    int numWaypoints();
    Location getLocation(int index);
    bool removeLocation(int index);
    bool insertLocation(Location l, int index);

    bool atDestination(Location curPos);
    double distanceToDestinationMeters(Location curPos);
    double totalDistanceMeters();
private:
    Waypoint* itemAt(int index);

    Location home;
    Waypoint* front;
    Waypoint* end;
    int count;
};

extern Route ROUTE;

#endif // __ROUTE_H__
