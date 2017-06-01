<!DOCTYPE html>
<html lang="zh-CN">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- 上述3个meta标签*必须*放在最前面，任何其他内容都*必须*跟随其后！ -->
    <meta name="description" content="">
    <meta name="author" content="">
    <link rel="icon" href="../../favicon.ico">

    <title>软件更新管理控制台</title>

    <!-- Bootstrap core CSS -->
    <link href="https://cdn.bootcss.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">

    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <link href="../../assets/css/ie10-viewport-bug-workaround.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="dashboard.css" rel="stylesheet">

    <!-- Just for debugging purposes. Don't actually copy these 2 lines! -->
    <!--[if lt IE 9]><script src="../../assets/js/ie8-responsive-file-warning.js"></script><![endif]-->
    <script src="../../assets/js/ie-emulation-modes-warning.js"></script>

    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
      <script src="https://cdn.bootcss.com/html5shiv/3.7.3/html5shiv.min.js"></script>
      <script src="https://cdn.bootcss.com/respond.js/1.4.2/respond.min.js"></script>
    <![endif]-->
  </head>

  <body>

    <nav class="navbar navbar-inverse navbar-fixed-top">
      <div class="container-fluid">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
            <span class="sr-only">软件更新管理控制台</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="#">软件更新管理控制台</a>
        </div>
        <div id="navbar" class="navbar-collapse collapse">
          <ul class="nav navbar-nav navbar-right">
            <li><a href="#">总览 </a></li>
            <li><a href="#">设置</a></li>
            <li><a href="#">配置</a></li>
            <li><a href="#">帮助</a></li>
          </ul>
          <form class="navbar-form navbar-right">
            <input type="text" class="form-control" placeholder="搜索...">
          </form>
        </div>
      </div>
    </nav>

    <div class="container-fluid">
      <div class="row">
        <div class="col-sm-3 col-md-2 sidebar">
          <ul class="nav nav-sidebar">
            <li class="active"><a href="#" onclick="document.getElementById('log').style.display='none';document.getElementById('something').style.display='block';">总览 <span class="sr-only">(current)</span></a></li>
            <li><a href="#">提交更新</a></li>
            <li><a href="#"onclick="document.getElementById('log').style.display='block';document.getElementById('something').style.display='none';">查询更新日志</a></li>
            <li><a href="#">导出更新日志</a></li>
          </ul>
          <ul class="nav nav-sidebar">
            <li><a href="">查看更新记录</a></li>

          </ul>
          <ul class="nav nav-sidebar">
            <li><a href="">退出登录</a></li>

          </ul>
        </div>
        <div class="col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main" id='something'>
                  <h2 class="sub-header">软件更新列表</h2>
          <div class="table-responsive">
            <table class="table table-striped" id="up date">
              <thead>
                <tr>
                  <th>id</th>
                  <th>下载路径</th>
                  <th>更新时间</th>
                  <th>版本号</th>
                  <th>主程序名称</th>
                </tr>
              </thead>
              <tbody>
<?php
require("../db_config.php");

		$dbh = new PDO("mysql:host=".$db_host.";dbname=".$db_database.";charset=utf8",$db_username,$db_password, array(PDO::ATTR_PERSISTENT => true));
		$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
		$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		
        $sqlstr = "select * from serverupdate";
$res=$dbh->query($sqlstr);
foreach($res as $row){  
    echo    "<tr>".
                  "<td>".$row['Id']."</td>".
                  "<td>".$row['downloadUrl']."</td>".
                  "<td>".$row['time']."</td>".
                  "<td>".$row['version']."</td>".
                  "<td>".$row['exename']."</td>".
                "</tr>";
}

		$dbh = null; //(free)
		
?>


              </tbody>
            </table>
          </div>
        </div>


             <div id='log' class="col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main" style="display:none">
                  <h2 class="sub-header">软件更新日志</h2>
          <div class="table-responsive">
            <table class="table table-striped" id="up date">
              <thead>
                <tr>
                  <th>id</th>
                  <th>时间</th>
                  <th>更新程序</th>
                  <th>版本号</th>
                  <th>ip地址</th>
                </tr>
              </thead>
              <tbody>
<?php
require("../db_config.php");

		$dbh = new PDO("mysql:host=".$db_host.";dbname=".$db_database.";charset=utf8",$db_username,$db_password, array(PDO::ATTR_PERSISTENT => true));
		$dbh->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);
		$dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		
        $sqlstr = "select * from updatelog";
$res=$dbh->query($sqlstr);
foreach($res as $row){  
    echo    "<tr>".
                  "<td>".$row['Id']."</td>".
                  "<td>".$row['time']."</td>".
                  "<td>".$row['json']."</td>".
                  "<td>".$row['ver']."</td>".
                  "<td>".$row['ip']."</td>".
                "</tr>";
}

		$dbh = null; //(free)
		
?>


              </tbody>
            </table>
          </div>
        </div>

      </div>
    </div>

    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://cdn.bootcss.com/jquery/1.12.4/jquery.min.js"></script>
    <script>window.jQuery || document.write('<script src="../../assets/js/vendor/jquery.min.js"><\/script>')</script>
    <script src="https://cdn.bootcss.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
    <!-- Just to make our placeholder images work. Don't actually copy the next line! -->
    <script src="../../assets/js/vendor/holder.min.js"></script>
    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <script src="../../assets/js/ie10-viewport-bug-workaround.js"></script>
  </body>
</html>
