#!/bin/bash

if [[ $EUID != 0 ]]; then
	sudo $0 $@
	exit
fi

if [[ $1 == "off" ]]; then
        tc qdisc del dev eth0 root
elif [[ $1 == "on" ]]; then
        # delay is 20ms +/- 10ms, loss is 5%
        tc qdisc add dev eth0 root netem delay 20ms 10ms loss 5%
fi

tc qdisc show dev eth0
