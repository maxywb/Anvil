#!/bin/bash

ANVIL_HOME=~/Dropbox/src/AnViL


anvilStart=`date +%s.%N`
for i in {1..1000}
do
    $ANVIL_HOME/anvil simpleLoop.avl
done
anvilEnd=`date +%s.%N`
echo ANVIL: $(calc $anvilEnd - $anvilStart)

gcc0Start=`date +%s.%N`
for i in {1..1000}
do
    ./simple_0
done
gcc0End=`date +%s.%N`
echo gcc0: $(calc $gcc0End - $gcc0Start)


gcc1Start=`date +%s.%N`
for i in {1..1000}
do
    ./simple_1
done
gcc1End=`date +%s.%N`
echo gcc1: $(calc $gcc1End - $gcc1Start)

gcc2Start=`date +%s.%N`
for i in {1..1000}
do
    ./simple_2
done
gcc2End=`date +%s.%N`
echo gcc2: $(calc $gcc2End - $gcc2Start)

gcc3Start=`date +%s.%N`
for i in {1..1000}
do
    ./simple_3
done
gcc3End=`date +%s.%N`
echo gcc3: $(calc $gcc3End - $gcc3Start)


