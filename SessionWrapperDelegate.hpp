#pragma once

//#include "SessionWrapper.hpp"
#include "libtorrent/alert_types.hpp"
#include <iostream>

class SessionWrapperAbstract : public std::enable_shared_from_this<SessionWrapperAbstract>
{
public:
    virtual void start() = 0;
    virtual void sendMessage( boost::asio::ip::udp::endpoint endPoint, const std::string& text ) = 0;

};

class SessionWrapperDelegate
{
public:
    virtual void onMessage( const std::string& messageText, boost::asio::ip::udp::endpoint senderEndpoint ) = 0;
};

//std::shared_ptr<SessionWrapperAbstract> createLtSessionPtr( const std::string& addressAndPort, SessionWrapperDelegate& delegate );

