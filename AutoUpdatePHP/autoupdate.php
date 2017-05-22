<?php


if(isset($_GET["name"])&&
   isset($_GET["version"])&&
   isset($_GET["publicKey"])){
    $mname=$_GET["name"];
    $version = $_GET["version"];
    $publicKey = $_GET["publicKey"];
    if($publicKey == md5('595902716'.'@qq.com')){
      $dbh  = new PDO("mysql:host=localhost;dbname=update","root","root", array(PDO::ATTR_PERSISTENT => true));
 
      flushmJson($mname,$version,$dbh);
      $dbh = null; //(释放)
    }
    else{
        echo 'Are you kidding me?';
    }
}
else{
    echo 'You need some salt!';
}

function flushmJson($name,$ver,$dbh){
   if(!addLog($name,$ver,$dbh))
   { 
        $str = array('code'=>'0');
        echo json_encode($str);
        return False;
   }
   $exename = $name;
   $sqlstr =   "SELECT downloadUrl ,version FROM `serverupdate` WHERE version > '".$ver."'  AND `exename` = '".$exename."'"; 
 //  $dbh   = new PDO("mysql:host=localhost;dbname=update","root","root");
   $rs  =  $dbh -> query($sqlstr);
   if($rs->rowCount()){
   while($row = $rs -> fetch()){
             $str = array('url'=>$row[0],'ver'=>intval($row[1]));
           echo json_encode($str);
      }
   }
   else
   {
         $str = array('code'=>'0');
        echo json_encode($str);
        return False;
   }

}

function addLog($json,$ver,$dbh)
{

    $sqlstr    = "INSERT INTO `updatelog` ( `time`, `json`, `ver`) VALUES ( '".date("Y-m-d H:i:s",time()) ."', '".$json."', '".$ver."')";
    //echo $sqlstr;
 
    if($dbh -> exec($sqlstr)){
        return True;
    }
    else
    {
        return False;
    }
  
    }

?>