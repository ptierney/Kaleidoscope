package Phrase;

use Grids::UUID;

use strict;
use warnings;

sub new {
	my $class = shift;
	my $self = {};

	$self->{ID} = Grids::UUID->new_id;
	$self->{TEXT} = undef;
	$self->{NODE_GENERATED} = 0;
	
	bless $self, $class;
	return $self;
}

sub id {
	my $self = shift;
	if(@_) { $self->{ID} = shift }
	return $self->{ID};
}

sub text {
	my $self = shift;
	if(@_) { $self->{TEXT} = shift }
	return $self->{TEXT};
}

sub node_generated {
	my $self = shift;
	if(@_) { $self->{NODE_GENERATED} = shift }
	return $self->{NODE_GENERATED};
}


1;
