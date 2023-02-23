use strict;
use warnings;

use List::Priority;
use List::AllUtils;

my @dirs = ([0, 0], [-1, 0], [1, 0], [0, -1], [0, 1]);
my @board = map {chomp; [split //]} <>;
my $start = [0, List::AllUtils::firstidx {$_ eq '.'} $board[0]->@*];
my $end = [$#board, List::AllUtils::firstidx {$_ eq '.'} $board[-1]->@*];
my $ymax = scalar(@board) - 2;
my $xmax = scalar($board[0]->@*) - 2;

push(@board, [('#') x ($xmax + 2)]);

my %posgroups;
my @vblizz;
my @hblizz;

for my $y (1 .. $ymax) {
  for my $x (1 .. $xmax) {
    push($posgroups{$board[$y][$x]}->@*, [$y, $x]);
  }
}

for (my $time = 0; $time < $ymax; $time++) {
  for my $up ($posgroups{'^'}->@*) {
    my $y = ($up->[0] - $time - 1) % $ymax + 1;
    $vblizz[$time]{$y, $up->[1]} = 1;
  }

  for my $down ($posgroups{'v'}->@*) {
    my $y = ($down->[0] + $time - 1) % $ymax + 1;
    $vblizz[$time]{$y, $down->[1]} = 1;
  }
}

for (my $time = 0; $time < $xmax; $time++) {
  for my $left ($posgroups{'<'}->@*) {
    my $x = ($left->[1] - $time - 1) % $xmax + 1;
    $hblizz[$time]{$left->[0], $x} = 1;
  }
  
  for my $right ($posgroups{'>'}->@*) {
    my $x = ($right->[1] + $time - 1) % $xmax + 1;
    $hblizz[$time]{$right->[0], $x} = 1;
  }
}

sub add {
  my ($x, $y) = @_;
  return [$x->[0] + $y->[0], $x->[1] + $y->[1]];
}

sub search {
  my ($start, $end) = @_;
  my $frontier = new List::Priority;
  my %visited;
  $frontier->insert(0, [0, $start]);

  SEARCH:
  while (my ($time, $pos) = ($frontier->shift)->@*) {
    if ($pos->[0] == $end->[0] && $pos->[1] == $end->[1]) {
      return $time;
    }

    next SEARCH if ($visited{$time, $pos->[0], $pos->[1]}++);

    DIRECTION:
    for my $dir (@dirs) {
      my $neigh = add($pos, $dir);

      next DIRECTION if (exists $vblizz[($time + 1) % $ymax]{$neigh->[0], $neigh->[1]}
                      || exists $hblizz[($time + 1) % $xmax]{$neigh->[0], $neigh->[1]}
                      || $board[$neigh->[0]][$neigh->[1]] eq '#');

      $frontier->insert($time + 1, [$time + 1, $neigh]);
    }
  }
}

my $result = &search($start, $end);
print "$result\n";
