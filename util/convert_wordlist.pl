#!/usr/bin/perl -w

while (<>) {
	if (/^#/) {
		next;
	}

	# filter 
	s/\{\S+\}\s*//g;
	s/\[\S+\]\s*//g;

	print;
}
