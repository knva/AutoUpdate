<?php
session_start();  
if(isset($_SESSION['expiretime'])) {
    if($_SESSION['expiretime'] < time()) {
        unset($_SESSION['expiretime']);
        header('Location: users/logout.php?action=logout'); // 登出
        exit(0);
    } else {
        $_SESSION['expiretime'] = time() + 500; // 刷新时间戳
    }
}
//检测是否登录，若没登录则转向登录界面  
if(!isset($_SESSION['username'])){  
    header("Location:users/login.php");  
    exit();  
}

?>

<!DOCTYPE html>
<html lang="zh-CN">

<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- 上述3个meta标签*必须*放在最前面，任何其他内容都*必须*跟随其后！ -->
    <meta name="description" content="">
    <meta name="author" content="">
    <link rel="icon" href="../favicon.ico">

    <title>软件更新管理控制台</title>

    <!-- Bootstrap core CSS -->
    <link href="https://cdn.bootcss.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">

    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <link href="../assets/css/ie10-viewport-bug-workaround.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="users/css/dashboard.css" rel="stylesheet">

    <!-- Just for debugging purposes. Don't actually copy these 2 lines! -->
    <!--[if lt IE 9]><script src="../../assets/js/ie8-responsive-file-warning.js"></script><![endif]-->
    <script src="../assets/js/ie-emulation-modes-warning.js"></script>

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
                    <li class="active"><a href="javascript:;" onclick=" location.href = 'index.php';fulshActive(4);">总览 <span class="sr-only">(current)</span></a></li>
                    <li><a href="javascript:;" onclick="commitUpdate()">提交更新</a></li>
                    <li><a href="javascript:;" onclick="log()">查询更新日志</a></li>
                    <li><a href="javascript:;">导出更新日志</a></li>
                </ul>
                <ul class="nav nav-sidebar">
                    <li><a href="javascript:;" onclick="softlog()" >查看更新记录</a></li>

                </ul>
                <ul class="nav nav-sidebar">
                    <li><a href="users/logout.php?action=logout">退出登录</a></li>

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
                                <th>版本</th>
                                <th>主程序名称</th>
                            </tr>
                        </thead>
                        <tbody id='tb1'>

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
                        <tbody id='tb2'>

                        </tbody>
                    </table>
                </div>
            </div>
            <div class="col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main" id='up'>
            </div>
        </div>
    </div>

    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://cdn.bootcss.com/jquery/1.12.4/jquery.min.js"></script>
    <script>
        window.jQuery || document.write('<script src="../assets/js/vendor/jquery.min.js"><\/script>')
    </script>
    <script src="https://cdn.bootcss.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
    <!-- Just to make our placeholder images work. Don't actually copy the next line! -->
    <script src="../assets/js/vendor/holder.min.js"></script>
    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <script src="../assets/js/ie10-viewport-bug-workaround.js"></script>

    <script>

        $(document).ready(function() {

            $.get(
                "common/action.php",
                "action=Soft",
                function(msg) {
  //console.log(msg.length);
  var mhtml;
  var button = "<td><input id ='editbtn' type='button' value='edit' class='btn  btn-success' onclick=\"edit()\"/></td>\
  <td><input id ='delbtn' type='button' value='delete' class='btn  btn-danger' onclick=\"del()\"/></td>";
  for(var i = 0 ; i <msg.length; i ++){
   mhtml+= "<tr><td>"+msg[i].id+"</td><td>"+msg[i].downloadurl+"</td><td>"+msg[i].time+"</td><td>"+msg[i].version+"</td><td>"+msg[i].exename+"</td>"+button+"</tr>";
 }
                $("#tb1").html(mhtml);
                },"json"
            );
            $.get(
                "common/action.php",
                "action=log",
                function(msg) {
                   var mhtml;
  for(var i = 0 ; i <msg.length; i ++){
   mhtml+= "<tr><td>"+msg[i].id+"</td><td>"+msg[i].time+"</td><td>"+msg[i].json+"</td><td>"+msg[i].version+"</td><td>"+msg[i].ip+"</td></tr>";
  }
    $("#tb2").html(mhtml);
                },"json"
            );
        });

    function log()
    {
      fulshActive(6);
        document.getElementById('log').style.display='block';

        document.getElementById('something').style.display='none';
        document.getElementById('up').style.display='none';
       
    }
    function commitUpdate()
    {
        fulshActive(5);
        document.getElementById('log').style.display='none';
        document.getElementById('something').style.display='none';
            document.getElementById('up').style.display='block';
       
        $.get("update.php","",function(msg)
        {
             $("#up").html(msg);
        });
       
    }
function softlog()
{
    
        fulshActive(8);
                document.getElementById('log').style.display='none';

        document.getElementById('something').style.display='block';
        document.getElementById('up').style.display='none';
}

    function fulshActive(num)
    {
     $('li:gt('+num+')').removeClass("active");
     $('li:lt('+num+')').removeClass("active");
     $('li:eq('+num+')').addClass("active");
    }
    function edit()
    {
        alert('aaa');
    }

    </script>
</body>

</html>