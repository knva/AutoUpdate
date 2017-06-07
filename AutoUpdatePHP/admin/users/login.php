<?php  
    session_start();
header("Content-Type: text/html; charset=utf-8");

if(isset($_SESSION['expiretime'])) {
    if($_SESSION['expiretime'] < time()) {
        unset($_SESSION['expiretime']);
        header('Location: logout.php?action=logout'); // 登出
        exit(0);
    } else {
        $_SESSION['expiretime'] = time() + 500; // 刷新时间戳
    }
}

    if(isset($_SESSION['username'])){
     //   echo 'success';
        header('location:../index.php');
        exit('success');
    }

    if(isset($_POST['username'])&&isset($_POST['password'])){

//登录  
//if(!isset($_POST['submit'])){  
//    exit('error');  
//}  
$username = htmlspecialchars($_POST['username']);  
$password = htmlspecialchars($_POST['password']);  
//包含数据库连接文件  
require("../../db_config.php");
//检测用户名及密码是否正确  
		$dbh = new PDO("mysql:host=".$db_host.";dbname=".$db_database.";charset=utf8",$db_username,$db_password, array(PDO::ATTR_PERSISTENT => true));
		$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
		$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        $res=$dbh->query("select * from user where username='$username' and passwd='$password' limit 1");
  $dbh=NULL;
if($res->fetchColumn() >0 ){  
    //登录成功  

        $_SESSION['username'] =  $username;  
$_SESSION['expiretime'] = time();
    //header("Location:index.php");    
    echo 'success';  
    exit;  
} else {  
    echo 'error';  
}  

  
       //   echo 'success';
        //header('location:../index.php');
    //    exit('success');
    }


  
?>  


<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->
    <meta name="description" content="">
    <meta name="author" content="">
    <link rel="icon" href="../../favicon.ico">

    <title>登陆到管理控制台</title>

    <!-- Bootstrap core CSS -->
    <link href="https://cdn.bootcss.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">


    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <link href="../../assets/css/ie10-viewport-bug-workaround.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="css/login.css" rel="stylesheet">

    <!-- Just for debugging purposes. Don't actually copy these 2 lines! -->
    <!--[if lt IE 9]><script src="../../assets/js/ie8-responsive-file-warning.js"></script><![endif]-->
    <script src="../../assets/js/ie-emulation-modes-warning.js"></script>

    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.3/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
</head>

<body>

    <div class="container">

        <form class="form-login" method="post" name="lgform" id="lgform">
            <h2 class="form-login-heading">登陆</h2>
            <label for="inputEmail" class="sr-only">邮箱</label>
            <input type="email" id="inputEmail" name="username" class="form-control" placeholder="Email address" required autofocus>
            <label for="inputPassword" class="sr-only">密码</label>
            <input type="password" name="password" id="inputPassword" class="form-control" placeholder="Password" required>
            <div class="checkbox">
                <p>
                    <div id="serverResponse"></div>

                </p>
                <label>
            <input type="checkbox" value="remember-me"> 记住我
          </label>
            </div>
            <button name="submit" id="login" class="btn btn-lg btn-primary btn-block" type="submit">登陆</button>
        </form>

    </div>
    <!-- /container -->
    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://cdn.bootcss.com/jquery/1.12.4/jquery.min.js"></script>
    <script>
        window.jQuery || document.write('<script src="../assets/js/vendor/jquery.min.js"><\/script>')
    </script>
    <script src="https://cdn.bootcss.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
    <!-- Just to make our placeholder images work. Don't actually copy the next line! -->
    <script src="../../assets/js/vendor/holder.min.js"></script>
    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <script src="../../assets/js/ie10-viewport-bug-workaround.js"></script>
    <script src="https://cdn.bootcss.com/blueimp-md5/2.7.0/js/md5.js"></script>
    <script>
        $(document).ready(function() {
            $("#login").click(function() {
                var user = document.getElementById("inputEmail").value;
                var pwd = document.getElementById("inputPassword").value;
                var passmd5 = md5(pwd);
                if (user == "" || pwd == "") {
                    $("#serverResponse").html("用户名与密码不能为空！")

                } else { //重点在这，对比之前写的xmlHttpRequest的代码量可以看到，在这只是调用了一个                  //$.post()函数  
                    $.post("login.php", {
                            username: user,
                            password: passmd5
                        },
                        function(msg) {
                            if (msg == "error") {
                                $("#serverResponse").html("密码错误");
                            } else if (msg == "success") {
                                location.href = '../index.php';
                            }
                        }
                    );


                }
            });
        });
    </script>
</body>

</html>