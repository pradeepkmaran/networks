set ns [new Simulator]

set tracefile [open out.tr w]
set namfile [open out.nam w]
$ns trace-all $tracefile
$ns namtrace-all $namfile

set n0 [$ns node]
set n1 [$ns node]
$ns duplex-link $n0 $n1 2Mb 10ms DropTail

set tcp [new Agent/TCP]
$tcp set packetSize_ 1000
$tcp set type_ Sink
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n1 $sink
$ns connect $tcp $sink

set ftp [new Application/FTP]
$ftp attach-agent $tcp

$ns at 0.1 "$ftp start"
$ns at 4.0 "$ftp stop"

proc finish {} {
    global ns tracefile namfile
    $ns flush-trace
    close $tracefile
    close $namfile
    exec nam out.nam &
    exit 0
}
$ns at 5.0 "finish"
$ns run