package Monolog;

# My first POOP :)

use strict;
use warnings;

use Data::Dumper;

use Phrase;

sub new {
	my $class = shift;
	my $self = {};

	$self->{ID} = undef;
	$self->{PHRASES} = [];
	$self->{POSITION} = [0.0, 0.0, 0.0];
	$self->{NODES_CREATED} = 0;
	$self->{CIRCLE_RADIUS} = 250.0;
	$self->{NUM_PER_CIRCLE} = 12;
	# pitch is how much it increasess per outward revolution
	$self->{PITCH} = 100;
	$self->{LAST_NODE_ID};

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

sub position {
	my $self = shift;
	if (@_) { @{$self->{POSITION} } = @_ }
	return @{ $self->{POSITION} };
}

sub x {
	my $self = shift;
	if(@_) { ${ $self->{POSITION}}[0] = @_ }
	return ${ $self->{POSITION}}[0];
}

sub y {
	my $self = shift;
	if(@_) { ${ $self->{POSITION}}[1] = @_ }
	return ${ $self->{POSITION}}[1];
}

sub z {
	my $self = shift;
	if(@_) { ${ $self->{POSITION}}[2] = @_ }
	return ${ $self->{POSITION}}[2];
}

sub nodes_created {
	my $self = shift;
	if(@_) { $self->{NODES_CREATED} = shift }
	return $self->{NODES_CREATED};
}

sub circle_radius {
	my $self = shift;
	if(@_) { $self->{CIRCLE_RADIUS} = shift }
	return $self->{CIRCLE_RADIUS};
}

sub num_per_circle {
	my $self = shift;
	if(@_) { $self->{NUM_PER_CIRCLE} = shift }
	return $self->{NUM_PER_CIRCLE};
}

sub pitch {
	my $self = shift;
	if(@_) { $self->{PITCH} = shift }
	return $self->{PITCH};
}

sub last_node_id {
	my $self = shift;
	if(@_) { $self->{LAST_NODE_ID} = shift }
	return $self->{LAST_NODE_ID};
}



# Looks through phrases and finds an array 
sub find_phrase {
	my ($self, $text) = @_;
	
	foreach my $phrase ( @{$self->{PHRASES}} ){
		if( $phrase->text eq $text){
			return $phrase;
		}
	}
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

	my $counter = 0;

	#my @phrase_cache = $self->phrases;
	#my $num_phrases = @phrase_cache;

	my $num_phrases = @{$self->{PHRASES}};

	foreach my $text_seg (@input_split){
		if($counter >= $num_phrases){
			last;
		}

		# Make sure to access the text INSIDE of the Phrase object
		if( $text_seg eq ${$self->{PHRASES}}[$counter]->text ){
			$counter++;
			next;
		}
		last;
	}

	for( ; $counter < @input_split; $counter++){
		print STDERR "MAKING NEW PHRASE\n";

		my $temp_phrase = Phrase->new();
		$temp_phrase->text( $input_split[$counter] );
		
		push( @{$self->{PHRASES}}, $temp_phrase);
		#$self->phrases( $self->phrases, $temp_phrase);
	}
}


1;
