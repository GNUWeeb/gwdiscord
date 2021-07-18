
#ifndef SRC__MAIN__HPP
#define SRC__MAIN__HPP

#include "sleepy_discord/sleepy_discord.h"

namespace gwdiscord {

class Main: public SleepyDiscord::DiscordClient {
public:
	using SleepyDiscord::DiscordClient::DiscordClient;
	void onMessage(SleepyDiscord::Message message) override;
};

} /* namespace gwdiscord  */

#endif
