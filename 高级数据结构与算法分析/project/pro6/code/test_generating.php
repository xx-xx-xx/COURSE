<?php
$file = fopen("test.txt","w+");
$set_array = array();
$size = array(100,200,300,400,500,600,700,800,900,1000,1500,2000,2500,3000,3500,4000,4500,5000,
6000,7000,8000,9000,10000,12000,14000,16000,18000,20000,30000,40000,50000,60000,70000,80000,90000,100000);
for ($fill = 0; $fill < 100000; $fill ++) {
    $random = mt_rand(1,99999);
    array_push($set_array,$random);

}
fwrite($file,"int Test_set[100000] = {") ;
for ($at = 0; $at < 100000; $at ++){
    if ($at!=99999)$string = $set_array[$at].",";
    else $string = $set_array[$at];
    fwrite($file,$string); 
    if ($at > 0 && $at%50 == 0) {
        fwrite($file,"\n");
    }
    flush();
}
fwrite($file,"};");
fclose($file);
echo "done.";
?>