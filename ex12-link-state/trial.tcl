#------------------------------------------------------- 
# This ns script has been created by the nam editor.
# If you edit it manually, the nam editor might not
# be able to open it properly in the future.
#
# EDITING BY HAND IS AT YOUR OWN RISK!
#------------------------------------------------------- 
# Create a new simulator object.
set ns [new Simulator]
# Create a nam trace datafile.
set namfile [open /mnt/d/clg/sem5/networks/lab/ex12-link-state/trial.nam w]
$ns namtrace-all $namfile

# Create wired nodes.
set node(3) [$ns node]
## node(3) at 602.890625,583.125000
$node(3) set X_ 602.890625
$node(3) set Y_ 583.125000
$node(3) set Z_ 0.0
$node(3) color "black"

set node(2) [$ns node]
## node(2) at 535.171875,584.500000
$node(2) set X_ 535.171875
$node(2) set Y_ 584.500000
$node(2) set Z_ 0.0
$node(2) color "black"

set node(1) [$ns node]
## node(1) at 474.328094,581.406250
$node(1) set X_ 474.328094
$node(1) set Y_ 581.406250
$node(1) set Z_ 0.0
$node(1) color "black"


# Create links between nodes.
$ns simplex-link $node(3) $node(2) 1.000000Mb 20.000000ms DropTail
$ns simplex-link-op $node(3) $node(2) queuePos 0.5
$ns simplex-link-op $node(3) $node(2) color black
$ns simplex-link-op $node(3) $node(2) orient 178.8deg
# Set Queue Properties for link 3->2
[[$ns link $node(3) $node(2)] queue] set limit_ 20

$ns simplex-link $node(2) $node(3) 1.000000Mb 20.000000ms DropTail
$ns simplex-link-op $node(2) $node(3) queuePos 0.5
$ns simplex-link-op $node(2) $node(3) color black
$ns simplex-link-op $node(2) $node(3) orient 358.8deg
# Set Queue Properties for link 2->3
[[$ns link $node(2) $node(3)] queue] set limit_ 20

$ns simplex-link $node(2) $node(1) 1.000000Mb 20.000000ms DropTail
$ns simplex-link-op $node(2) $node(1) queuePos 0.5
$ns simplex-link-op $node(2) $node(1) color black
$ns simplex-link-op $node(2) $node(1) orient 182.9deg
# Set Queue Properties for link 2->1
[[$ns link $node(2) $node(1)] queue] set limit_ 20

$ns simplex-link $node(1) $node(2) 1.000000Mb 20.000000ms DropTail
$ns simplex-link-op $node(1) $node(2) queuePos 0.5
$ns simplex-link-op $node(1) $node(2) color black
$ns simplex-link-op $node(1) $node(2) orient 2.9deg
# Set Queue Properties for link 1->2
[[$ns link $node(1) $node(2)] queue] set limit_ 20

# Add Link Loss Models

# Create agents.
set agent(4) [new Agent/Null]
$ns attach-agent $node(3) $agent(4)
set agent(3) [new Agent/Null]
$ns attach-agent $node(2) $agent(3)
set agent(1) [new Agent/UDP]
$ns attach-agent $node(2) $agent(1)

# Create traffic sources and add them to the agent.
set traffic_source(1) [new Application/Traffic/CBR]
$traffic_source(1) attach-agent $agent(1)
$traffic_source(1) set interval_ 0.001950
set agent(2) [new Agent/UDP]
$ns attach-agent $node(1) $agent(2)

# Connect agents.
# Run the simulation
proc finish {} {
	global ns namfile
	$ns flush-trace
	close $namfile
	exec nam -r 2000.000000us /mnt/d/clg/sem5/networks/lab/ex12-link-state/trial.nam &	
	exit 0
	}
$ns at 60.000000 "finish"
$ns run
