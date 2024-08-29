#include "bytebuffer.h"
#include "string.h"
#include <sys/socket.h>
#include <unistd.h>

ByteBuffer::ByteBuffer(int sz)
{
    data.resize(sz);
    oft = 0;
}

int ByteBuffer::length()
{
    return (int)data.size();
}

char* ByteBuffer::bytes()
{
    return (length() == 0) ? NULL : &data.at(0);
}

void ByteBuffer::erase(int size)
{
    if(size <= 0)
        return;
    if(size >= length())
    {
        data.clear();
        return;
    }
    data.erase(data.begin(), data.begin() + size);
    
}

void ByteBuffer::append(const char* bytes, int size)
{
    data.insert(data.end(), bytes, bytes+size);
}

char* ByteBuffer::curat()
{
    return (length() == 0)? NULL : &data.at(oft);
}

int ByteBuffer::getoft()
{
    return oft;
}

bool ByteBuffer::requeire(int size)
{
    return size <= length() - oft;
}

bool ByteBuffer::skip_x(int size)
{
    if(requeire(size))
    {
        oft += size;
        return true;
    }
    return false;
}

bool ByteBuffer::read_bytes_x(char *cb, int size)
{
    if(requeire(size))
    {
        memcpy(cb, &data.at(oft), size);
        oft += size;
        return true;
    }
    return false;
}

void ByteBuffer::resetoft()
{
    oft = 0;
}

int ByteBuffer::read_fd(int fd)
{

    int readnum = recv(fd, data.data()+oft, data.size()-oft, 0);
    oft += readnum;
    return readnum;
}

int ByteBuffer::write_fd(int fd)
{
    int writenum = write(fd, data.data()+oft, data.size()-oft);
    oft += writenum;
    return writenum;
}