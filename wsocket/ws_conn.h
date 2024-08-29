#ifndef WEB_SOCKET_CONN_H
#define WEB_SOCKET_CONN_H

#include "../conn/conn.h"
#include "bytebuffer.h"
// #include "message.h"
#include "ws_packet.h"
#include "memory"

typedef void (*nt_write_cb)(char * buf,int64_t size, void* wd);

class http_conn;
// enum class MessageType;
class User;
class ws_conn : public conn
{
    public:
        enum REQUEST_CODE{
            NO_REQUEST,
            BAD_REQUEST,
            NORMAL_REQUEST
        };

    public:

        ws_conn(const http_conn&);
        
        ~ws_conn() = default;

        void init(int sockfd, const sockaddr_in &addr,
                char *, int, int, string user, string passwd, string sqlname) override;
        void close_conn(bool real_close = true) override;
        void process() override;
        bool read_once() override;
        bool write() override;
        sockaddr_in *get_address() override
        {
            return &m_address;
        }
        void initmysql_result(connection_pool *connPool) override;


        bool append2w(const ByteBuffer&) override;

    private:
        void init();

        bool recv_msg();
        bool send_msg();

        bool process_msg();

    private:
        int m_sockfd;
        sockaddr_in m_address;
        int m_TRIGMode;
        char *doc_root;
        int m_close_log;

        std::string sql_user;
        std::string sql_passwd;
        std::string sql_name;
        std::string m_host;

        std::string m_handshake_reply;

        bool ws_handshake_completed_;
        ByteBuffer  read_buf;
        std::list<ByteBuffer>  write_buf;
        locker w_lock;

        WebSocketPacket wspacket;

        nt_write_cb nt_write_cb_;
        void *nt_work_data_;

        bool inflag;
        bool outflag;

        std::shared_ptr<User> user;

};
#endif