#!/usr/bin/perl

$start = time;
qx'1.exe';
$finish = time;
print "Row by row addition\n";
print "Running time: ",$finish-$start," sec\n"; 
$start = time;
qx'2.exe';
$finish = time;
print "Column by column addition\n";
print "Running time: ",$finish-$start," sec\n"; 