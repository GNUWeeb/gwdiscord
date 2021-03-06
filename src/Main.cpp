
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

void Main::onMessage(SleepyDiscord::Message msg)
{
	if (msg.channelID == "865960077051035648" && msg.webhookID == "") {
		forwardToGNUWeebTelegram(msg);
		return;
	}
}


void Main::forwardToGNUWeebTelegram(SleepyDiscord::Message &msg)
{
	rapidjson::Document jsonDoc;
	jsonDoc.SetObject();
	rapidjson::Document::AllocatorType &alloc = jsonDoc.GetAllocator();

	rapidjson::Value tmp;
	rapidjson::Value mentions(rapidjson::kArrayType);

	for (auto &user: msg.mentions) {
		std::string fullMention =
			user.username + "#" + user.discriminator;

		tmp.SetString(fullMention.c_str(), alloc);
		mentions.PushBack(tmp, alloc);
	}
	jsonDoc.AddMember("mentions", mentions, alloc);


	std::string fullAuthor =
		msg.author.username + "#" + msg.author.discriminator;
	tmp.SetString(fullAuthor.c_str(), alloc);
	jsonDoc.AddMember("author", tmp, alloc);


	std::string content_text;
	content_text.reserve(4096);

	for (auto &att: msg.attachments) {
		content_text += att.url + "\n";
	}

	if (msg.attachments.size() > 0)
		content_text += "\n";

	content_text += msg.content;

	tmp.SetString(content_text.c_str(), alloc);
	jsonDoc.AddMember("content", tmp, alloc);


	if (msg.referencedMessage) {
		auto rmsg = msg.referencedMessage;
		tmp.SetString(rmsg->author.username.c_str(), alloc);
	} else {
		tmp.SetString("", alloc);
	}
	jsonDoc.AddMember("ref_author", tmp, alloc);

	tmp.SetString(std::string(msg.author.ID).c_str(), alloc);
	jsonDoc.AddMember("author_id", tmp, alloc);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	jsonDoc.Accept(writer);

	const char *jsonString = strbuf.GetString();
	std::cout << jsonString << std::endl;
	sendToAPI(jsonString);
}

} /* namespace gwdiscord  */
