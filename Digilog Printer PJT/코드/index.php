<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Ending Credit's XY Plotter</title>
<meta name="viewport" content="width=device-width, initial-scale=0.7">
<style>
body {
	text-align: center;
	background-color: #CCEEFF;
}
#cvs_frame {
	margin-right: auto;
	margin-left: auto; 
	position: relative;
}
.canvas {
	position: absolute; 
	left: 0px;
	top: 0px;
	overflow-y: auto;
	overflow-x: hidden;
	-webkit-overflow-scrolling: touch;
}
#layer_1 {
	z-index: 1;
	background-color: #FFFFFF;
}
</style>

<script>
var CMD_PEN_UP = 0;
var CMD_PEN_DOWN = 1;
var CMD_MOVE = 2;
var RESOLUTION = 20; // 속도 조절

var MAX_X = 1900, MAX_Y = 1650;

var ws = null;
var layer_1 = null
var cvs_frame = null;
var ctx1 = null

var canvas_width = 0, canvas_height = 0;
var x = 0, y = 0;
var touch_x = 0; touch_y = 0;
var cvs_pos_x = 0, cvs_pos_y = 0;
var pre_cvs_pos_x = 0, pre_cvs_pos_y = 0;
var pre_pen_state = CMD_PEN_UP;

function init()
{
	cvs_frame = document.getElementById("cvs_frame");
	
	layer_1 = document.getElementById("layer_1");
	
	layer_1.addEventListener("touchstart", mouse_down);
	layer_1.addEventListener("touchend", mouse_up);
	layer_1.addEventListener("touchmove", mouse_move);
	layer_1.addEventListener("mousedown", mouse_down);
	layer_1.addEventListener("mouseup", mouse_up);
	layer_1.addEventListener("mousemove", mouse_move);
	
	ctx1 = layer_1.getContext("2d");
	ctx1.translate(0, 0);
	
	canvas_resize();
}
function ws_onopen()
{
	document.getElementById("ws_state").innerHTML = "OPEN";
	document.getElementById("wc_conn").innerHTML = "Disconnect";
}
function ws_onclose()
{
	document.getElementById("ws_state").innerHTML = "CLOSED";
	document.getElementById("wc_conn").innerHTML = "Connect";
	ws.onopen = null;
	ws.onclose = null;
	ws.onmessage = null;
	ws = null;
}
function wc_onclick()
{
	if(ws == null)
	{
		ws = new WebSocket("ws://<?echo _SERVER("HTTP_HOST")?>/xy_plotter", "csv.phpoc");
		document.getElementById("ws_state").innerHTML = "CONNECTING";
		ws.onopen = ws_onopen;
		ws.onclose = ws_onclose;
		ws.onmessage = ws_onmessage;  
	}
	else
		ws.close();
}
function ws_onmessage(e_msg)
{
	var arr = JSON.parse(e_msg.data);
	var pos_x			= arr[0];
	var pos_y			= arr[1];
	var cur_pen_state	= arr[2];
	
	pre_cvs_pos_x = cvs_pos_x;
	pre_cvs_pos_y = cvs_pos_y;
	
	cvs_pos_x = Math.round(pos_x * canvas_width / MAX_X);
	cvs_pos_y = Math.round(pos_y * canvas_height / MAX_Y);
	
	if(pre_pen_state == CMD_PEN_UP && cur_pen_state == CMD_PEN_DOWN)
		ctx1.beginPath();
	
	if(cur_pen_state == CMD_PEN_DOWN)
	{
		ctx1.lineTo(cvs_pos_x, cvs_pos_y);
		ctx1.stroke();
	}
	
	pre_pen_state = cur_pen_state;
}
function event_handler(event, type)
{
	var pre_x = x, pre_y = y;
	if(event.targetTouches)
	{
		var targetTouches = event.targetTouches;
		
		touch_x = targetTouches[0].pageX - cvs_frame.offsetLeft;
		touch_y = targetTouches[0].pageY - cvs_frame.offsetTop;
	}
	else
	{
		touch_x = event.offsetX;
		touch_y = event.offsetY;
	}
	
	var temp_x = Math.round(touch_x / canvas_width * MAX_X);
	var temp_y = Math.round((touch_y) / canvas_height * MAX_Y); 
	
	if(type == "MOVE")
	{
		var delta_x = temp_x - pre_x;
		var delta_y = temp_y - pre_y;
		var dist = Math.sqrt( Math.pow(delta_x, 2) + Math.pow(delta_y, 2) );
		
		if(dist < RESOLUTION)
			return false;
	}

	x = temp_x;
	y = temp_y;
	
	return true;
}
function canvas_resize()
{
	var width = Math.round(window.innerWidth*0.95);
	var height = Math.round(window.innerHeight*0.95) - 100;
	
	var temp_height = Math.round(width*MAX_Y/MAX_X);
	if(temp_height <= height)
	{
		canvas_width = width;
		canvas_height = temp_height;
	}
	else
	{
		canvas_width = height*MAX_X/MAX_Y;
		canvas_height = height;
	}
	
	cvs_frame.style.width = canvas_width + "px";
	cvs_frame.style.height = canvas_height + "px";
	
	layer_1.width = canvas_width;
	layer_1.height = canvas_height;
	ctx1.translate(0, 0);
	ctx1.lineWidth = 3;
	ctx1.strokeStyle = "black";
}
function canvas_clear()
{
	ctx1.clearRect(0, 0, canvas_width, canvas_height);
}
function mouse_down()
{
	if(event.targetTouches)
	{
		event.preventDefault();
		if(event.targetTouches.length > 1)
			return;
	}
	
	event_handler(event, "DOWN");
	
	if(ws == null || ws.readyState != 1)
		return;
	ws.send(CMD_PEN_DOWN + " " + x + " " + y + "\r\n"); 
}
function mouse_up()
{
	if(event.targetTouches)
	{
		event.preventDefault();
	}
	
	if(ws == null || ws.readyState != 1)
		return;
			
	ws.send(CMD_PEN_UP + "\r\n"); 
}
function mouse_move()
{
	if(event.targetTouches)
	{
		event.preventDefault();
		if(event.targetTouches.length > 1)
			return;
	}
	
	if(!event_handler(event, "MOVE"))
		return;
	
	if(ws == null || ws.readyState != 1)
		return;
	
	ws.send(CMD_MOVE + " " + x + " " + y + "\r\n"); 
}
window.onload = init;
</script>
</head>

<body onresize="canvas_resize()">
<br>
<div id="cvs_frame">
	<canvas id="layer_1" class="canvas"></canvas>
</div>
<p>WebSocket : <span id="ws_state">null</span></p>
<button id="wc_conn" type="button" onclick="wc_onclick();">Connect</button>
<button type="button" onclick="canvas_clear();">Clear</button>
<hr>
<h1>< 엔딩크레딧 ></h1>
<p>B688057 정재훈<p>B688044 이성원<p>B688059 조영관<p>B684050 황지환</p>
</body>
</html>