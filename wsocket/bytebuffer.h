#ifndef BYTE_BUFFER_H
#define BYTE_BUFFER_H

#include<vector>
#include<stdint.h>
class ByteBuffer
{
    private:
        std::vector<char> data;
        uint32_t oft;
    public:
        ByteBuffer(int sz = 2048);
        virtual ~ByteBuffer() = default;
    public:
        virtual int length();

        virtual char *bytes();
        
        virtual void erase(int size);

        virtual void append(const char *bytes, int size);

        //获取目前偏移指针
        virtual char *curat();

        //获取目前偏移值
        virtual int getoft();

        //检查是否有足够大小
        virtual bool requeire(int size);

        //从当前位置前跳
        virtual bool skip_x(int size);

        //读取size字节并移动当前位置
        virtual bool read_bytes_x(char *cb, int size);

        //重置偏移指针到开头
        virtual void resetoft();

        virtual int read_fd(int);
        
        virtual int write_fd(int);
};


#endif