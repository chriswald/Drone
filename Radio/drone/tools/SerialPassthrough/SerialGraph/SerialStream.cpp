#include "SerialStream.h"

bool SerialStream::open(unsigned comm)
{
    return isopen = psio.openComm(handle, comm);
}

bool SerialStream::close()
{
    return !(isopen = !psio.close(handle));
}

SerialStream& SerialStream::operator<<(string& output)
{
    psio.write(handle, output);
    return *this;
}

SerialStream& SerialStream::operator>>(string& input)
{
    input = psio.read(handle);
    return *this;
}

bool SerialStream::isOpen()
{
    return isopen;
}