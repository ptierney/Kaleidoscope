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

run();

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

	#$con->print("About to dump");
	#$con->print( Dumper($args));

	if( $evt->args->{attr}->{type} eq "InputText"){
		$con->print("CREATED INPUT TEXT!!!");

		my $pos_x = 500 - rand(1000);
		my $pos_y = 500 - rand(1000);

		# limit to 3 decimals of precision
		$pos_x = sprintf("%.3f", $pos_x);
		$pos_y = sprintf("%.3f", $pos_y);

		my $pos_z = 0;

		$pos_x += 0.0;
		$pos_y += 0.0;
		$pos_z += 0.0;
				
		my $node_value = { '_broadcast' => 1, pos => [$pos_x,$pos_y,0], rot => [0,0,0], scl => [1,1,1], id => Grids::UUID->new_id, room_id => $room, attr => { type => 'GenericNode', text => 'Lol Dongs' } };

		#$con->print("About to make node:");
		#$con->print( Dumper($node_value));

		sleep(1);

		$client->dispatch_event('Room.CreateObject', $node_value);
	}
}

sub update_object_cb {
	$con->print("UPDATE OBJECT CALL BACK");
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
