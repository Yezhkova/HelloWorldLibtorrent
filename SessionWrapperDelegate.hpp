#pragma once

#include "SessionWrapper.hpp"
#include "libtorrent/alert_types.hpp"
#include <iostream>

class SessionWrapperDelegate
{
public:
    virtual void onMessage( const std::string& messageText, boost::asio::ip::udp::endpoint senderEndpoint ) = 0;
};

std::shared_ptr<SessionWrapper> createLtSession( const std::string& addressAndPort, SessionWrapperDelegate& delegate )
{
    return std::make_shared<SessionWrapper>( addressAndPort, delegate );
}

