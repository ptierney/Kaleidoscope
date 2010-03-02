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
