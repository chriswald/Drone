#include "MessageQueue.h"

#define MAX_QUEUE 10
static QITEMTYPE queue[MAX_QUEUE];
static int front = 0;
static int rear = 0;

bool qpush(QITEMTYPE item)
{
    if ((rear + 1) % MAX_QUEUE != front)
    {
        queue[rear] = item;
        rear = (rear + 1) % MAX_QUEUE;
        return true;
    }
    return false;
}

bool qpop(QITEMTYPE& item)
{
    bool retVal = false;
    if (front != rear)
    {
        item  = queue[front];
        front = (front + 1) % MAX_QUEUE;
        retVal = true;
    }
    return retVal;
}

bool qpeek(QITEMTYPE& item)
{
    if (front != rear)
    {
        item = queue[front];
        return true;
    }
    return false;
}

bool qisFull()
{
    return (rear + 1) % MAX_QUEUE == front;
}

bool qisEmpty()
{
    bool retVal;
    retVal = front == rear;
    return retVal;
}