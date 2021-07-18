<?php

$f = json_decode(file_get_contents("php://input"), true);

if (!isset($f["author"], $f["mentions"], $f["content"]))
	exit(1);

require __DIR__."/../bootstrap/autoload.php";

loadConfig("telegram_bot");

$CCList = "";

if (count($f["mentions"]) > 0) {
	$pdo    = \DB::pdo();
	$query  = <<<SQL
		SELECT a.user_id, a.username, a.first_name, a.last_name
		FROM users AS a INNER JOIN discord_uname AS b ON a.user_id = b.user_id
		WHERE 
	SQL;

	$data = [];
	foreach ($f["mentions"] as $k => $m) {
		$query .= ($k ? "OR" : "")." b.username = ? ";
		$data[] = $m;
	}

	$st = $pdo->prepare($query);
	$st->execute($data);

	while ($r = $st->fetch(PDO::FETCH_ASSOC)) {
		$fullName = $r["first_name"].(isset($r["last_name"]) ? " ".$r["last_name"] : "");
		$un = isset($r["username"]) ? " (@{$r["username"]})" : "";
		$CCList .= "Cc: <a href=\"tg://user?id={$r["user_id"]}\">".htmlspecialchars($fullName)."</a>{$un}\n";
	}
}


if (isset($f["ref_author"]) && $f["ref_author"]) {
	$ident = "zx";
	if (preg_match("/(?:\({$ident}:)([a-f0-9]+)(?:\.)([a-f0-9]+)(?:\))/", $f["ref_author"], $m)) {
		// $userId  = hexdec($m[1]); // future use
		$replyTo = hexdec($m[2]) + 426500;
	} else {
		$replyTo = NULL;
	}
} else {
	$replyTo = NULL;
}

$targetChatId = -1001483770714;
$author = htmlspecialchars($f["author"]);
$text =
	"<b>{$author}:</b>\n"
	.htmlspecialchars($f["content"])
	."\n\n{$CCList}";

\TeaBot\Exe::sendChatAction(
	[
		"chat_id"	=> $targetChatId,
		"action"	=> "typing"
	]
);
usleep(1000000 * rand(0, 5));
echo \TeaBot\Exe::sendMessage(
	[
		"chat_id"		=> $targetChatId,
		"text"			=> trim($text),
		"parse_mode"		=> "HTML",
		"reply_to_message_id"	=> $replyTo
	]
)["out"];
