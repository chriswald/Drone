#ifndef __MESSAGE_QUEUE_H__
#define __MESSAGE_QUEUE_H__

#include <Arduino.h>
#include <Radio/DroneRadio.h>

typedef Packet QITEMTYPE;

bool qpush(QITEMTYPE item);
bool qpop (QITEMTYPE& item);
bool qpeek(QITEMTYPE& item);
bool qisFull();
bool qisEmpty();

#endif // __MESSAGE_QUEUE_H__