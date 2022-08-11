
#if !defined TORRENT_DISABLE_EXTENSIONS && !defined TORRENT_DISABLE_DHT

#include "libtorrent/config.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/session_params.hpp"
#include "libtorrent/extensions.hpp"
#include "libtorrent/alert_types.hpp"
#include "libtorrent/bdecode.hpp"
#include <iostream>
#include <windows.h>
#define TEST_CHECK(expr) {assert (expr);}
#define TEST_EQUAL(expr1, expr2) {assert(expr1 == expr2); }

using namespace lt;

namespace
{

struct test_plugin : plugin
{
    feature_flags_t implemented_features() override
    {
        return plugin::dht_request_feature;
    }

    bool on_dht_request(string_view /* query */
                        , udp::endpoint const& /* source */, bdecode_node const& message
                        , entry& response) override
    {
        std::cout <<" \n------------------------------------------LOG\n";

        std::cout << message.dict_find_string_value("q") << '\n';
        if (message.dict_find_string_value("q") == "test_good")
        {

            std::cout<<"\n------------------------------------------LOG\n" << std::flush;
            exit(0);
            response["r"]["good"] = 1;
            return true;
        }
        return false;
    }
};

udp::endpoint uep(char const* ip, int port)
{
    error_code ec;
    udp::endpoint ret(make_address(ip, ec), std::uint16_t(port));
    TEST_CHECK(!ec);
    return ret;
}

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

int main()
{
    settings_pack sp;
    sp.set_bool(settings_pack::enable_lsd, false);
    sp.set_bool(settings_pack::enable_natpmp, false);
    sp.set_bool(settings_pack::enable_upnp, false);
    sp.set_bool(settings_pack::enable_dht, true);
    //    sp.set_str(settings_pack::dht_bootstrap_nodes, "");
    sp.set_int(settings_pack::max_retry_port_bind, 800);

    sp.set_int(lt::settings_pack::alert_mask, ~0);

#  define IP            "192.168.1.10"
#  define IP_REQUESTER  "192.168.1.20"

        // TODO : make wrapper, add funcrions add_extension, dht_direct_request
    // TODO : settingsPack -> to constructor

    sp.set_str(settings_pack::listen_interfaces, IP ":11101");
    lt::session responder(session_params(sp, {}));
    sp.set_str(settings_pack::listen_interfaces, IP_REQUESTER ":11102");
    lt::session requester(session_params(sp, {}));

    responder.add_extension(std::make_shared<test_plugin>());

    // successful request
    Sleep(1000);
    entry r;
    r["q"] = "test_good";
    requester.dht_direct_request(uep(IP, responder.listen_port())
                                 , r, client_data_t(reinterpret_cast<int*>(12345)));

    for(;;)
    {
        std::vector<lt::alert*> alerts;
        responder.pop_alerts(&alerts);
        for (lt::alert const* a : alerts) {
            std::cout << a->message() << std::endl;
        }
    }

    //        abort.emplace_back(responder.abort());
    //        abort.emplace_back(requester.abort());
}
