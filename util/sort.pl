#!/usr/bin/perl

print sort { length($b) - length($a) } <> ;

