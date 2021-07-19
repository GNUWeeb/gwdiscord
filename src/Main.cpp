
#include <iostream>
#include "Main.hpp"

namespace gwdiscord {

static void sendToAPI(const char *str)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://telegram-bot.teainside.org/gwdiscord.php");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, str);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		curl_easy_cleanup(curl);
	}
}

void Main::onMessage(SleepyDiscord::Message message)
{
	if (message.channelID == "865960077051035648") {
		forwardToGNUWeebTelegram(message);
		return;
	}
}


void Main::forwardToGNUWeebTelegram(SleepyDiscord::Message &message)
{
	rapidjson::Document jsonDoc;
	jsonDoc.SetObject();
	rapidjson::Document::AllocatorType &alloc = jsonDoc.GetAllocator();

	rapidjson::Value tmp;
	rapidjson::Value mentions(rapidjson::kArrayType);

	for (auto &user: message.mentions) {
		std::string fullMention =
			user.username + "#" + user.discriminator;

		tmp.SetString(fullMention.c_str(), alloc);
		mentions.PushBack(tmp, alloc);
	}
	jsonDoc.AddMember("mentions", mentions, alloc);


	std::string fullAuthor =
		message.author.username + "#" + message.author.discriminator;
	tmp.SetString(fullAuthor.c_str(), alloc);
	jsonDoc.AddMember("author", tmp, alloc);


	tmp.SetString(message.content.c_str(), alloc);
	jsonDoc.AddMember("content", tmp, alloc);


	if (message.referencedMessage) {
		auto rmsg = message.referencedMessage;
		tmp.SetString(rmsg->author.username.c_str(), alloc);
	} else {
		tmp.SetString("", alloc);
	}
	jsonDoc.AddMember("ref_author", tmp, alloc);


	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	jsonDoc.Accept(writer);

	const char *jsonString = strbuf.GetString();
	std::cout << jsonString << std::endl;
}

} /* namespace gwdiscord  */
