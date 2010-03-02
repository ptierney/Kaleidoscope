package Monolog;

# My first POOP :)

use strict;
use warnings;

use Phrase;

sub new {
	my $class = shift;
	my $self = {};

	$self->{ID} = undef;
	$self->{PHRASES} = [];

	bless $self, $class;
	return $self;
}

sub id {
	my $self = shift;
	if(@_) { $self->{ID} = shift }
	return $self->{ID};
}

sub phrases {
	my $self = shift;
	if (@_) { @{$self->{PHRASES} } = @_ }
	return  @{ $self->{PHRASES} };
}

# Finds if the phrase is 
sub find_phrase {
	my ($self, $text) = @_;
	
	foreach my $phrase ($self->{PHRASES}){
		if( $phrase->text eq $text){
			return $phrase;
		}
	}
	
	# Phase not found, generate new phrase
	my $temp_phrase = Phrase->new();
	$temp_phrase->text($text);
	
	return $temp_phrase;
}

# This function takes a newline separated input string
# and generates phrases from it, making sure not
# to create duplicate phrases.
sub parse_text_input {
	my ($self, $text) = @_;

	my $last_valid = 0;
	
	if( chop($text) eq "\n"){
		$last_valid = 1;
	}

	my @input_split = split(/\n/, $text);

	# Delete the last element if it's not valid
	if( $last_valid == 0){
		pop(@input_split);
	}
	
	foreach my $text_seg (@input_split){
		$self->find_phrase( $text_seg);		
	}	
}


1;