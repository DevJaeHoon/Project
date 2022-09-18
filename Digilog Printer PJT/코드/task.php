<?php

include_once "/lib/sd_340.php";
include_once "/lib/sd_spc.php";
include_once "/lib/sn_tcp_ws.php";

define("PWM_PERIOD", 20000); // (20ms)
define("ANGLE_UP", 500);
define("ANGLE_DOWN", 1500);

define("SID_X",			2);
define("SID_Y",			1);

define("MAX_X", 		1900);
define("MAX_Y", 		1650);

define("CMD_PEN_UP",	0);
define("CMD_PEN_DOWN",	1);
define("CMD_MOVE",		2);

define("SPEED",		20);
define("SPEED_MAX",		2000);
define("ACCEL_X",		1500);
define("ACCEL_Y",		1000);
define("ACCEL_MAX",		50000);

function spc_check_did($sid, $did)
{
	$resp = spc_request_csv($sid, 0, "get did");

	if($resp === false)
	{
		echo "spc_check_did: sid$sid - device not found\r\n";
		return false;
	}

	if($resp[1] != "40002405")
	{
		echo "spc_check_did: unknown device ", $resp[2], "\r\n";
		return false;
	}

	return true;
}

function pen_up()
{
	$angle = ANGLE_UP;
	ht_pwm_width(2, $angle, PWM_PERIOD);
}

function pen_down()
{
	$angle = ANGLE_DOWN;
	ht_pwm_width(2, $angle, PWM_PERIOD);
}

function xy_goto($x, $y)
{
	global $mode;
	
	$r_x = (int)(-($x + $y)/sqrt(2));
	$r_y = (int)(-($x - $y)/sqrt(2));
	
	echo "r_xy is ", $r_x, ", ", $r_y, "\r\n";

	$x0 = (int)spc_request_dev(SID_X, "get pos") / $mode;
	$y0 = (int)spc_request_dev(SID_Y, "get pos") / $mode;
	
//	echo "goto xy is ", $x0, ", ", $y0, "\r\n";

	$delta_x = $r_x - $x0; 
	$delta_y = $r_y - $y0;

	if($delta_x == 0 && $delta_y == 0)
		return;
	
	$speed_x = SPEED * abs($delta_x);
	$speed_y = SPEED * abs($delta_y);
	
	$r_x *= $mode; 
	$r_y *= $mode;
	$speed_x *= $mode;
	$speed_y *= $mode;
	
	if($speed_x > SPEED_MAX * $mode)
		$speed_x = SPEED_MAX * $mode;
	if($speed_y > SPEED_MAX * $mode)
		$speed_y = SPEED_MAX * $mode;
	
	$accel_x = ACCEL_X * $speed_x;
	$accel_y = ACCEL_Y * $speed_y;
	
	if($accel_x > ACCEL_MAX * $mode)
		$accel_x = ACCEL_MAX * $mode;
	if($accel_y > ACCEL_MAX * $mode)
		$accel_y = ACCEL_MAX * $mode;

	if($delta_x == 0)
		spc_request_dev(SID_Y, "goto $r_y $speed_y $accel_y");
	else if($delta_y == 0)
		spc_request_dev(SID_X, "goto $r_x $speed_x $accel_x");
	else
	{
		spc_request_dev(SID_X, "goto $r_x $speed_x $accel_x");
		spc_request_dev(SID_Y, "goto $r_y $speed_y $accel_y");
	}
	
	
}

function xy_wait()
{
	$x_state = (int)spc_request_dev(SID_X, "get state");
	$y_state = (int)spc_request_dev(SID_Y, "get state");

	if($x_state == 0 && $y_state == 0)
		usleep(1000);
}

function xy_init()
{
	global $mode;
	
	$angle = ANGLE_UP;
	ht_pwm_setup(2, $angle, PWM_PERIOD);
	pen_up();
	
	spc_reset();
	spc_sync_baud(115200);

	if(!spc_check_did(SID_X, "40002405"))
		return;
	if(!spc_check_did(SID_Y, "40002405"))
		return;

	spc_request_dev(SID_X, "set vref stop 4");
	spc_request_dev(SID_X, "set vref drive 15");
	spc_request_dev(SID_X, "set mode $mode");
	spc_request_dev(SID_X, "set rsnc 120 250");
	
	spc_request_dev(SID_Y, "set vref stop 4");
	spc_request_dev(SID_Y, "set vref drive 15");
	spc_request_dev(SID_Y, "set mode $mode");
	spc_request_dev(SID_Y, "set rsnc 120 250");

	spc_request_dev(SID_X, "reset");
	spc_request_dev(SID_Y, "reset");
	xy_goto(0, 0);
	xy_wait();
}

function send_position($x, $y, $pen_state)
{
	global $mode;
	
//	$x1= (int)spc_request_dev(SID_X, "get pos") / $mode;
//	$y1 = (int)spc_request_dev(SID_Y, "get pos") / $mode;
	
//	echo "send xy is ", $x1, ", ", $y1, "\r\n";
	
//	$x = (int)(-($x1 + $y1)/sqrt(2));
//	$y = (int)(($x1 - $y1)/sqrt(2));
	
//	$pos_x = -(int)(($x+$y)/sqrt(2));
//	$pos_y = (int)(($x-$y)/sqrt(2));
	
//	echo "pos xy is ", $pos_x, ", ", $pos_y, "\r\n";

	$wbuf = "[$x, $y, $pen_state]";
	ws_write(0, $wbuf);
}

$mode = 32;

ws_setup(0, "xy_plotter", "csv.phpoc");
xy_init();

$rbuf = "";
$pen_state = CMD_PEN_UP;

while(1)
{
	if(ws_state(0) == TCP_CONNECTED)
	{
		$rlen = ws_read_line(0, $rbuf);
		
		if($rlen)
		{
			$data = explode(" ", $rbuf);
			$cmd = (int)$data[0];
      
			switch($cmd)
			{
				case CMD_PEN_DOWN:
					$x = (int)$data[1];
					$y = (int)$data[2];
					xy_goto($x, $y);
					xy_wait();
					
					echo "xy is ", $x, ", ", $y, "\r\n";
					
					pen_down();
					$pen_state = CMD_PEN_DOWN;
					
					send_position($x, $y, $pen_state);
					
					break;
				case CMD_PEN_UP:
					pen_up();
					$pen_state = CMD_PEN_UP;
					
					send_position($x, $y, $pen_state);
					
					break;
				case CMD_MOVE:
					$x = (int)$data[1];
					$y = (int)$data[2];
					xy_goto($x, $y);
					xy_wait();
					
					send_position($x, $y, $pen_state);
					
					break;
			}
		}
	}
	
/*	$cur_x = (int)spc_request_dev(SID_X, "get pos") / $mode;
	$cur_y = (int)spc_request_dev(SID_Y, "get pos") / $mode;
	
	if(abs($pre_x - $cur_x) > 10 || abs($pre_y - $cur_y) > 10)
	{
		$pre_x = $cur_x;
		$pre_y = $cur_y;
		send_position($pen_state);
	}*/
}

?>