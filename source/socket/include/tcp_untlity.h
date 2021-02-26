#ifndef __TCP_UNTLITY_H__
#define __TCP_UNTLITY_H__

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
#include <QtNetwork/QHostAddress>

namespace Shata
{
    namespace Socket
    {
        struct SocketInfo final
        {
            QHostAddress        address;
            quint16             port;
        };
    }
}




#endif // __TCP_UNTLITY_H__