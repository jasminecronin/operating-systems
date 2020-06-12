#!/bin/bash

find . -type f -name "*.$1" -printf '%p %s\n' | sort -nrk 2 | head -$2 | awk '{total += $2} {print $1, $2} END {print "Total size: " total}'

