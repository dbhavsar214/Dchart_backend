#include "Dchart_backend.h"
#include "WebSocketConnection.h"

using namespace std;

int main()
{
	BOOST_LOG_TRIVIAL(info) << "Connecting to Server....";
	WebSocketConnection::runServer(9001);
	return 0;
}
