<?php  //注销登录  
   session_start();
if($_GET['action'] == "logout"){  
   session_unset();//free all session variable
            session_destroy();//销毁一个会话中的全部数据
            setcookie(session_name(),'',time()-3600);//销毁与客户端的卡号
   header("Location:login.php");    
  
    exit;  
}  
?>