function initready() {

    $.get("common/action.php", "action=Soft",
        function(msg) {
            //console.log(msg.length);
            var mhtml;
            var button = "<td><input id ='editbtn' type='button' value='编辑' class='btn  btn-success' /></td>\
  <td><input id ='delbtn' type='button'  data-toggle='modal' data-target='#mydelModal' value='删除' class='btn  btn-danger' /></td>";
            for (var i = 0; i < msg.length; i++) {
                mhtml += "<tr><td>" + msg[i].id + "</td><td style='width:40%'>" + msg[i].downloadurl + "</td><td>" + msg[i].time + "</td><td>" + msg[i].version + "</td><td>" + msg[i].exename + "</td>" + button + "</tr>";
            }
            $("#tb1").html(mhtml);
        },
        "json");
    $.get("common/action.php", "action=log",
        function(msg) {
            var mhtml;
            for (var i = 0; i < msg.length; i++) {
                mhtml += "<tr><td>" + msg[i].id + "</td><td>" + msg[i].time + "</td><td>" + msg[i].json + "</td><td>" + msg[i].version + "</td><td>" + msg[i].ip + "</td></tr>";
            }
            $("#tb2").html(mhtml);
        },
        "json");
}
$(document).ready(function() {
    initready();
});

function log() {
    fulshActive(6);
    document.getElementById('log').style.display = 'block';

    document.getElementById('something').style.display = 'none';
    document.getElementById('up').style.display = 'none';

}

function commitUpdate() {
    fulshActive(5);
    document.getElementById('log').style.display = 'none';
    document.getElementById('something').style.display = 'none';
    document.getElementById('up').style.display = 'block';

    $.get("update.php", "",
        function(msg) {
            $("#up").html(msg);
        });

}

function softlog() {

    fulshActive(8);
    document.getElementById('log').style.display = 'none';

    document.getElementById('something').style.display = 'block';
    document.getElementById('up').style.display = 'none';
}

function fulshActive(num) {
    $('li:gt(' + num + ')').removeClass("active");
    $('li:lt(' + num + ')').removeClass("active");
    $('li:eq(' + num + ')').addClass("active");
}

$(document).on("click", "input:button#editbtn",
    function() {

        var mparent = $(this).parent().siblings("td");

        // alert("test_0");
        str = $(this).val() == "编辑" ? "确定" : "编辑";

        $(this).val(str); // 按钮被点击后，在“编辑”和“确定”之间切换
        var mlength = $(this).parent().siblings("td").length;

        //console.log(mparent);
        for (var i = 0; i < mlength; i++) {
            switch (i) {
                case mlength - 1:
                    break;
                case 0:
                    break;
                case 1:
                    obj_text = $(mparent[i]).find("input:button"); // 判断单元格下是否有文本框
                    if (!obj_text.length) // 如果没有文本框，则添加文本框使之可以编辑
                        $(mparent[i]).html($(mparent[i]).text() + "<input id='uploadbtn'  type='button' class='btn btn-success' value='修改' />"); //data-toggle='modal' data-target='#myModal'

                    else // 如果已经存在文本框，则将其显示为文本框修改的值
                        $(mparent[i]).html($(mparent[i]).text());
                    break;
                case 2:
                    break;
                case 3:
                    obj_text = $(mparent[i]).find("input:text"); // 判断单元格下是否有文本框
                    if (!obj_text.length) // 如果没有文本框，则添加文本框使之可以编辑
                        $(mparent[i]).html("<input type='text'  disabled='disabled' style='width:30px' value='" + $(mparent[i]).text() + "'>");
                    else // 如果已经存在文本框，则将其显示为文本框修改的值
                        $(mparent[i]).html(obj_text.val());
                    break;
                default:
                    // console.log(mparent[i]);
                    obj_text = $(mparent[i]).find("input:text"); // 判断单元格下是否有文本框
                    if (!obj_text.length) // 如果没有文本框，则添加文本框使之可以编辑
                        $(mparent[i]).html("<input type='text' value='" + $(mparent[i]).text() + "'>");
                    else // 如果已经存在文本框，则将其显示为文本框修改的值
                        $(mparent[i]).html(obj_text.val());
                    break;
            }

        }

        if ($(this).val() == '编辑') {
            var name = $(mparent[4]).html();
            var version = parseInt($(mparent[3]).html()) + 1;
            var url = $(mparent[1]).html();
            $.get("common/action.php", "action=upload&name=" + name + "&version=" + version + "&url=" + url,
                function(msg) {});
            initready();
            return;
        }

    });

$(document).on("click", "input:button#delbtn",
    function() {



    });

$(document).on("click", "input:button#uploadbtn",
    function() {
        $('#myModal').modal({ backdrop: 'static', keyboard: false });
        var mlength = $(this).parent().siblings("td").length;
        var mparent = $(this).parent().siblings("td");
        var id = $(mparent[0]).html();
        //  var editbtn = "<input id=\"fileupload\" class='fileupload' type=\"file\" name=\"files[]\" data-url=\"server/php/\" >\
        //  <label id='uploadfiles'/>";
        $.get("common/action.php?action=edit&id=" + id,
            function(msg) {
                var mhtml;
                var arr = new Array();
                arr = msg[0].downloadurl.split(',');
                //console.log(arr);
                for (var i = 1; i < arr.length + 1; i++) {
                    // var editbtn = "  <div class=\"upload clearfix \"><div class=\"uploadbtnBox clearfix\"><a herf='javascript:;' class='a-upload'><input id='" + parseInt(i - 1) + "' class='fileupload' name=\"files[]\" type=\"file\" onclick='checkFile(this);'/>点击上传</a></div></div>";
                    var edibtn = "<span class=\"btn btn-success fileinput-button\">\
        <i class=\"glyphicon glyphicon-plus\"></i>\
        <span>选择文件...</span>\
        <!-- The file input field used as target for the file upload widget -->\
        <input onclick='checkFile(this);' id=\"" + parseInt(i - 1) + "\" type=\"file\" name=\"files[]\" multiple=\"\">\
    </span>";
                    var delbtn = "<button onclick='delteFile(this)' id ='delBtn" + parseInt(i - 1) + "' class=\"btn btn-danger delete\" data-type=\"DELETE\" data-url=\"server/php/index.php?file=" + arr[i - 1].split('/')[arr[i - 1].split('/').length - 1] + "\">\
                    <i class=\"glyphicon glyphicon-trash\"></i>\
                    <span>删除</span>\
                </button>";
                    var path = "<select>\
  <option value =\"/\">/</option>\
  <option value =\"/dll\">/dll</option>\
</select>";
                    mhtml += "<tr>";
                    //  mhtml += "<tr><td>" +parseInt(i) + "</td><td>" + arr[i] + "</td><td>"+editbtn+"</td>"+ "</td><td>"+delbtn+"</td></tr>";
                    mhtml += "<td>" + parseInt(i) + "</td>";
                    var tmp = arr[i - 1].split('/');
                    var name = tmp[tmp.length - 1];
                    if (name == '') {
                        name = '未上传';
                    }
                    // console.log(tmp);
                    mhtml += "<td id='data" + parseInt(i - 1) + "' value='" + arr[i - 1] + "'>" + name + "</td>";
                    mhtml += "<td>" + path + "</td><td>" + edibtn + "</td><td>" + delbtn + "</td>";
                    mhtml += "</tr>"
                }
                $("#motal-edit").html(mhtml);
                $("#myEditnum").html(id);
            },
            "json");
    });

$(document).on("click", "button#save",

    function() {
        var arr = new Array();
        $("#motal-edit tr td:nth-child(2)").each(function(key, value) {
            arr.push($(this).attr('value'));
        });
        console.log(arr);

        var myid = $("#myEditnum").html();
        //console.log($("#tb1 tr td:nth-child(2)")[myid - 1]);
        var obj_text = $($("#tb1 tr td:nth-child(2)")[myid - 1]).find("input:button");
        if (!obj_text.length) {

        } else {
            var btn = "<input id='uploadbtn' data-toggle='modal' data-target='#myModal' type='button' class='btn btn-success' value='修改' />"
            $($("#tb1 tr td:nth-child(2)")[myid - 1]).html(arr + btn);
        }
    });

function checkFile(tt) {
    var id = $(tt).attr('id');
    console.log(id);
    $('input[type=file]').fileupload({
        url: 'server/php/index.php?softName=update&version=143',
        dataType: 'json',
        done: function(e, data) {
            $.each(data.result.files,
                function(index, file) {
                    $('#data' + id).replaceWith($('<td/>').text(file.name).attr('value', file.url));
                    $('#delBtn' + id).attr('data-url', file.deleteUrl);

                });
        },
        progressall: function(e, data) {
            var progress = parseInt(data.loaded / data.total * 100, 10);
            $('#progress .progress-bar').css('width', progress + '%');
        }
    });
}

function delteFile(tt) {
    //console.log($(tt).attr('data-url'));
    var tmp = $(tt).attr('data-url').split('=');
    var nametmp = tmp[tmp.length - 1];
    var argtmp = nametmp.split('&');
    var pathtmp = argtmp[0].split('/');
    var name = pathtmp[pathtmp.length - 1];
    //console.log(name);
    if (name == 'null' || name == '未上传' || name == '') {
        $(tt).parents('tr').remove();
        return;
    }
    $.ajax({
        url: $(tt).attr('data-url') + "&softName=update&version=143",
        type: 'delete',
        dataType: 'json',
        success: function(result) {
            // Do something with the result

            if (result[decodeURI(name)]) {
                $(tt).parents('tr').remove();
            } else {
                if (confirm('无法删除远程文件,是否进行强制删除?')) {
                    $(tt).parents('tr').remove();
                }
            }
        },

    });
}

function addOne(tt) {
    var t01 = $("#motal-edit tr").length + 1;
    var edibtn = "<span class=\"btn btn-success fileinput-button\">\
        <i class=\"glyphicon glyphicon-plus\"></i>\
        <span>选择文件...</span>\
        <!-- The file input field used as target for the file upload widget -->\
        <input onclick='checkFile(this);' id=\"" + parseInt(t01 - 1) + "\" type=\"file\" name=\"files[]\" multiple=\"\">\
    </span>";
    var delbtn = "<button onclick='delteFile(this)' id ='delBtn" + parseInt(t01 - 1) + "' class=\"btn btn-danger delete\" data-type=\"DELETE\" data-url=\"server/php/index.php?file=" + 'null' + "\">\
                    <i class=\"glyphicon glyphicon-trash\"></i>\
                    <span>删除</span>\
                </button>";

    var path = "<select>\
  <option value =\"/\">/</option>\
  <option value =\"/dll\">/dll</option>\
</select>";
    var mhtml = "<tr>";
    //  mhtml += "<tr><td>" +parseInt(i) + "</td><td>" + arr[i] + "</td><td>"+editbtn+"</td>"+ "</td><td>"+delbtn+"</td></tr>";
    mhtml += "<td>" + parseInt(t01) + "</td><td id='data" + parseInt(t01 - 1) + "' value=''>" + "未上传" + "</td><td>" + path + "</td><td>" + edibtn + "</td><td>" + delbtn + "</td>";
    mhtml += "</tr>"
    $('#motal-edit').append(mhtml); //在table最后面添加一行
}

$(document).on('click', '.mtclosebtn',
    function() {
        if (confirm('确定关闭?')) {
            $('#myModal').modal('hide')
        }
    });