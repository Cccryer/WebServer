#include "ws_conn.h"
#include "../http/setfd.h"
#include "../http/http_conn.h"
#include "message.h"
#include "../gamebox/game.h"
#include "../gamebox/user.h"
#include "../gamebox/game_session.h"
#include "../gamebox/game_manager.h"
#include "../gamebox/gomoku.h"
#include "../http/setfd.h"
ws_conn::ws_conn(const http_conn& http)
{
    
    m_sockfd = http.m_sockfd;
    m_address = http.m_address;
    doc_root = http.doc_root;
    m_close_log = http.m_close_log;
    mysql = http.mysql;
    m_host = http.m_host;

    sql_name = http.sql_name;
    sql_passwd = http.sql_passwd;
    sql_user= http.sql_user;
    
    m_TRIGMode = http.m_TRIGMode;

    improv = http.improv;
    m_state = http.m_state;
    timer_flag = http.timer_flag;

    ByteBuffer handFrame;
    handFrame.append(http.m_write_buf, http.m_write_idx);
    write_buf.push_back(handFrame);

    addfd(m_epollfd, m_sockfd, true, m_TRIGMode);

}

void ws_conn::init(int sockfd, const sockaddr_in &addr, char *root, int TRIGMode,
                     int close_log, string user, string passwd, string sqlname)
{
    printf("ws_conn init with param\n");
}

void ws_conn::init()
{
    printf("ws_conn init\n");
}



bool ws_conn::recv_msg()
{
    if(!ws_handshake_completed_)
    {

    }
    else
    {
        uint64_t ret = wspacket.recv_dataframe(read_buf);
        if(ret == 0)
            return 0;
        if(ret > 0xFFFFFFFF)
            printf("Attention:frame data length exceeds the max value of a uint32_t varable!\n");
        
        process_msg();
    }
    

}

bool ws_conn::send_msg()
{
    printf("send msg\n");
}


bool ws_conn::process_msg()
{
    switch (wspacket.get_opcode())
    {
        case WebSocketPacket::WSOpcode_Continue:
        {
            LOG_INFO("WebSocketEndpoint - recv a Continue opcode.")

            break;
        }
        case WebSocketPacket::WSOpcode_Text:
        {
            LOG_INFO("WebSocketEndpoint - recv a Text opcode.");
            Message* msg = Message::deserialize(std::string(wspacket.get_payload().bytes()));
            std::string res = user->getSession()->excute();

            WebSocketPacket packet;
            packet.set_fin(1);
            packet.set_opcode(0x1);
            packet.set_payload(res.c_str(), res.size());
            ByteBuffer frame;
            wspacket.pack_dataframe(frame);

            auto userList = user->getSession()->getUserList();
            bool ret;
            for(auto u : userList)
            {
                ret =  u->getConn()->append2w(frame);
                if(!ret)
                    LOG_ERROR("Failed append frame to user:%s write queue.", u->getName());
            }

            //构造帧内容
            //写入缓冲区 write_buf （多个用户）
            //addfd(EPOLLOUT)
            //发送完毕
            //

            break;
        }
        case WebSocketPacket::WSOpcode_Binary:
        {
            LOG_INFO("WebSocketEndpoint - recv a Binary opcode.");

            break;
        }
        case WebSocketPacket::WSOpcode_Close:
        {
            LOG_INFO("WebSocketEndpoint - recv a Close opcode.");

            break;
        }
        case WebSocketPacket::WSOpcode_Ping:
        {
            LOG_INFO("WebSocketEndpoint - recv a Ping opcode.");

            break;
        }
        case WebSocketPacket::WSOpcode_Pong:
        {
            LOG_INFO("WebSocketEndpoint - recv a Pong opcode.")

            break;
        }

        default:
        {
            LOG_INFO("WebSocketEndpoint - recv an unknown opcode.");

            break;
        }
    }
}

void ws_conn::process()
{
    bool ret = recv_msg();
    if (!ret)
    {
        modfd(m_epollfd, m_sockfd, EPOLLIN, m_TRIGMode);
        return;
    }
    ret = send_msg();
    modfd(m_epollfd, m_sockfd, EPOLLOUT, m_TRIGMode);
}


bool ws_conn::read_once()
{
    //oft表示已经读出了多少
    if(read_buf.getoft() >= read_buf.length())
    {
        return false;
    }
    int bytes_read;
    if(0 == m_TRIGMode)
    {
        bytes_read = read_buf.read_fd(m_sockfd);
        if(bytes_read <= 0)
            return false;
        return true;
    }
    //ET读
    else
    {
        while(true)
        {
            bytes_read = read_buf.read_fd(m_sockfd);
            if (bytes_read == -1)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    break;
                return false;
            }
            else if (bytes_read == 0)
            {
                return false;
            }
        }
        return true;
    }
}

bool ws_conn::write()
{
    //oft表示已经发送了多少
    if(write_buf.empty())
    {
        modfd(m_epollfd, m_sockfd, EPOLLIN, m_TRIGMode);
        return true;
    }
    int writenum = 0;
    while(!write_buf.empty())
    {
        w_lock.lock();
        ByteBuffer frame = write_buf.front();
        write_buf.pop_front();
        w_lock.unlock();
        while(1)
        {
            writenum = frame.write_fd(m_sockfd);
            if(writenum < 0)
            {
                if(errno == EAGAIN)
                {
                    modfd(m_epollfd, m_sockfd, EPOLLOUT, m_TRIGMode);
                    return true;
                }
                return false;
            }
            
            if(frame.getoft() >= frame.length())
                break;
        }
    }
    modfd(m_epollfd, m_sockfd, EPOLLIN, m_TRIGMode);
    return true;

}


bool ws_conn::append2w(const ByteBuffer& frame)
{
    w_lock.lock();
    write_buf.push_back(frame);
    w_lock.unlock();
    modfd(m_epollfd, m_sockfd, EPOLLOUT, m_TRIGMode);
    return true;
}


void ws_conn::initmysql_result(connection_pool *connPool)
{
    printf("ws_conn init mysql\n");
}

 void ws_conn::close_conn(bool real_close)
 {
    printf("ws_conn close\n");
 }