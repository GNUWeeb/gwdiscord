
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
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		curl_easy_cleanup(curl);
	}
}

void Main::onMessage(SleepyDiscord::Message message)
{
	size_t len = message.mentions.size();
	if (len > 0) {
		rapidjson::Document jsonDoc;
		jsonDoc.SetObject();
		rapidjson::Value myArray(rapidjson::kArrayType);
		rapidjson::Document::AllocatorType &allocator
			= jsonDoc.GetAllocator();

		for (auto &user: message.mentions) {
			rapidjson::Value tmp;
			std::string fullMention
				= user.username + "#" + user.discriminator;
			tmp.SetString(fullMention.c_str(), allocator);
			myArray.PushBack(tmp, allocator);
		}

		rapidjson::Value tmp;
		std::string fullAuthor = message.author.username + "#"
			+ message.author.discriminator;

		tmp.SetString(fullAuthor.c_str(), allocator);
		jsonDoc.AddMember("author", tmp, allocator);
		tmp.SetString(message.content.c_str(), allocator);
		jsonDoc.AddMember("content", tmp, allocator);
		jsonDoc.AddMember("mentions", myArray, allocator);
		rapidjson::StringBuffer strbuf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		jsonDoc.Accept(writer);

		const char *jsonString = strbuf.GetString();
		std::cout << jsonString << std::endl;
		sendToAPI(jsonString);
	}
}

} /* namespace gwdiscord  */
