##自动更新软件开发计划

问题1:多个文件更新.                                                             over
已解决
问题2:指定文件更新.                                                             over
已解决
问题3:传递到服务器如何判断是什么软件.                                            over
根据json,直接传递软件名称
接收json 判断是否需要更新,从远端下载程序,创建脚本.进行更新操作

问题4:传递的hash 的系统信息如何解析. 也就是给php传递的json 数据.需要包含什么        over
``` json
{
  "result": {
    "status": "ok",
    "ver": "2",
    "list": {
      "url": [
        "http://aize.org",
        "http://aize.org",
        "http://aize.org",
        "http://aize.org"
      ],
      "file": [
        "a.zip",
        "a.zip",
        "a.zip",
        "a.zip"
      ]
    }
  }
}
```

问题5:mysql 数据表设计                                                          over

数据库 update:

表:serverupdate
id  downloadUrl                                             time                version     exename
1	http://127.0.0.1/update.exe,http://127.0.0.1/update.dll	2017-05-22 00:00:00	30	        update.exe



表:updatelog
id   time               json    ver
27	2017-05-22 10:55:48	{aa:aa}	2.23

问题6:POST GET判断                                                              over
在php端添加了 post 与get 的判断

问题7:更新成功返回值


问题8:更新成功后,本地json刷新问题

问题9:主要程序中如何使用本更新程序.
(不推荐)思路1:
将本程序制作为一个lib库,方便其它程序调用,以达到更新的目的.

(不推荐)思路2:
制作一个lib库,主要用来下载本更新程序,其它程序调用.以达到更新的目的

(推荐)思路3:
本程序随被更新程序一同打包.
程序给出接口. arg[1] 软件名称 arg[2] 软件版本 
当被更新程序执行时,调用本接口. 可以达到更新的目的.