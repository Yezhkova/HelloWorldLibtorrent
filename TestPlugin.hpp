#pragma once

#include "libtorrent/config.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/session_params.hpp"
#include "libtorrent/extensions.hpp"
#include "libtorrent/alert_types.hpp"
#include "libtorrent/bdecode.hpp"
#include <iostream>

struct test_plugin : libtorrent::plugin
{
    feature_flags_t implemented_features() override
    {
        return plugin::dht_request_feature;
    }

    bool on_dht_request(
        lt::string_view                         query,
        boost::asio::ip::udp::endpoint const&   senderEndpoint,
        lt::bdecode_node const&                 dict,
        lt::entry&                              response ) override
{
        std::cout <<" \n------------------------------------------LOG\n";

        std::cout << dict.dict_find_string_value("q") << '\n';
        if (dict.dict_find_string_value("q") == "test_good")
        {
            std::cout<<"\n------------------------------------------LOG\n" << std::flush;
            exit(0);
            response["r"]["good"] = 1;
            return true;
        }
        return false;
    }
};
