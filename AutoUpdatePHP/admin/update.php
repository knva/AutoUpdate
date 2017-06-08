<html>
 <head>
  <link rel="stylesheet" type="text/css" href="upcontrol/webuploader.css" /> 
 </head>
 <body>
  <h1>文件上传</h1> 
  <form class="form-inline" > 
   <div class="form-group"> 
    <label for="softname">软件名称</label> 
    <input type="text" class="form-control" id="softname1" placeholder="softname" /> 
   </div> 
   <div class="form-group"> 
    <label for="version">版本号</label> 
    <input type="text" class="form-control" id="version1" placeholder="version" /> 
   </div> 
   <div class ="form-group" id='msgBox'></div>
   <input type='button'id="commit" class="btn btn-default" value="提交"></input> 
  </form> 
  <div id="uploader" class="wu-example"> 
   <div id="thelist" class="uploader-list"></div> 
     <span>
   <div class="btns"> 
     <div id="picker">
      选择文件
     </div>
   </div> 
   <button id="ctlBtn" class="btn btn-default up-btn" style="">开始上传</button></span>
  </div>  
  <script src="https://cdn.bootcss.com/jquery/1.12.4/jquery.min.js"></script> 
  <!-- Include all compiled plugins (below), or include individual files as needed --> 
  <script src="https://cdn.bootcss.com/bootstrap/3.3.7/js/bootstrap.min.js" integrity="sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa" crossorigin="anonymous"></script> 
  <script type="text/javascript" src="upcontrol/webuploader.js"></script> 
  <script>
  var url = new Array();
// 文件上传
jQuery(function() {
    var $ = jQuery,
        $list = $('#thelist'),
        $btn = $('#ctlBtn'),
        state = 'pending',
        uploader;

    uploader = WebUploader.create({

        // 不压缩image
        resize: false,

        // swf文件路径
        swf:  'upcontrol/Uploader.swf',

        // 文件接收服务端。
        server: 'http://localhost/admin/upcontrol/userupload/fileupload2.php',

        // 选择文件的按钮。可选。
        // 内部根据当前运行是创建，可能是input元素，也可能是flash.
        pick: '#picker'
    });

    // 当有文件添加进来的时候
    uploader.on( 'fileQueued', function( file ) {
        $list.append( '<div id="' + file.id + '" class="item">' +
            '<h4 class="info">' + file.name + '</h4>' +
            '<p class="state">等待上传...</p>' +
        '</div>' );
    });

    // 文件上传过程中创建进度条实时显示。
    uploader.on( 'uploadProgress', function( file, percentage ) {
        var $li = $( '#'+file.id ),
            $percent = $li.find('.progress .progress-bar');

        // 避免重复创建
        if ( !$percent.length ) {
            $percent = $('<div class="progress progress-striped active">' +
              '<div class="progress-bar" role="progressbar" style="width: 0%">' +
              '</div>' +
            '</div>').appendTo( $li ).find('.progress-bar');
        }

        $li.find('p.state').text('上传中');

        $percent.css( 'width', percentage * 100 + '%' );
    });

    uploader.on( 'uploadSuccess', function( file ) {
        url.push("http://127.0.0.1/upcontrol/userupload/uploads/"+file.name)
        $( '#'+file.id ).find('p.state').text('已上传');
        
    });

    uploader.on( 'uploadError', function( file ) {
        $( '#'+file.id ).find('p.state').text('上传出错');
    });

    uploader.on( 'uploadComplete', function( file ) {
        $( '#'+file.id ).find('.progress').fadeOut();
    });

    uploader.on( 'all', function( type ) {
        if ( type === 'startUpload' ) {
            state = 'uploading';
        } else if ( type === 'stopUpload' ) {
            state = 'paused';
        } else if ( type === 'uploadFinished' ) {
            state = 'done';
        }

        if ( state === 'uploading' ) {
            $btn.text('暂停上传');
        } else {
            $btn.text('开始上传');
        }
    });

    $btn.on( 'click', function() {
        if ( state === 'uploading' ) {
            uploader.stop();
        } else {
            uploader.upload();
        }
    });
});

$("#commit").click(function() {
    var name = document.getElementById("softname1").value;
    var version = document.getElementById("version1").value;
    var murl = url.toString();
    console.log(murl);
    if (name == "" || version == "" || url == "") {
        $("#msgBox").html("软件名称与版本号不能为空.版本号会根据数据库内的版本号自动增加")

    } else {
        $.get("common/action.php?action=upLoad", {
            name: name,
            version: version,
            url: murl
        },
        function(msg) {
            if (msg == "error") {
                $("#msgBox").html("错误");
            } else {
                $("#msgBox").html("成功");
            }
        });

    }
});

</script> 
 </body>
</html>

