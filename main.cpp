
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

	curl_global_init(CURL_GLOBAL_ALL);
	gwdiscord::Main client(token, SleepyDiscord::USER_CONTROLED_THREADS);
	client.run();
	return 0;
}
