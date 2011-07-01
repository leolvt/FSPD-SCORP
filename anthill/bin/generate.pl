#!/usr/bin/perl

$a = $ARGV[0];
$b = $ARGV[1];
$n = $ARGV[2];

print "$a\n";

while ($n--) {
    print "".int(rand($b))."\n";
}
