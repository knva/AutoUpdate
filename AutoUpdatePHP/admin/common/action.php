<?php
	require ("../../db_config.php");
	require ("../app/MMysql.php");
	$configArr = Array('host'=>$db_host,'port'=>'3306','user'=>$db_username,'passwd'=>$db_password,'dbname'=>$db_database);
	
	$mysql= new MMysql($configArr);

function getSoftUpdate() {

	// $dbh = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_database . ";charset=utf8", $db_username, $db_password, array(PDO::ATTR_PERSISTENT => true));
	// $dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
	// $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		// $sqlstr = "select * from serverupdate ORDER BY id";
	// $res = $dbh->query($sqlstr);
global 	$mysql ;
	$sqlstr = "select * from serverupdate ORDER BY id";
	$res = $mysql->doSql($sqlstr);
	$softarr = Array();

	foreach ($res as $row) {
	//$row['id'];
	$arr =Array('id'=>$row['Id'],'downloadurl'=>$row['downloadUrl'],'path'=>$row['path'],'time'=>$row['time'],'version'=>$row['version'],'exename'=>$row['exename']);

	array_push($softarr,$arr);
	}

	echo json_encode($softarr);
	$dbh = null; //(free)
	
}
function getUserUpdatelog() {
	// require ("../../db_config.php");
	global 	$mysql ;
	// $dbh = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_database . ";charset=utf8", $db_username, $db_password, array(PDO::ATTR_PERSISTENT => true));
	// $dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
	// $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	//$sqlstr = "select * from updatelog";
$res = $mysql->field('Id,time,json,ver,ip')
    ->order('Id desc')
    ->limit(1,20)
    ->select('updatelog');

	//$res = $mysql->doSql($sqlstr);
		$logarr = Array();
	foreach ($res as $row) {
//		echo "<tr>" . "<td>" . $row['Id'] . "</td>" . "<td>" . $row['time'] . "</td>" . "<td>" . $row['json'] . "</td>" . "<td>" . $row['ver'] . "</td>" . "<td>" . $row['ip'] . "</td>" . "</tr>";
	$arr =Array('id'=>$row['Id'],'time'=>$row['time'],'json'=>$row['json'],'version'=>$row['ver'],'ip'=>$row['ip']);
array_push($logarr,$arr);
	}
		echo json_encode($logarr);
	$dbh = null; //(free)
	
}
function uploadUpdate($name,$path,$version,$url) {

	// require ("../../db_config.php");
	global 	$mysql ;
	// $dbh = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_database . ";charset=utf8", $db_username, $db_password, array(PDO::ATTR_PERSISTENT => true));
	// $dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
	// $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sqlstr = "update serverupdate set `downloadUrl`='".$url."',`path`='".$path."' ,`time`='".date("Y-m-d H:i:s", time())."',`version`='".$version."' 
		where id = 
		(
		select a.id from (
		select id from serverupdate where `exename`='".$name."'
		)a
		)";
    $res = $mysql->doSql($sqlstr);
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
global 	$mysql ;
// require ("../../db_config.php");
// 	$dbh = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_database . ";charset=utf8", $db_username, $db_password, array(PDO::ATTR_PERSISTENT => true));
// 	$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
// 	$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sqlstr = 'select * from serverupdate where id='.$findid;
   $res = $mysql->doSql($sqlstr);
    $arr =array();
    foreach($res as $row)
    {
        $ma = array('id'=>$row['Id'],'downloadurl'=>$row['downloadUrl'],'path'=>$row['path'],'version'=>$row['version'],'softname'=>$row['exename'],'time'=>$row['time']);
        array_push($arr,$ma);
    }
    echo json_encode($arr);


	$dbh = null; //(free)
}

function newUpdateObject()
{
	global 	$mysql ;
// require ("../../db_config.php");
// 	$dbh = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_database . ";charset=utf8", $db_username, $db_password, array(PDO::ATTR_PERSISTENT => true));
// 	$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
// 	$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sqlstr = 'insert into serverupdate (downloadUrl,path,version,exename) 
	(select \'/\',\'/\',0,\'未命名\' from serverupdate where not exists 
	(select * from serverupdate where exename=\'未命名\') limit 1)';
$res = $mysql->doSql($sqlstr);
	if($res==0)
	{
		 exit('0');
	}	
	$dbh = null; //(free)
}

function deleteObject()
{
	global 	$mysql ;
	$name = urldecode($_GET['name']);
	// //echo $name;
	// require ("../../db_config.php");
	// $dbh = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_database . ";charset=utf8", $db_username, $db_password, array(PDO::ATTR_PERSISTENT => true));
	// $dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
	// $dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sqlstr = "delete from serverupdate  where id =".$name;
	//echo $sqlstr;
	$res = $mysql->doSql($sqlstr);
	if($res==0)
	{
		 exit('0');
	}	
	$dbh = null; //(free)
}
if($_GET['action']=='log')
{
    getUserUpdatelog();
}elseif($_GET['action']=='Soft')
{
    getSoftUpdate();
}elseif($_GET['action']=='upload')
{
   uploadUpdate($_GET['name'],$_GET['path'],$_GET['version'],$_GET['url']);
}elseif($_GET['action']=='edit')
{
	getEditlist();
}elseif($_GET['action']=='new')
{
	newUpdateObject();
}
elseif($_GET['action']=='del')
{
	deleteObject();
}
?>	
