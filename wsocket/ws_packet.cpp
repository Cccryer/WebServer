/*
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-------+-+-------------+-------------------------------+
|F|R|R|R| opcode|M| Payload len |    Extended payload length    |
|I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
|N|V|V|V|       |S|             |   (if payload len==126/127)   |
| |1|2|3|       |K|             |                               |
+-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
|     Extended payload length continued, if payload len == 127  |
+ - - - - - - - - - - - - - - - +-------------------------------+
|                               |Masking-key, if MASK set to 1  |
+-------------------------------+-------------------------------+
| Masking-key (continued)       |          Payload Data         |
+-------------------------------- - - - - - - - - - - - - - - - +
:                     Payload Data continued ...                :
+ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
|                     Payload Data continued ...                |
+---------------------------------------------------------------+

*/




#include<arpa/inet.h>
#include"ws_packet.h"

unsigned long long ntohll(unsigned long long v) {
    union { unsigned long lv[2]; unsigned long long llv; } u;
    u.llv = v;
    return ((unsigned long long)ntohl(u.lv[0]) << 32) | (unsigned long long)ntohl(u.lv[1]);
}

unsigned long long htonll(unsigned long long v) {
    union { unsigned long lv[2]; unsigned long long llv; } u;
    u.lv[0] = htonl(v >> 32);
    u.lv[1] = htonl(v & 0xFFFFFFFFULL);
    return u.llv;
}




uint64_t WebSocketPacket::recv_dataframe(ByteBuffer &input)
{   
    int header_size = fetch_frame_info(input);

    if(payload_length_ + header_size > input.length())
    {
        printf("WebSocketPacket:recv_dataframe: continue recving data.\n");
        input.resetoft();
        return 0;
    }
    fetch_payload(input);

    printf("WebSocketPacket: received data with header size: %d, payload size: %d, input size: %d", header_size, payload_length_, input.getoft());

    return input.getoft();

}

int32_t WebSocketPacket::pack_dataframe(ByteBuffer &output)
{
    uint8_t onebyte = 0;
    onebyte |= (fin_ << 7);
    onebyte |= (rsv1_ << 6);
    onebyte |= (rsv2_ << 5);
    onebyte |= (rsv3_ << 4);
    onebyte |= (opcode_ & 0x0F);
    output.append((char*)&onebyte, 1);

    onebyte = 0;
    onebyte = onebyte | (mask_ << 7);
    uint8_t length_type_ = get_length_type();

    if(length_type_ < 126)
    {
        onebyte |= payload_length_;
        output.append((char*)&onebyte, 1);
    }
    else if(length_type_ == 126)
    {
        onebyte |= length_type_;
        output.append((char*)onebyte, 1);

        uint16_t l = htons(payload_length_);
        output.append((char*)&l, 2);
    }
    else if(length_type_ > 126)
    {
        onebyte |= length_type_;
        output.append((char*)onebyte, 1);

        unsigned long long ll = htonll(payload_length_);
        output.append((char*)&ll, 8);
    }
    else
        return -1;
    
    if(mask_ == 1)
    {
        char value = 0;
        output.append((char*)masking_key_, 4);
        printf("WebSocketPacket: send data with header size: %d, payload size: %d\n", output.length(), payload_length_);

        for(uint64_t i = 0; i < payload_length_; i++)
        {
            payload_.read_bytes_x(&value, 1);
            value = value ^ masking_key_[i%4];
            output.append(&value, 1);
        }
    }
    else
    {
        printf("WebSocketPacket: send data with header size: %d, payload size: %d\n", output.length(), payload_length_);
        output.append(payload_.bytes(), payload_.length());
    }
}

int32_t WebSocketPacket::fetch_frame_info(ByteBuffer &input)
{
    uint8_t onebyte = 0;
    input.read_bytes_x((char*)&onebyte, 1);
    fin_ = onebyte >> 7;
    opcode_ = onebyte & 0x7F;
    
    input.read_bytes_x((char*)onebyte, 1);
    mask_ = onebyte >> 7 & 0x01;
    length_type_ = onebyte & 0x7F;

    if(length_type_ < 126)
    {
        payload_length_ = length_type_;
    }
    else if(length_type_ == 126)
    {
        uint16_t len = 0;
        input.read_bytes_x((char*)&len, 2);
        payload_length_ = ntohs(len);

    }
    else if(length_type_ == 127)
    {
        unsigned long long len = 0;
        input.read_bytes_x((char*)&len, 8);
        payload_length_ = ntohll(len);

    }
    else
    {}
    if(mask_ == 1)
    {
        input.read_bytes_x((char *)masking_key_, 4);
    }
    return input.getoft();
}

int32_t WebSocketPacket::fetch_payload(ByteBuffer &input)
{
    char real = 0;
    if(mask_ == 1)
    {
        for(uint64_t i = 0; i < payload_length_; i++)
        {
            input.read_bytes_x(&real, 1);
            real = real^masking_key_[i%4];
            payload_.append(&real, 1);
        }
    }
    else
    {
        payload_.append(input.curat(), payload_length_);
        input.skip_x(payload_length_);
    }
    return 0;
}


const uint8_t WebSocketPacket::get_length_type()
{
    if(payload_length_ < 126)
        return (uint8_t)payload_length_;
    else if(payload_length_ >= 126 && payload_length_ <= 0xFFFF)
        return 126;
    else
        return 127;
}

const uint8_t WebSocketPacket::get_header_size()
{
    int header_size = 0;
    const uint8_t len_type = get_length_type();
    if(len_type < 126)
        header_size = 2;
    else if(len_type == 126)
        header_size = 4;
    else if(len_type > 126)
        header_size = 10;
    if(mask_ == 1)
        header_size += 4;

    return header_size;        
}

void WebSocketPacket::set_payload(const char* buf, uint64_t size)
{
    payload_.append(buf, size);
    payload_length_ = payload_.length();
}