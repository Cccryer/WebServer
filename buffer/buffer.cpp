#include "buffer.h"
#include<assert.h>
#include<cstring>
#include<sys/uio.h>
#include<unistd.h>

Buffer::Buffer(int initBuffSize) : buffer_(initBuffSize), readPos_(0), writePos_(0) {}

//剩余可读
size_t Buffer::ReadableBytes() const {
    return writePos_ - readPos_;
}

//剩余可写
size_t Buffer::WritableBytes() const {
    return buffer_.size() - writePos_;
}

//readPos大小
size_t Buffer::PrependableBytes() const {
    return readPos_;
}

//readPos所指
const char* Buffer::Peek() const {
    return BeginPtr_() + readPos_;
}

//readPos前移
void Buffer::Retrieve(size_t len) {
    assert(len <= ReadableBytes());
    readPos_ += len;
}

//readPos前移到指定位置
void Buffer::RetrieveUntil(const char* end) {
    assert(Peek() <= end );
    Retrieve(end - Peek());
}

//清空
void Buffer::RetrieveAll() {
    bzero(&buffer_[0], buffer_.size());
    readPos_ = 0;
    writePos_ = 0;
}

//提取出剩余元素
std::string Buffer::RetrieveAllToStr() {
    std::string str(Peek(), ReadableBytes());
    RetrieveAll();
    return str;
}

//writePos位置元素 const
const char* Buffer::BeginWriteConst() const {
    return BeginPtr_() + writePos_;
}

//writePos位置元素
char* Buffer::BeginWrite() {
    return BeginPtr_() + writePos_;
}

//writePos前移
void Buffer::HasWritten(size_t len) {
    writePos_ += len;
} 

//添加string
void Buffer::Append(const std::string& str) {
    Append(str.data(), str.length());
}

//添加任意
void Buffer::Append(const void* data, size_t len) {
    assert(data);
    Append(static_cast<const char*>(data), len);
}

//添加char*
void Buffer::Append(const char* str, size_t len) {
    assert(str);
    EnsureWriteable(len);
    std::copy(str, str + len, BeginWrite());
    HasWritten(len);
}

//添加Buffer
void Buffer::Append(const Buffer& buff) {
    Append(buff.Peek(), buff.ReadableBytes());
}

//确保可写（可能删除以读元素）
void Buffer::EnsureWriteable(size_t len) {
    if(WritableBytes() < len) {
        MakeSpace_(len);
    }
    assert(WritableBytes() >= len);
}

//读
ssize_t Buffer::ReadFd(int fd, int* saveErrno) {
    char buff[65535];
    struct iovec iov[2];
    const size_t writable = WritableBytes();
    /* 分散读， 保证数据全部读完 */
    iov[0].iov_base = BeginPtr_() + writePos_;
    iov[0].iov_len = writable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    const ssize_t len = readv(fd, iov, 2);
    if(len < 0) {
        *saveErrno = errno;
    }
    else if(static_cast<size_t>(len) <= writable) {
        writePos_ += len;
    }
    else {
        writePos_ = buffer_.size();
        Append(buff, len - writable);
    }
    return len;
}


ssize_t Buffer::WriteFd(int fd, int* saveErrno) {
    size_t readSize = ReadableBytes();
    ssize_t len = write(fd, Peek(), readSize);
    if(len < 0) {
        *saveErrno = errno;
        return len;
    } 
    readPos_ += len;
    return len;
}

//起始指针
char* Buffer::BeginPtr_() {
    return &*buffer_.begin();
}

//起始指针 const
const char* Buffer::BeginPtr_() const {
    return &*buffer_.begin();
}

//扩展或优化空间，可能删除已读过元素
void Buffer::MakeSpace_(size_t len) {
    if(WritableBytes() + PrependableBytes() < len) {
        buffer_.resize(writePos_ + len + 1);
    } 
    else {
        size_t readable = ReadableBytes();
        std::copy(BeginPtr_() + readPos_, BeginPtr_() + writePos_, BeginPtr_());
        readPos_ = 0;
        writePos_ = readPos_ + readable;
        assert(readable == ReadableBytes());
    }
}