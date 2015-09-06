#!/usr/bin/perl

use strict;
use warnings;

use Getopt::Long;

my $folder = "/usr/bin";
my $pbf = "planet-latest.osm.pbf";
my $prefix = "geodata";

my $getopt = GetOptions(
	"folder=s" => \$folder,
	"pbf=s"    => \$pbf,
	"prefix=s" => \$prefix
);

print("folder = $folder\npbf = $pbf\nprefix = $prefix\n");

system("$folder/geo-base-convert $pbf >$prefix.raw.pbf") == 0 or die;
system("cat $prefix.raw.pbf | $folder/geo-base-generate $prefix.raw.dat") == 0 or die;
system("cat $prefix.raw.pbf | $folder/geo-base-sift $prefix.raw.dat >$prefix.pbf") == 0 or die;
system("cat $prefix.pbf | $folder/geo-base-generate $prefix.dat") == 0 or die;
