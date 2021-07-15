#!/bin/sh
if test $2
then outsw=1
else outsw=0
fi
if test $3
then maxc=$3
else maxc=0
fi
if test $4 
then ilooka=$4
else ilooka=0
fi
if test $5 
then nstop=$5
else nstop=1
fi
echo "$ilooka $outsw $nstop $maxc" >GRAPE_tcin
cat >GRAPE_tcfein
$GRAPE_BIN/tcfrontend3 <GRAPE_tcfein
cat GRAPE_tcfeout >>GRAPE_tcin
$GRAPE_BIN/tcmaingap3 
if test $1
then cat GRAPE_tcfein GRAPE_tcout >>$1; echo ' ' >>$1
fi
cat GRAPE_tcout
if test $2 
then mv GRAPE_tcpermgrp $2
fi
rm GRAPE_tcfein GRAPE_tcfeout GRAPE_tcin GRAPE_tcout
