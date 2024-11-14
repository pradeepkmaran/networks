BEGIN {
 stime = 0
 ftime = 0
 flag = 0
 fsize = 0
 throughput = 0
 latency = 0
}
{
 if ($1 == "r" && $4 == 2) { # Check for received packets with flow ID 4
 fsize += $6 # Accumulate the size of received packets
 if (flag == 0) { # Set the start time on the first packet received
 stime = $2
 flag = 1
 }
 ftime = $2 # Update the finish time to the latest packet received
 }
}
END {
 latency = ftime - stime
if (latency > 0) {
    throughput = (fsize * 8) / latency
    printf("\nLatency: %f seconds", latency)
    printf("\nThroughput: %f Mbps\n", throughput / 1000000)
} else {
    printf("\nError: Invalid latency. Check start and finish times.\n")
}

}
