#include "PriceStreamer.h"

using json = nlohmann::json;

void startPriceStreamer(WsSession* session, struct lws* wsi, const std::string& symbol, double initialPrice, double stdDev, int intervalMS){

	double price = initialPrice;

	BOOST_LOG_TRIVIAL(info) << "Price Streaming Started";
	while (session -> active) {
		price = PriceGenerator(price, stdDev);

		json j;
		j["symbol"] = symbol;
		j["price"] = price;
		j["timestamp"] = std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count();

		std::string payload = j.dump();

		{
			std::lock_guard<std::mutex> lock(session->mtx);
			session->outgoingMessage = payload;
		}

		lws_callback_on_writable(wsi);
		lws_cancel_service(lws_get_context(wsi));

		std::this_thread::sleep_for(std::chrono::milliseconds(intervalMS));
	}


}
