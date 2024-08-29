#ifndef CONN_H
#define CONN_H

#include <netinet/in.h>
#include <functional>
#include "../CGImysql/sql_connection_pool.h"

class ByteBuffer;
class conn 
{
    public:
        int improv;
        MYSQL *mysql;
        int m_state;
        int timer_flag;
        std::function<void(int, conn*)> replaceHandlerCallback;

        static int m_epollfd;
        static int m_user_count;
    public:
        conn() = default;
        virtual ~conn() = default;
        virtual void init(int sockfd, const sockaddr_in &addr, char *, int, int, string user, string passwd, string sqlname) = 0;
        virtual void close_conn(bool real_close = true) = 0;
        virtual bool read_once() = 0;
        virtual bool write() = 0;
        virtual void process() = 0;
        virtual sockaddr_in *get_address() = 0;
        virtual void initmysql_result(connection_pool *connPool) = 0;

        virtual bool append2w(const ByteBuffer&) = 0;
        
        virtual bool setCallback(std::function<void(int, conn*)> callback)
        {
            replaceHandlerCallback = callback;
        }
    
};


#endif