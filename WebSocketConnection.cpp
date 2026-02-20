#include "WebSocketConnection.h"
#include "PriceStreamer.h"


int WebSocketConnection::callback(struct lws* wsi,
    enum lws_callback_reasons reason,
    void* user,
    void* in,
    size_t len)
{
    WsSession* session = reinterpret_cast<WsSession*>(user);

    switch (reason) {

    case LWS_CALLBACK_ESTABLISHED: {
  
        session->active = true;
        session->lastSeen = std::chrono::steady_clock::now();


        BOOST_LOG_TRIVIAL(info) << "Client connected";

        std::thread streamer(startPriceStreamer, session, wsi, "XYZ", 105.0, 0.5, 500);
        streamer.detach();

        break;
    }

    case LWS_CALLBACK_RECEIVE: {
        session->lastSeen = std::chrono::steady_clock::now();

        std::string msg(reinterpret_cast<char*>(in), len);
        BOOST_LOG_TRIVIAL(info) << "Received message: " << msg;

     
        if (msg == "ping") {
            session->outgoingMessage = "pong";

            lws_callback_on_writable(wsi);
        }
        else {
            BOOST_LOG_TRIVIAL(warning) << "Unknown message";
        }
        break;
    }

    case LWS_CALLBACK_SERVER_WRITEABLE: {
        std::lock_guard<std::mutex> lock(session->mtx);
        if (!session->outgoingMessage.empty()) {

            std::string& payload = session->outgoingMessage;
          
            unsigned char buffer[LWS_PRE + 512];
            unsigned char* p = &buffer[LWS_PRE];

            size_t n = payload.size();
            std::memcpy(p, payload.c_str(), n);

            lws_write(wsi, p, n, LWS_WRITE_TEXT);
            BOOST_LOG_TRIVIAL(info) << "Sent message: " << payload;
            session->outgoingMessage.clear();
            
        }
        break;
    }

    case LWS_CALLBACK_CLOSED: {
        session->active = false;
        BOOST_LOG_TRIVIAL(info) << "Client disconnected";
        break;
    }

    case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE: {
        BOOST_LOG_TRIVIAL(info) << "Peer initiated close";
        break;
    }

    default:
        break;
    }

    return 0;
}

void WebSocketConnection::runServer(int port) {
    static struct lws_protocols protocols[] = {
        {
            "ws-protocol",
            WebSocketConnection::callback,
            sizeof(WsSession),
            512,
        },
        { nullptr, nullptr, 0, 0 }
    };

    struct lws_context_creation_info info = {};
        info.port = port;
        info.protocols = protocols;
        info.gid = -1;
        info.uid = -1;
  

    struct lws_context* context = lws_create_context(&info);
    if (!context) {
        BOOST_LOG_TRIVIAL(error) << "Failed to create context";
        return;
    }

    BOOST_LOG_TRIVIAL(info) << "WebSocket server running on port " << port;

    while (true) {
        lws_service(context, 50);
    }

    lws_context_destroy(context);

};



