<?php
function getSoftUpdate() {
	require ("../../db_config.php");
	$dbh = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_database . ";charset=utf8", $db_username, $db_password, array(PDO::ATTR_PERSISTENT => true));
	$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
	$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sqlstr = "select * from serverupdate";
	$res = $dbh->query($sqlstr);
	$softarr = Array();

	foreach ($res as $row) {
	//$row['id'];
	$arr =Array('id'=>$row['Id'],'downloadurl'=>$row['downloadUrl'],'time'=>$row['time'],'version'=>$row['version'],'exename'=>$row['exename']);

	array_push($softarr,$arr);
	}

	echo json_encode($softarr);
	$dbh = null; //(free)
	
}
function getUserUpdatelog() {
	require ("../../db_config.php");
	$dbh = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_database . ";charset=utf8", $db_username, $db_password, array(PDO::ATTR_PERSISTENT => true));
	$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
	$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sqlstr = "select * from updatelog";
	$res = $dbh->query($sqlstr);
		$logarr = Array();
	foreach ($res as $row) {
//		echo "<tr>" . "<td>" . $row['Id'] . "</td>" . "<td>" . $row['time'] . "</td>" . "<td>" . $row['json'] . "</td>" . "<td>" . $row['ver'] . "</td>" . "<td>" . $row['ip'] . "</td>" . "</tr>";
	$arr =Array('id'=>$row['Id'],'time'=>$row['time'],'json'=>$row['json'],'version'=>$row['ver'],'ip'=>$row['ip']);
array_push($logarr,$arr);
	}
		echo json_encode($logarr);
	$dbh = null; //(free)
	
}
function uploadUpdate($name,$version,$url) {
	require ("../../db_config.php");
	$dbh = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_database . ";charset=utf8", $db_username, $db_password, array(PDO::ATTR_PERSISTENT => true));
	$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
	$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sqlstr = "update serverupdate set `downloadUrl`='".$url."' ,`time`='".date("Y-m-d H:i:s", time())."',`version`='".$version."' 
		where id = 
		(
		select a.id from (
		select id from serverupdate where `exename`='".$name."'
		)a
		)";
    $res = $dbh->exec($sqlstr);
	if($res==0)
	{
		 exit('error');
	}	
	$dbh = null; //(free)
	//echo $name.$version.$url;
	
	
}
function getEditlist()
{
	$findid = $_GET['id'];

require ("../../db_config.php");
	$dbh = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_database . ";charset=utf8", $db_username, $db_password, array(PDO::ATTR_PERSISTENT => true));
	$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
	$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sqlstr = 'select * from serverupdate where id='.$findid;
    $res = $dbh->query($sqlstr);
    $arr =array();
    foreach($res as $row)
    {
        $ma = array('id'=>$row['Id'],'downloadurl'=>$row['downloadUrl'],'version'=>$row['version'],'softname'=>$row['exename'],'time'=>$row['time']);
        array_push($arr,$ma);
    }
    echo json_encode($arr);


	$dbh = null; //(free)
}


if($_GET['action']=='log')
{
    getUserUpdatelog();
}elseif($_GET['action']=='Soft')
{
    getSoftUpdate();
}elseif($_GET['action']=='upLoad')
{
   uploadUpdate($_GET['name'],$_GET['version'],$_GET['url']);
}elseif($_GET['action']=='edit')
{
	getEditlist();
}
?>	
