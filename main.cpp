
#if !defined TORRENT_DISABLE_EXTENSIONS && !defined TORRENT_DISABLE_DHT

#include "libtorrent/config.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/session_params.hpp"
#include "libtorrent/extensions.hpp"
#include "libtorrent/alert_types.hpp"
#include "libtorrent/bdecode.hpp"
#include <iostream>
#include "log.hpp"
#include "SessionWrapper.hpp"
#include "SessionWrapperDelegate.hpp"

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

#endif // #if !defined TORRENT_DISABLE_EXTENSIONS && !defined TORRENT_DISABLE_DHT

class TestDelegate : public SessionWrapperDelegate

{
    void onMessage( const std::string& messageText, boost::asio::ip::udp::endpoint senderEndpoint ) override
    {
        LOG(senderEndpoint << '\t' << messageText);
    }
};

int main()
{
    TestDelegate responderTestDel;
    auto responderPtr = createLtSession(IP ":11101", responderTestDel);

    TestDelegate requesterTestDel;
    auto requesterPtr = createLtSession(IP_REQUESTER ":11102", requesterTestDel);

//    SessionWrapper responder(IP ":11101");
//    SessionWrapper requester(IP_REQUESTER ":11102");

    responderPtr->addExtension(std::make_shared<test_plugin>());

//    responder.addExtension(std::make_shared<test_plugin>());

    // successful request
    Sleep(1000);
    entry r;
    r["q"] = "test_good";
    requesterPtr->dhtDirectRequest(IP, responderPtr, r, client_data_t(reinterpret_cast<int*>(12345))); // ip + responder -> endpoint
    Sleep(120000);
}
