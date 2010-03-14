
use strict;
use warnings;

use FindBin;
use lib "$FindBin::Bin/../../grids-platform/lib";

use Grids::Address::IPv4;
use Grids::Client;
use Grids::Identity;
use Grids::Console;
use Grids::Conf;

use Data::Dumper;

my $server_address = '127.0.0.1';
my $num_messages = 300;
# This is the pause between each message
my $millis_pause = 0.01;

my $is_sender;
my $messages_received = 0;

my $sender_id = Grids::Identity->new();
my $received_id = Grids::Identity->new();
my $id;
my $client;
my $room;

my $con;

run();

# This test tests whether all connected
# clients receive broadcast messages if the volume
# of messages is high
#
# It starts two client processes, one sends a large volume
# of data to the server, and the other one checks to make sure
# it received all the messages.

sub connect {
    #my $address = Grids::Address::IPv4->new( address => 'elcerrito.ath.cx');
    my $address = Grids::Address::IPv4->new( address => $server_address );
    $client->connect($address);
}

sub login_cb {
	print "LOGIN_CB";
}

sub request_list_rooms {
    $client->dispatch_event('Room.List');
}

sub create_room {
	$client->dispatch_event('Room.Create');
}

sub connected_cb {
	my($c, $evt) = @_;

	# Don't crash the server
	sleep(1);

	request_list_rooms;
}

# Set yourself to the first room
sub list_rooms_cb {
	my ($c, $evt) = @_;

	my $rooms = $evt->args->{rooms};

	$room = $rooms->[0];

	# No rooms on the server
	unless (defined($room)) { 
		create_room;
		return; 
	}
	
	# You are now in a room,
	# if sender, sleep, then send a bunch of messages
	return if( $is_sender == 0);

	# Sleep for a bit to make sure the other client is connected
	# to the node.
	sleep(6);
	
	$con->print( "About to create nodes");
	
	for(my $i = 0; $i < $num_messages; $i++){
		send_test_text($i);
		select(undef, undef, undef, $millis_pause);
	}

	exit();
	#$con->print("Your room now set to $room");
}

sub room_create_cb {
	request_list_rooms;
}

sub create_object_cb {
	my($c, $evt) = @_;

	$con->print( "CREATE ROOM CB");

	return unless($evt);

	return if($is_sender == 1);

	my $args = $evt->args;

	# Filter out the bounceback confirmation code
	return if( $args->{success});
	
	# Problem with Grids
	# It seems to be the case that error / success messages get sent back
	# but since I only use broadcast, it's fine
	return if( $args->{error});

	# Discard other messages
	return unless( $evt->args->{attr}->{type} );
	return unless( $evt->args->{attr}->{owner} );

	#$con->print( Dumper( $sender_id));
	#$con->print( Dumper( $evt->args->{attr}));	

	if( $evt->args->{attr}->{type} eq "GenericNode"){
#		if( $evt->args->{attr}->{owner} eq $sender_id->{name}){
			$messages_received++;
			$con->print( $messages_received);
			if($messages_received == $num_messages){
				$con->print("Passed");
				exit();
			}
#		}
	}
}

sub send_test_text {
	my( $text_num) = @_;

	my $test_node_id = Grids::UUID->new_id();
	
	my $node_text = "Node Num $text_num";

	my $node_value = { '_broadcast' => 1, pos => [0,0,0], rot => [0,0,0], scl => [1,1,1], id => $test_node_id, room_id => $room, attr => { type => 'GenericNode', text => $node_text, owner => $id->{name} } };

	#$con->print( Dumper($node_value));

	$client->dispatch_event('Room.CreateObject', $node_value);
}

sub run {
	my $pid = fork();
	
	unless( defined($pid) ) {
		print "Oh snap!";
		# Return fail
	} elsif( $pid == 0) {
		# This is the child
		$is_sender = 0;
		$id = $received_id;
		sleep 3;
	} else {
		# This is the parent
		$is_sender = 1;
		$id = $sender_id;
	}

	$con = Grids::Console->new(
							   title => "Parser",
							   prompt => "Parser>",
							   );

	# main loop condition
	my $main = AnyEvent->condvar;

	$client = Grids::Client->new(id => $id, use_encryption => 0, debug => 1, auto_flush_queue => 1);

	# Refer to Grids::Node::Hooks.. for hooks
	$client->register_hooks(
							'Authentication.Login' => \&login_cb,
							'Broadcast.Event' => \&broadcast_cb,            
							'Connected' => \&connected_cb,
							'Room.Create' => \&room_create_cb,
							'Room.List' => \&list_rooms_cb,
							'Room.CreateObject' => \&create_object_cb,
							#'Room.UpdateObject' => \&update_object_cb,
							#'Room.ListObjects' => \&list_objects_cb,
							);

	::connect();

	$con->listen_for_input;

	$main->recv;
}
