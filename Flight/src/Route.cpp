#include "Route.h"
#include "TinyGPSPlus/TinyGPS++.h"

#ifndef NULL
#define NULL 0
#endif

Route ROUTE;

Waypoint::Waypoint(Location l, Waypoint* n)
{
    location = l;
    next = n;
}

Route::Route()
{
    front = NULL;
    end   = NULL;
    count = 0;
}

Route::~Route()
{
    clearRoute();
}

void Route::navigate()
{
    // Determine desired orientation based
    // on difference between current location and
    // desired location.

    // FlightController.setOrientation()
    //   or
    // FlightController.setAccelerometer()

    // FlightController.loop()
}

void Route::setHomeLocation(Location loc)
{
    home = loc;
}

bool Route::addLocation(Location l)
{
    if (count == 0)
    {
        front = end = new Waypoint(l, NULL);
    }
    else
    {
        end->next = new Waypoint(l, NULL);
        end = end->next;
    }
    count ++;
    return true;
}

bool Route::useNextLocation()
{
    if (front != NULL)
    {
        Waypoint* tmp = front;
        front = front->next;
        delete tmp;
        count --;
        return true;
    }
    return false;
}

Location Route::getDestination()
{
    if (front != NULL)
        return front->location;
    else
        return (Location) {0,0,0};
}

bool Route::atDestination(Location curPos)
{
    if (front != NULL)
        return distanceToDestinationMeters(curPos) < 4.0;
    else
        return false;
}

void Route::clearRoute()
{
    Waypoint* tmp;
    while (front != NULL)
    {
        tmp = front;
        front = front->next;
        delete tmp;
    }
    end = NULL;
    count = 0;
}

int Route::numWaypoints()
{
    return count;
}

Location Route::getLocation(int index)
{
    Waypoint* tmp = itemAt(index);
    if (tmp == NULL) return (Location) {0, 0, 0};
    else return tmp->location;
}

bool Route::removeLocation(int index)
{
    Waypoint* tmp1 = itemAt(index);
    Waypoint* tmp2 = itemAt(index-1);

    if (tmp1 == NULL)
        return false;

    if (tmp2 == NULL)
    {
        front = tmp1->next;
        delete tmp1;
        count --;
        return true;
    }

    if (tmp1->next == NULL)
    {
        end = tmp2;
        delete tmp1;
        count --;
        return true;
    }

    tmp2->next = tmp1->next;
    delete tmp1;
    count --;
    return true;
}

bool Route::insertLocation(Location l, int index)
{
    if (index < count+1)
    {
        Waypoint* tmp1 = itemAt(index);
        Waypoint* tmp2 = itemAt(index-1);
        Waypoint* insert = new Waypoint(l, tmp1);
        if (tmp2 != NULL)
            tmp2->next = insert;
        if (index == 0)
            front = insert;
        if (index == count)
            end = insert;
        count ++;
        return true;
    }
    return false;
}

Waypoint* Route::itemAt(int index)
{
    if (index < count)
    {
        int i = 1;
        Waypoint* tmp = front;
        while (i < index)
        {
            tmp = tmp->next;
            i ++;
        }
        return tmp;
    }
    return NULL;
}

double Route::distanceToDestinationMeters(Location curPos)
{
    if (front != NULL)
    {
        double lat1, lon1, lat2, lon2;
        lat1 = curPos.lat;
        lon1 = curPos.lon;
        lat2 = front->location.lat;
        lon2 = front->location.lon;
        return TinyGPSPlus::distanceBetween(lat1, lon1, lat2, lon2);
    }
    return 0;
}

double Route::totalDistanceMeters()
{
    if (front != NULL)
    {
        double lat1, lat2, lon1, lon2;
        lat1 = home.lat;
        lon1 = home.lon;
        lat2 = front->location.lat;
        lon2 = front->location.lon;
        double distance = TinyGPSPlus::distanceBetween(lat1, lon1, lat2, lon2);
        Waypoint* tmp = front->next;
        while (tmp != NULL)
        {
            lat1 = lat2;
            lon1 = lon2;
            lat2 = tmp->location.lat;
            lon2 = tmp->location.lon;
            distance += TinyGPSPlus::distanceBetween(lat1, lon1, lat2, lon2);
            tmp = tmp->next;
        }
        return distance;
    }
    return 0;
}

