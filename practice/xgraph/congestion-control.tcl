set ns [new Simulator]

$ns color 1 Red
$ns color 2 Blue

set nf [open out.nam w]
$ns namtrace-all $nf

proc finish {} {
  global ns nf
  $ns flush-trace 
  close $nf
  exec nam out.nam &
  exec xgraph congestion.xg -geometry 300x300 &
  exit 0    
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

$ns duplex-link $n0 $n1 10Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns duplex-link-op $n0 $n1 orient right
$ns duplex-link-op $n1 $n2 orient right

set tcp [new Agent/TCP]
$tcp set class_ 1
$tcp set window_ 32
$tcp set packetSize_ 1000
$tcp set fid_ 2                                     
$tcp set type_ Tahoe
$ns attach-agent $n0 $tcp

set sink [new Agent/TCPSink]
$ns attach-agent $n2 $sink
$ns connect $tcp $sink

set cbr [new Application/Traffic/CBR]
$cbr set packetSize_ 1000
$cbr set interval_ 0.1000
$cbr attach-agent $tcp

$ns at 0.2 "$cbr start"
$ns at 4.5 "$cbr stop"
$ns at 5.0 finish
$ns run