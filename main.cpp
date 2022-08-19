
#if !defined TORRENT_DISABLE_EXTENSIONS && !defined TORRENT_DISABLE_DHT

#include "libtorrent/config.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/session_params.hpp"
#include "libtorrent/extensions.hpp"
#include "libtorrent/alert_types.hpp"
#include "libtorrent/bdecode.hpp"
#include <iostream>
#include "log.hpp"
#include "DhtRequestHandler.hpp"
#include "SessionWrapperDelegate.hpp"
#include "SessionWrapper.hpp"

#define TEST_CHECK(expr) {assert (expr);}
#define TEST_EQUAL(expr1, expr2) {assert(expr1 == expr2); }
#define IP            "192.168.1.10"
#define IP_REQUESTER  "192.168.1.20"

using namespace lt;

namespace
{

dht_direct_response_alert* get_direct_response(lt::session& ses)
{
    for (;;)
    {
        alert* a = ses.wait_for_alert(seconds(30));
        // it shouldn't take more than 30 seconds to get a response
        // so fail the test and bail out if we don't get an alert in that time
        TEST_CHECK(a);
        if (!a) return nullptr;
        std::vector<alert*> alerts;
        ses.pop_alerts(&alerts);
        for (std::vector<alert*>::iterator i = alerts.begin(); i != alerts.end(); ++i)
        {
            if ((*i)->type() == dht_direct_response_alert::alert_type)
                return static_cast<dht_direct_response_alert*>(&**i);
        }
    }
}
}

boost::asio::ip::udp::endpoint uep(char const* ip, int port)
{
    libtorrent::error_code ec;
    boost::asio::ip::udp::endpoint ret( boost::asio::ip::make_address(ip, ec), std::uint16_t(port) );
    assert(!ec);
    return ret;
}

#endif // #if !defined TORRENT_DISABLE_EXTENSIONS && !defined TORRENT_DISABLE_DHT

class UIDelegate : public SessionWrapperDelegate

{
    void onMessage( const std::string& messageText, boost::asio::ip::udp::endpoint senderEndpoint ) override
    {
        LOG(senderEndpoint << '\t' << messageText);
    }
};

int main()
{
    UIDelegate responderTestDel;
    auto responderPtr = createLtSessionPtr(IP ":11101", std::make_shared<UIDelegate> (responderTestDel));

    UIDelegate requesterTestDel;
    auto requesterPtr = createLtSessionPtr(IP_REQUESTER ":11102", std::make_shared<UIDelegate> (requesterTestDel));

    responderPtr->start();
    // successful request
    Sleep(1000);
    requesterPtr->sendMessage(uep( IP, 11101 ), "my message");
    Sleep(120000);
}
