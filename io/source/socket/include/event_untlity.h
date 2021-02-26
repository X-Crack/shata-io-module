#ifndef __TCP_UNTLITY_H__
#define __TCP_UNTLITY_H__
#include <event_libqt.h>
#include <functional>
#include <unordered_map>
#include <memory>
#include <atomic>
/*!
 *
 * file: tcp_untlity.h
 *
 * author: JoYous(master@crack.ee)
 *
 * reviser: 
 *
 * date: 2021/02/25
 *
 */
namespace Shata
{
    namespace Socket
    {
        class TcpSession;

        typedef std::function<bool(const std::shared_ptr<TcpSession>&, const u96)>                                          InterfaceConnection;
        typedef std::function<bool(const std::shared_ptr<TcpSession>&, const u96)>                                          InterfaceDisconnect;
        typedef std::function<bool(const std::shared_ptr<TcpSession>&, QIODevice*, const u96)>                              InterfaceMessage;
        typedef std::function<bool(const std::shared_ptr<TcpSession>&, const i64, const u96)>                               InterfaceSendReceipt;
        typedef std::function<bool(const std::shared_ptr<TcpSession>&, QAbstractSocket::SocketError, const u96)>            InterfaceDisplayError;
    }
}




#endif // __TCP_UNTLITY_H__