#!/opt/bin/perl

use strict;
use warnings;

# Command-line interface to the Grids client

# Temp library loader while Grids is still in development
use FindBin;
use lib "$FindBin::Bin/../../grids-platform/lib";

use Grids::Address::IPv4;
use Grids::Client;
use Grids::Identity;
use Grids::Console;
use Grids::Conf;

use Carp qw (croak);
use Getopt::Long;
use Data::Dumper;

# To be implemented
#use WordNet::QueryData;
#use WWW::Wikipedia;
use Monolog;
use Phrase;

my $id = Grids::Identity->new();
my $client;
my $room;

my $con = Grids::Console->new(
    title => "Parser",
    prompt => "Parser>",
    handlers => {
        newid => \&create_id,
        connect => \&connect,
        login => \&login,
        listrooms => \&request_list_rooms,
		createroom => \&create_room,
    },
);

# Hashref to each monolog
my $monologs = {};

my $column_x = 250;
my $column_y = -250;

# data structure to keep track of conversation structures

#my $wn = WordNet::QueryData->new(noload => 1);
#my $wiki = WWW::Wikipedia->new();

run();

# Search through the list of monologs, and return the object when found
# If the monolog doesn't exist, create it.
sub find_monolog {
	my $monolog_id = shift;
	
	while ( my ($key, $value) = each( %$monologs) ) {
		if( $key eq $monolog_id ){
			return $value;
		}
	}

	$con->print("MAKING NEW MONOLOG");
	
	# Not found, create an object
	my $new_monolog = Monolog->new();
	$new_monolog->id($monolog_id);
	
	$monologs->{ $monolog_id } = $new_monolog;
	
	return $new_monolog;
}


sub connect {
    #my $address = Grids::Address::IPv4->new( address => 'elcerrito.ath.cx');
    my $address = Grids::Address::IPv4->new( address => '127.0.0.1' );
    $client->connect($address);
}

sub login {
    $client->dispatch_event('Authentication.Login');
}

sub request_list_rooms {
    $client->dispatch_event('Room.List');
}

sub create_room {
	$client->dispatch_event('Room.Create');
}

sub create_id {
    $con->interactively_generate_identity;
}

sub connected_cb {
	my($c, $evt) = @_;
	$con->print("CONNECTED CALL BACK");

	# Don't crash the server
	sleep(1);

	$client->dispatch_event('Room.List');
}

sub broadcast_cb {
	print "BROADCAST CALL BACK\n";
}

sub room_create_cb {
	# Room created, let's get all the rooms on the server just to be sure
	# that my room is where it needs to be.
	request_list_rooms;
}

# Set yourself to the first room
sub list_rooms_cb {
	my ($c, $evt) = @_;

	my $rooms = $evt->args->{rooms};

	$room = $rooms->[0];

	# No rooms on the server
	unless ($room) { 
		create_room;
		return; 
	}

	$con->print("Your room now set to $room");
}

sub create_object_cb {
	my($c, $evt) = @_;

	unless($evt){
		return;
	}

	my $args = $evt->args;

	# Filter out the bounceback confirmation code
	if( $args->{success}){
		return;
	}
	
	# Problem with Grids
	# It seems to be the case that error / success messages get sent back
	# but since I only use broadcast, it's fine
	if( $args->{error}){
		return;
	}

	unless( $evt->args->{attr}->{type} ) {
		return;
	}

	#$con->print("Dump from create_object_cb");
	#$con->print( Dumper($args));

	if( $evt->args->{attr}->{type} eq "InputText"){
		my $monolog = find_monolog( $evt->args->{id} );
		
		if( $evt->args->{attr}->{text} ){
			$monolog->parse_text_input( $evt->args->{attr}->{text} );		
		}
	}
}

sub update_object_cb {
	$con->print("UPDATE OBJECT CALL BACK");

	my($c, $evt) = @_;

	unless($evt){
		return;
	}

	my $args = $evt->args;

	#$con->print("Dump from update_object_cb");
	#$con->print( Dumper($args));

	# Filter out the bounceback confirmation code
	if( $args->{success}){
		return;
	}
	# Problem with Grids
	if( $args->{error}){
		return;
	}

	if( $monologs->{$evt->args->{id}} ) {
		
		my $monolog = $monologs->{ $evt->args->{id} };
		
		if( $evt->args->{attr}->{text} ){
			$monolog->parse_text_input( $evt->args->{attr}->{text} );		
		}
		
		foreach my $phrase ($monolog->phrases) {
			if( $phrase->node_generated == 0){
				create_node_from_phrase($phrase);
			}
		}
	}
}

sub list_objects_cb {
	$con->print("LIST OBJECTS CALL BACK");
}

sub run {
    # main loop condition
    my $main = AnyEvent->condvar;

	$client = Grids::Client->new(id => $id, use_encryption => 0, debug => 1, auto_flush_queue => 1);

	# Refer to Grids::Node::Hooks.. for hooks
	$client->register_hooks(
							'Authentication.Login' => \&connected_cb,
							'Broadcast.Event' => \&broadcast_cb,            
							'Connected' => \&connected_cb,
							'Room.Create' => \&room_create_cb,
							'Room.List' => \&list_rooms_cb,
							'Room.CreateObject' => \&create_object_cb,
							'Room.UpdateObject' => \&update_object_cb,
							'Room.ListObjects' => \&list_objects_cb,
							);

	::connect();

	$con->listen_for_input;

	$main->recv;
}

sub create_node_from_phrase {
	my ($phrase) = @_;

	$phrase->node_generated(1);

	$con->print("Creating node from phrase");
	
	my $pos_x = $column_x;
	my $pos_y = $column_y;
	$column_y += 50;

	# limit to 3 decimals of precision
	$pos_x = sprintf("%.3f", $pos_x);
	$pos_y = sprintf("%.3f", $pos_y);

	my $pos_z = 0;

	my $node_value = { '_broadcast' => 1, pos => [$pos_x,$pos_y,0], rot => [0,0,0], scl => [1,1,1], id => $phrase->id, room_id => $room, attr => { type => 'GenericNode', text => $phrase->text } };

	#$con->print("About to make node:");
	#$con->print( Dumper($node_value));

	# Sleep for 250 ms
	select(undef,undef,undef, 0.5);

	$client->dispatch_event('Room.CreateObject', $node_value);
} 



