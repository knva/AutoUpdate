<?php
//get & send
//Parameter1:get name
//Parameter2:get version
//Parameter3:pdo handle
function flushmJson($name, $ver, $dbh) {
	$userinfo = create_unique();
	if (!addLog($dbh, $name, $ver, $userinfo)) {
		$str = array('result' => 'failed');
		//echo json_encode($str);
		return False;
	}
	$exename = $name;
	$sqlstr = "SELECT downloadUrl ,version FROM `serverupdate` WHERE version > " . $ver . "  AND `exename` = '" . $exename . "'";
	//  $dbh   = new PDO("mysql:host=localhost;dbname=update","root","root");
	$rs = $dbh->query($sqlstr);
	if ($rs->rowCount()) {
		while ($row = $rs->fetch()) {
			$allurl = explode(',', $row[0]);
			$allfilename = array();
			foreach ($allurl as $url) {
				array_push($allfilename, basename($url));
			}
			$result = array("status" => "ok", 'ver' => intval($row[1]), "url" => $allurl, "file" => $allfilename);
			//$str = array('result'=>'ok','url'=>$row[0],'ver'=>intval($row[1]));
			$json = json_encode($result);
			echo $json;
			updateLog($dbh, $json, $userinfo);
		}
	} else {
		$str = array('result' => 'failed');
		$json = json_encode($str);
		echo $json;
		updateLog($dbh, $json, $userinfo);
		return False;
	}
}
//Add a log to the database
//Parameter1:pdo handle
//Parameter2:update soft name
//Parameter3:ver
//Parameter4:user id
function addLog($dbh, $json, $ver, $userinfo) {
	$ip = GetIP();
	$sqlstr = "insert into `updatelog` (`time`,`json`,`ver`,`ip`,`agent`) values ('" . date("Y-m-d H:i:s", time()) . "','" . $json . "','" . $ver . "','" . $ip . "','" . $userinfo . "')";
	//echo $sqlstr;
	if ($dbh->exec($sqlstr)) {
		return True;
	} else {
		return False;
	}
}
//Updata log
//Parameter1:pdo handle
//Parameter2:update soft result
//Parameter4:user id
function updateLog($dbh, $result, $userinfo) {
	$sqlstr = "UPDATE `updatelog` SET `result` = '" . $result . "' WHERE (`agent`='" . $userinfo . "')";
	//echo $sqlstr;
	if ($dbh->exec($sqlstr)) {
		return True;
	} else {
		return False;
	}
}
//create user id
function create_unique() {
	$data = $_SERVER['HTTP_USER_AGENT'] . $_SERVER['REMOTE_ADDR'] . time() . rand();
	return sha1($data); //return md5(time().$data);   //return $data;
	
}
//get ip
function GetIP() {
	if (getenv("HTTP_CLIENT_IP") && strcasecmp(getenv("HTTP_CLIENT_IP"), "unknown")) $ip = getenv("HTTP_CLIENT_IP");
	else if (getenv("HTTP_X_FORWARDED_FOR") && strcasecmp(getenv("HTTP_X_FORWARDED_FOR"), "unknown")) $ip = getenv("HTTP_X_FORWARDED_FOR");
	else if (getenv("REMOTE_ADDR") && strcasecmp(getenv("REMOTE_ADDR"), "unknown")) $ip = getenv("REMOTE_ADDR");
	else if (isset($_SERVER['REMOTE_ADDR']) && $_SERVER['REMOTE_ADDR'] && strcasecmp($_SERVER['REMOTE_ADDR'], "unknown")) $ip = $_SERVER['REMOTE_ADDR'];
	else $ip = "unknown";
	return ($ip);
}
/**
 * 是否是AJAx提交的
 * @return bool
 */
function isAjax() {
	if (isset($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest') {
		return true;
	} else {
		return false;
	}
}
/**
 * 是否是GET提交的
 */
function isGet() {
	return $_SERVER['REQUEST_METHOD'] == 'GET' ? true : false;
}
/**
 * 是否是POST提交
 * @return int
 */
function isPost() {
	return $_SERVER['REQUEST_METHOD'] == 'POST' ? 1 : 0;
}
//main
if ((isset($_GET["name"]) && isset($_GET["version"]) && isset($_GET["publicKey"])) || (isset($_POST["name"]) && isset($_POST["version"]) && isset($_POST["publicKey"]))) {
	if (isGet()) {
		$mname = $_GET["name"];
		$version = $_GET["version"];
		$publicKey = $_GET["publicKey"];
	} elseif (isPost()) {
		$mname = $_POST["name"];
		$version = $_POST["version"];
		$publicKey = $_POST["publicKey"];
	}
	if ($publicKey == md5('595902716' . '@qq.com' . floor(time() / 100)) || $publicKey == "595902716") {
		$dbh = new PDO("mysql:host=localhost;dbname=update;charset=utf8", "root", "root", array(PDO::ATTR_PERSISTENT => true));
		$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
		$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		flushmJson($mname, $version, $dbh);
		$dbh = null; //(free)
		
	} else {
		echo 'Are you kidding me?';
	}
} else {
	echo 'You need some salt!';
}
?>