#!/usr/bin/perl
 
use strict;
use warnings;
 
my @matrix;
 
$matrix[0]=[-99     ,1      ,2      ,-1     ,4      ,1      ,4      ,2      ,-2     ,-99];
$matrix[1]=[-2      ,4      ,1      ,0      ,-2     ,-1     ,0      ,1      ,2      ,-99];
$matrix[2]=[1       ,2      ,-1     ,1      ,-99    ,4      ,2      ,-1     ,1      ,-99];
$matrix[3]=[0       ,2      ,0      ,-2     ,2      ,1      ,-99    ,2      ,-1     ,-99];
$matrix[4]=[-2      ,1      ,4      ,1      ,0      ,-1     ,4      ,1      ,-2     ,-99];
$matrix[5]=[-1      ,-99    ,1      ,-1     ,0      ,1      ,2      ,-2     ,0      ,-99];
$matrix[6]=[2       ,4      ,-2     ,1      ,0      ,2      ,-1     ,-99    ,2      ,-99];
$matrix[7]=[1       ,0      ,2      ,4      ,-1     ,-2     ,1      ,0      ,1      ,-99];
$matrix[8]=[0       ,-2     ,-1     ,1      ,-99    ,1      ,2      ,-2     ,4      ,-99];
$matrix[9]=[-99     ,-99    ,-99    -99     ,-99    ,-99    ,-99    ,-99    ,-99    ,-99];
 
 
my @colour;
 
my @dx    = ( -1,  1,  2,  2,  1, -1, -2, -2);
my @dy    = ( -2, -2, -1,  1,  2,  2,  1, -1);
 
 
my @hx1   = ( -1,  1,  1,  1,  1, -1, -1, -1);
my @hx2   = (  0,  0,  1,  1,  0,  0, -1, -1);
my @hy2   = ( -1, -1,  0,  0,  1,  1,  0,  0);
 
my @vy1   = ( -1, -1, -1,  1,  1,  1,  1, -1);
my @vx2   = (  0,  0,  1,  1,  0,  0, -1, -1);
my @vy2   = ( -1, -1,  0,  0,  1,  1,  0,  0);
 
 
my $x=4;
my $y=4;
my $prize = 0;
my $path = "";
my @maxPaths = {};
my @maxGolds = {};
my $depth = 0;
my $maxPrize = 0;
my $gold = 0;
 
 
my @cost;
my @possible;
$possible[0]=[0,0,0,0,0,0,0,0,0];
$possible[1]=[0,0,0,0,0,0,0,0,0];
$possible[2]=[0,0,0,0,0,0,0,0,0];
$possible[3]=[0,0,0,0,0,0,0,0,0];
$possible[4]=[0,0,0,0,0,0,0,0,0];
$possible[5]=[0,0,0,0,0,0,0,0,0];
$possible[6]=[0,0,0,0,0,0,0,0,0];
$possible[7]=[0,0,0,0,0,0,0,0,0];
$possible[8]=[0,0,0,0,0,0,0,0,0];
 
 
sub CALCULATE_PRIZE {
    my $x;
    my $y;
    my $dir;
    for ($x = 0; $x < 9; $x++ ) {
        foreach ($y = 0; $y < 9; $y++ ) {
            foreach ($dir = 0; $dir < 8; $dir++ ) {
                my $dx = $x + $dx[$dir];
                my $dy = $y + $dy[$dir];
                if ( $dx >= 0 && $dy >= 0 && $dx <=8 && $dy <=8 && $matrix[$dy][$dx] != -99 ) {
                    my $flag = 0;
                    my $x1 = $x + $hx1[$dir];
                    my $y1 = $y;
                    my $x2 = $x1 + $hx2[$dir];
                    my $y2 = $y1 + $hy2[$dir];
                    my $cost = -99;
                    if ($matrix[$y1][$x1] != -99 && $matrix[$y2][$x2] != -99) {
                        $cost = $matrix[$dy][$dx] + $matrix[$y1][$x1] + $matrix[$y2][$x2];
                        $flag = 1;
                    }
                    $x1 = $x;
                    $y1 = $y + $vy1[$dir];
                    $x2 = $x1 + $vx2[$dir];
                    $y2 = $y1 + $vy2[$dir];
                    if ($matrix[$y1][$x1] != -99 && $matrix[$y2][$x2] != -99) {
                        $cost = ($cost,$matrix[$dy][$dx] + $matrix[$y1][$x1] + $matrix[$y2][$x2])[$cost < $matrix[$dy][$dx] + $matrix[$y1][$x1] + $matrix[$y2][$x2]];
                        $flag = 1;
                    }
                    $cost = -99 if ($flag == 0);
                    $possible[$y][$x]++ if ($flag);
 
                    $cost[$y][$x][$dir] = $cost;
                } else {
                    $cost[$y][$x][$dir] = -99;
                }
#               print "$x,$y + $dir = $cost[$y][$x][$dir]\n";
            }
        }
    }
}
 
 
 
sub DISPLAY {
    my $line;
    my @display = @_;
    for ($x = 0; $x < 9; $x++ ) {
        foreach ($y = 0; $y < 9; $y++ ) {
            print "$display[$y][$x] ";
        }
        print "\n";
    }
}
 
sub DECOLOUR {
    $colour[0]=[1,0,0,0,0,0,0,0,0];
    $colour[1]=[0,0,0,0,0,0,0,0,0];
    $colour[2]=[0,0,0,0,1,0,0,0,0];
    $colour[3]=[0,0,0,0,0,0,1,0,0];
    $colour[4]=[0,0,0,0,0,0,0,0,0];
    $colour[5]=[0,1,0,0,0,0,0,0,0];
    $colour[6]=[0,0,0,0,0,0,0,1,0];
    $colour[7]=[0,0,0,0,0,0,0,0,0];
    $colour[8]=[0,0,0,0,1,0,0,0,0];
    $prize = 0;
    $depth = -1;
    $path = "";
    $gold = 0;
    $x=4;
    $y=4;
}
 
sub MINIMUM {
#   my $listOfPossAdj="";
    my $x = $_[0];
    my $y = $_[1];
    my $dir;
#   my $min = -99;
#   my $minDir = -1;
    my @possDir = ();
    my $flag = 1;
#   my $final = -1;
    my $limit = 4;
    my $count = 0;
#   while ($count < 4) {
    for ( $dir = 0; $dir < 8; $dir++) {
        my $futx = $x + $dx[$dir];
        my $futy = $y + $dy[$dir];
        if ($cost[$y][$x][$dir] != -99  && $colour[$futy][$futx] == 0 && $possible[$futy][$futx] > 0) {
            push @possDir,($dir);
#           $flag = 0;
            #$count ++;
#           if ($min < ($cost[$y][$x][$dir] * 1.0 / $possible[$futy][$futx]) ) {
#               if ($futx == 4 && $futy == 4 ) {
#                   $final = $dir;
#               } else {
#                   $minDir = $dir;
#               }
#               $min = ($cost[$y][$x][$dir] * 1.0 / $possible[$futy][$futx]);
#           }
        }
    }
#   $limit++;
#   }
     
    my $max = -99;
    my $i = 0;
    my $j = 0;
#   print "$_," foreach (@possDir);
#   print " - Before \n";
    for ($j = 0;$j <=$#possDir;$j++) {
        my $iMax = $j;
        my $maxDir = $possDir[$j];
        my $maxCost = ($cost[$y][$x][$maxDir] * 1.0 / $possible[$y+$dy[$maxDir]][$x+$dx[$maxDir]]);
#       my $maxCost = $possible[$y+$dy[$maxDir]][$x+$dx[$maxDir]];
        for ( $i = $j+1; $i < $#possDir;$i++) {
            my $currDir = $possDir[$i];
            my $currCost = $cost[$y][$x][$currDir] * 1.0 / $possible[$y+$dy[$currDir]][$x+$dx[$currDir]];
#           my $currCost = $possible[$y+$dy[$currDir]][$x+$dx[$currDir]];
            #CHECK FOR 4,4
            if ( $maxCost < $currCost ) {
                $iMax = $i;
                $maxDir = $currDir;
            }
        }
        if ($iMax != $j) {
            my $t = $possDir[$iMax];
            $possDir[$iMax] = $possDir[$j];
            $possDir[$j] = $t;
        }
    }
#   print "$_," foreach (@possDir);
#   print "\n";
#   $minDir = $final if ($minDir == -1);
#   print "min for $x,$y = $minDir and $cost[$y][$x][$minDir]\n";
#   $possible[$y][$x]--;    
    if ($depth < 4 && $#possDir > 3) {
    #   print "start\n";
        return ($possDir[0],$possDir[1],$possDir[2],$possDir[3]);
    } elsif ($depth < 7 && $#possDir > 2) {
    #   print "start\n";
        return ($possDir[0],$possDir[1],$possDir[2]);
    } elsif ($depth < 13 && $#possDir > 1) {
#       print "middle\n";
        return ($possDir[0],$possDir[1]);
    } elsif ($depth > 2 && $#possDir > 0) {
#       print "end\n";
        return ($possDir[0]);
    } else {
        return @possDir;
    }
}
 
         
my $maxPrize = 30; 
 
sub PATH {
    my $x = $_[0];
    my $y = $_[1];
    my $tdir;
    for ( $tdir = 0; $tdir < 8; $tdir++) {
        my $futx = $x + $dx[$tdir];
        my $futy = $y + $dy[$tdir];
        if ( $futy < 9 && $futx < 9 && $futx >= 0 && $futx >= 0) {
            $possible[($y+$dy[$tdir])][($x+$dx[$tdir])]-- unless ($possible[($y+$dy[$tdir])][($x+$dx[$tdir])] == 0);
        }
    }
    my @possDir = MINIMUM($x,$y);
    my $dir;
    foreach $dir (@possDir) {
    my $futx = $x + $dx[$dir];
    my $futy = $y + $dy[$dir];
    if ($futx == 4 && $futy == 4 && $cost[$y][$x][$dir] != -99) {
        my $potPrize = $prize +  $cost[$y][$x][$dir];
        $maxPrize = $potPrize if($potPrize > $maxPrize);
        #print "$path -> $x,$y going to $futx,$futy\n";
        my @len = split ',',$path;
        if ($prize >= $maxPrize) {
            $maxPrize = $prize;
            print "$prize with $#len -> $path$dir \n";
        }
        die if ($depth > 240);
    } elsif ($cost[$y][$x][$dir] != -99 && $colour[$futy][$futx] == 0) {
        $path .= "$dir,";
        $prize += $cost[$y][$x][$dir];
        $colour[$futy][$futx] = 1;
        $depth++;
#       print "$depth - $path\n";
        #DISPLAY(@possible);
        #print "--------------------\n";
        #DISPLAY(@colour);
        PATH($futx,$futy);
#       print "$x,$y\n";
        $depth--;
        for ( $tdir = 0; $tdir < 8; $tdir++) {
            $possible[($futy+$dy[$tdir])][($futx+$dx[$tdir])]++;
        }
        my @len = split ',',$path;
        pop @len;
        $path = join ',',@len;
        $path .= ',';
        $colour[$futy][$futx] = 0;
#       print "$x,$y,$dir\n";
        $prize -= $cost[$y][$x][$dir];
    }
    }
}
DECOLOUR();
CALCULATE_PRIZE();
DISPLAY(@possible);
PATH(4,4);
