#pragma once

#include "libtorrent/config.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/session_params.hpp"
#include "libtorrent/extensions.hpp"
#include "libtorrent/alert_types.hpp"
#include "libtorrent/bdecode.hpp"
#include <iostream>
#include "TestPlugin.hpp"

libtorrent::udp::endpoint uep(char const* ip, int port);


class SessionWrapper
{
private:
    libtorrent::session       m_session;

    lt::settings_pack generateSessionSettings(std::string addressAndPort)
    {
        libtorrent::settings_pack sp;
        sp.set_bool(libtorrent::settings_pack::enable_lsd, false);
        sp.set_bool(libtorrent::settings_pack::enable_natpmp, false);
        sp.set_bool(libtorrent::settings_pack::enable_upnp, false);
        sp.set_bool(libtorrent::settings_pack::enable_dht, true);
        //    m_sp.set_str(libtorrent::settings_pack::dht_bootstrap_nodes, "");
        sp.set_int(libtorrent::settings_pack::max_retry_port_bind, 800);
        sp.set_int(libtorrent::settings_pack::alert_mask, ~0);
        sp.set_str(libtorrent::settings_pack::listen_interfaces, addressAndPort);
        return sp;
    }


public:
    SessionWrapper(std::string addressAndPort) : m_session( generateSessionSettings( addressAndPort ) )
    {

    }

    void addExtension(std::shared_ptr<test_plugin> testPluginPtr)
    {
        m_session.add_extension(testPluginPtr);
    }

    void dhtDirectRequest(
            std::string IP,
            SessionWrapper & otherSession,
            libtorrent::entry const& e,
            libtorrent::client_data_t userdata = {} )
    {
        libtorrent::udp::endpoint endpoint = uep( IP.c_str(), otherSession.m_session.listen_port() );
        m_session.dht_direct_request( endpoint, e, userdata );
    }
};

libtorrent::udp::endpoint uep(char const* ip, int port)
{
    libtorrent::error_code ec;
    libtorrent::udp::endpoint ret(libtorrent::make_address(ip, ec), std::uint16_t(port));
    assert(!ec);
    return ret;
}
