
#include <cstdio>
#include <cstdlib>
#include "src/Main.hpp"

int main(void)
{
	const char *token;
	token = getenv("DISCORD_BOT_TOKEN");
	if (!token) {
		printf("Missing DISCORD_BOT_TOKEN var!\n");
		return 1;
	}

	gwdiscord::Main client(token, SleepyDiscord::USER_CONTROLED_THREADS);
	client.setIntents(SleepyDiscord::Intent::SERVER_MESSAGES);
	client.run();
	return 0;
}
