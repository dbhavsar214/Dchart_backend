#pragma once

#include "PriceGenerator.h"
#include "WebSocketConnection.h" 

#include <boost/log/trivial.hpp>
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>
#include <mutex>

void startPriceStreamer(WsSession* session, struct lws* wsi, const std::string& symbol, double initialPrice, double stdDev, int intervalMs);
