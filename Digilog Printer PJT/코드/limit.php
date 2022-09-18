<?php
 
include_once "/lib/sd_spc.php";
 
spc_reset();
spc_sync_baud(460800);
 
$sid = 1;
 
spc_request_dev($sid, "set vref stop 2");
spc_request_dev($sid, "set vref drive 8");
spc_request_dev($sid, "set mode full");
 
spc_request_dev($sid, "set pos 0");
 
spc_request_dev($sid, "goto -sw0 200");
 
while((int)spc_request_dev($sid, "get state"))
    usleep(1);
 
$pos = (int)spc_request_dev($sid, "get pos");
echo "Limit position is: $pos\r\n";
 
?>
