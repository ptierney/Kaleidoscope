#!/usr/bin/perl
use strict;
use warnings;

# Command-line interface to the Grids client

# Temp library loader while Grids is still in development
use FindBin;
use lib "$FindBin::Bin/../../grids-platform/lib";

use Grids::Client;
use Grids::Identity;
use Grids::Console;
use Grids::Conf;

use Carp qw (croak);
use Getopt::Long;
use Data::Dumper;

use WordNet::QueryData;
use WWW::Wikipedia;

# Set up a wordnet and have it load the db
my $wn = WordNet::QueryData->new(noload => 1);
my $wiki = WWW::Wikipedia->new();


my $id = Grids::Identity->new();
my $client = Grids::Client->new(id => $id, use_encryption => 0, debug => 1);
#my $address = Grids::Address::TCP->new( address => 'elcerrito.ath.cx');
my $address = Grids::Address::TCP->new( address => '127.0.0.1' );

# Refer to Grids::Node::Hooks.. for hooks
$client->register_hooks(
						'Authentication.Login' => \&connected_cb,
						'Broadcast.Event' => \&broadcast_cb,
						'Room.Create' => \&room_create_cb,
						'Room.List' => \&list_rooms_cb,
						'Room.CreateObject' => \&create_object_cb,
						'Room.UpdateObject' => \&update_object_cb,
						'Room.ListObjects' => \&list_objects_cb,
						qr/.*/ => \&anything_cb,
						'Connected' => \&anything_cb,
						'ProtocolEstablished' => \&anything_cb,
						);

my $con = Grids::Console->new(
							  title => "Parser",
							  prompt => "Parser>",
							  handlers => {
								  newid => \&create_id,
								  connect => \&connect,
								  login => \&login,
								  listrooms => \&request_list_rooms,
							  },
							  );

# Stores the UUID of the room you send create objects in
my $room;
my %all_words;
my $RANDOM_SPREAD = 10.0;

$client->register_hook(qr/.*/ => \&anything_cb);
$client->register_hook(qr/.*/, sub { $con->print("LOL HI"); });

run();


sub anything_cb {
	$con->print("Anything");
}

sub connect {
	$client->connect($address);
}

sub login {
	$client->dispatch_event('Authentication.Login');
}

sub request_list_rooms {
	$client->dispatch_event('Room.List');
}

sub create_id {
    $con->interactively_generate_identity;
}

sub parse_create_object {
	my ($c, $evt) = @_;

}

sub parse_update_object {
	my ($c, $evt) = @_;

	# Get position, create pos vec
}

sub create_generic_node {
	my ($node_text, $parent_pos) = @_;

	# send request to the server
}

sub parse_from_event {
	my($evt) = @_;
	
	my $args = $evt->args;
	
	# generate position vector
	# extract text
	#parse_input_text($text, %pos);
}

sub parse_input_text {
	my ($text, $position) = @_;

	# split based on white space
	my @words = split(/\s/, $text);

	foreach my $word (@words) {
		my @query_return = $wn->querySense($word);
		
		foreach my $def (@query_return) {
			# if the word is a noun
			if( $def =~ m/\#n/ {
				# check to see if its in the hash
				if( $all_words{$word} ){
					next;
				}
				# make a node
				# Maybe query Wikipedia
				# or use the built in WordNet functions
				my $ran_pos = make_vec( rand($RANDOM_SPREAD/2.0) + $RANDOM_SPREAD,
										rand($RANDOM_SPREAD/2.0) + $RANDOM_SPREAD,
										rand($RANDOM_SPREAD/2.0) + $RANDOM_SPREAD );

				create_generic_node($word, vec_add($position, $ran_pos);
				$all_words{$word} = 1;
			}
		}
	}
}

sub run {
	$con->run;
}

sub connected_cb {
	$con->print("CONNECTED CALL BACK");
}

sub broadcast_cb {
	print "BROADCAST CALL BACK\n";
}

sub room_create_cb {
	print "ROOM CREATE CALL BACK\n";
}

sub list_rooms_cb {
	$con->print("LIST ROOMS CALL BACK");
}

sub create_object_cb {

}

sub update_object_cb {

}

sub list_objects_cb {

}

sub print_vec {
	my ($in_vec) = @_;

	my $x = $in_vec->{'x'};
	my $y = $in_vec->{'y'};
	my $z = $in_vec->{'z'};

	print "<$x, $y, $z>\n";
}

sub make_vec {
	my($x, $y, $z) = @_;

	my $vec;
	$vec->{ 'x' } = $x;
	$vec->{ 'y' } = $y;
	$vec->{ 'z' } = $z;

	return $vec;
}

sub vec_add {
	my($vec1, $vec2) = @_;
	
	my $vec;
	$vec->{ 'x' } = ($vec1->{'x'} + $vec2->{'x'});
	$vec->{ 'y' } = ($vec1->{'y'} + $vec2->{'y'});
	$vec->{ 'z' } = ($vec1->{'z'} + $vec2->{'z'});

	return $vec;
}

sub login_hook {
	my($c, $evt) = @_;

	my $args = $evt->args;
#login successful	
	if($args->{success} == 1) {
		print "LOGIN!!!\n";
	}
}

