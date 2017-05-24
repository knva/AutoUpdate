<?php
if (isset($_GET["name"]) && isset($_GET["version"]) && isset($_GET["publicKey"])) {
	$mname = $_GET["name"];
	$version = $_GET["version"];
	$publicKey = $_GET["publicKey"];
	if ($publicKey == md5('595902716' . '@qq.com')) {
		$dbh = new PDO("mysql:host=localhost;dbname=update", "root", "root", array(PDO::ATTR_PERSISTENT => true));
		flushmJson($mname, $version, $dbh);
		$dbh = null; //(释放)
		
	} else {
		echo 'Are you kidding me?';
	}
} else {
	echo 'You need some salt!';
}
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
function updateLog($dbh, $result, $userinfo) {
	$sqlstr = "UPDATE `updatelog` SET `result` = '" . $result . "' WHERE (`agent`='" . $userinfo . "')";
	//echo $sqlstr;
	if ($dbh->exec($sqlstr)) {
		return True;
	} else {
		return False;
	}
}
function create_unique(){
    $data = $_SERVER['HTTP_USER_AGENT'].$_SERVER['REMOTE_ADDR'].time().rand();
    return sha1($data);//return md5(time().$data);   //return $data;   
}

function GetIP() {
	if (getenv("HTTP_CLIENT_IP") && strcasecmp(getenv("HTTP_CLIENT_IP"), "unknown")) $ip = getenv("HTTP_CLIENT_IP");
	else if (getenv("HTTP_X_FORWARDED_FOR") && strcasecmp(getenv("HTTP_X_FORWARDED_FOR"), "unknown")) $ip = getenv("HTTP_X_FORWARDED_FOR");
	else if (getenv("REMOTE_ADDR") && strcasecmp(getenv("REMOTE_ADDR"), "unknown")) $ip = getenv("REMOTE_ADDR");
	else if (isset($_SERVER['REMOTE_ADDR']) && $_SERVER['REMOTE_ADDR'] && strcasecmp($_SERVER['REMOTE_ADDR'], "unknown")) $ip = $_SERVER['REMOTE_ADDR'];
	else $ip = "unknown";
	return ($ip);
}
?>