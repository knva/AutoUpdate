<?php  
    session_start();
header("Content-Type: text/html; charset=utf-8");

    if(isset($_SESSION['username'])){
           header('location:../index.php');
        exit('您已经登入了，请不要重新登入');
    }


//登录  
//if(!isset($_POST['submit'])){  
//    exit('error');  
//}  
$username = htmlspecialchars($_POST['username']);  
$password = MD5($_POST['password']);  
//包含数据库连接文件  
require("../../db_config.php");
//检测用户名及密码是否正确  
		$dbh = new PDO("mysql:host=".$db_host.";dbname=".$db_database.";charset=utf8",$db_username,$db_password, array(PDO::ATTR_PERSISTENT => true));
		$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
		$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        $res=$dbh->query("select * from user where username='$username' and passwd='$password' limit 1");

if($res->fetchColumn() >0 ){  
    //登录成功  

        $_SESSION['username'] =  $username;  

    //header("Location:index.php");    
    echo 'success';  
    exit;  
} else {  
    echo 'error';  
}  
  $dbh=NULL;
  
  

  
?>  