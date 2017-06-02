<?php
function getSoftUpdate() {
	require ("../../db_config.php");
	$dbh = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_database . ";charset=utf8", $db_username, $db_password, array(PDO::ATTR_PERSISTENT => true));
	$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
	$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sqlstr = "select * from serverupdate";
	$res = $dbh->query($sqlstr);
	foreach ($res as $row) {
		echo "<tr>" . "<td>" . $row['Id'] . "</td>" . "<td>" . $row['downloadUrl'] . "</td>" . "<td>" . $row['time'] . "</td>" . "<td>" . $row['version'] . "</td>" . "<td>" . $row['exename'] . "</td>" . "</tr>";
	}
	$dbh = null; //(free)
	
}
function getUserUpdatelog() {
	require ("../../db_config.php");
	$dbh = new PDO("mysql:host=" . $db_host . ";dbname=" . $db_database . ";charset=utf8", $db_username, $db_password, array(PDO::ATTR_PERSISTENT => true));
	$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
	$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sqlstr = "select * from updatelog";
	$res = $dbh->query($sqlstr);
	foreach ($res as $row) {
		echo "<tr>" . "<td>" . $row['Id'] . "</td>" . "<td>" . $row['time'] . "</td>" . "<td>" . $row['json'] . "</td>" . "<td>" . $row['ver'] . "</td>" . "<td>" . $row['ip'] . "</td>" . "</tr>";
	}
	$dbh = null; //(free)
	
}

if($_GET['action']=='log')
{
    getUserUpdatelog();
}elseif($_GET['action']=='Soft')
{
    getSoftUpdate();
}
?>	
