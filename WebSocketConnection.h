#pragma once

#include <libwebsockets.h>
#include <boost/log/trivial.hpp>
#include <chrono>
#include <cstring>
#include <thread>
#include <mutex>



struct WsSession {
    bool active = true;
    bool authenticated = false;
    std::chrono::steady_clock::time_point lastSeen;
    std::string outgoingMessage;
    std::mutex mtx;
};

class WebSocketConnection {

public:

    static void runServer(int port);

private:
    static int callback(struct lws* wsi,
        enum lws_callback_reasons reason,
        void* user,
        void* in,
        size_t len);
};

