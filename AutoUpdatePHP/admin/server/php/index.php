<?php
/*
 * jQuery File Upload Plugin PHP Example
 * https://github.com/blueimp/jQuery-File-Upload
 *
 * Copyright 2010, Sebastian Tschan
 * https://blueimp.net
 *
 * Licensed under the MIT license:
 * https://opensource.org/licenses/MIT
 */
session_start();  
if(isset($_SESSION['expiretime'])) {
    if($_SESSION['expiretime'] < time()) {
        unset($_SESSION['expiretime']);
        header('Location: ../../users/logout.php?action=logout'); // 登出
        exit(0);
    } else {
        $_SESSION['expiretime'] = time() + 500; // 刷新时间戳
    }
}
//检测是否登录，若没登录则转向登录界面  
if(!isset($_SESSION['username'])){  
    header("Location:../../users/login.php");  
    exit();  
}

error_reporting(E_ALL | E_STRICT);
require('UploadHandler.php');
$upload_handler = new UploadHandler();
