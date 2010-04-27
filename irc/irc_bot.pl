
use strict;
use warnings;

use FindBin;
use lib "$FindBin::Bin/../../grids-platform/lib";
use lib "$FindBin::Bin/../../crypt-otr/lib";
use lib "$FindBin::Bin/../../crypt-otr/blib/lib";
use lib "$FindBin::Bin/../../crypt-otr/blib/lib/auto";
use lib "$FindBin::Bin/../../crypt-otr/blib/arch/auto";

use Grids::Address::IPv4;
use Grids::Client;
use Grids::Identity;
use Grids::Console;
use Grids::Conf;

use AnyEvent::IRC;
use AnyEvent::IRC::Client;

use Data::Dumper;

my $server_address = '127.0.0.1';
#my $server_address = 'elcerrito.ath.cx';
my $num_messages = 3;
my $message_number = 2;
# This is the pause between each message
my $millis_pause = 0.01;
my $bot_color = [rand()*10000 % 255,
                 rand()*10000 % 255,
                 rand()*10000 % 255 ];

my $is_sender;
my $messages_received = 0;

my $sender_id = Grids::Identity->new();
my $received_id = Grids::Identity->new();
my $id = Grids::Identity->new();;
my $chat_id = Grids::UUID->new_id();
my $client;
my $room;
my $parent_id = Grids::UUID->new_id();
my $last_irc_id;

my $con;

my $bot_nick = "WaltherG";
my $c = AnyEvent->condvar;
my $irc_client = new AnyEvent::IRC::Client;
my $irc_server = "irc.blessed.net";
my $channel = "#stocks";
#my $irc_server = "mmmii.net";
#my $channel = "#chattest";


my $bot_name = $channel;

$irc_client->reg_cb( publicmsg => \&public_irc_message );

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

  select(undef, undef, undef, 0.1);

	$irc_client->send_srv("JOIN", $channel);

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

  my $parent_id = Grids::UUID->new_id();
  my $pos_x = 500 - int(rand(1000));
  my $pos_y = 500 - int(rand(1000));

	my $node_value = { '_broadcast' => 1, 
                     pos => [$pos_x, $pos_y, 0], 
                     rot => [0,0,0], 
                     scl => [1,1,1], 
                     id => $parent_id, 
                     room_id => $room, 
                     attr => { type => 'Tete', 
                               chat => $chat_id, 
                               text => "Parent", user_name => $bot_name,
                               owner_color => $bot_color,
                               parent => Grids::UUID->new_id(), 
                               owner => $id->{name} } };
	$client->dispatch_event('Room.CreateObject', $node_value);
  select(undef, undef, undef, $millis_pause);
	
	send_test_text($parent_id, $num_messages);

	exit();
	#$con->print("Your room now set to $room");
}

sub room_create_cb {
	request_list_rooms;
}

sub create_object_cb {

}


sub public_irc_message {
	my ($self, $channel, $message) = @_;

	my $pub_text = $message->{params}[1]; 

  my $new_id = Grids::UUID->new_id();

  my @user_split = split(/\!/, $message->{prefix});
  my $user_name = $user_split[0];

  my $long_text = $user_name . "> " . $pub_text ;

	my $node_value = { '_broadcast' => 1, 
                     pos => [0, 0, 0], 
                     rot => [0,0,0], 
                     scl => [1,1,1], 
                     id => $new_id, 
                     room_id => $room, 
                     attr => { type => 'Tete', 
                               chat => $chat_id, 
                               text => $long_text, 
                               user_name => $bot_name,
                               owner_color => $bot_color,
                               parent => Grids::UUID->new_id(), 
                               owner => $id->{name} } };

	$client->dispatch_event('Room.CreateObject', $node_value);

  if($last_irc_id) {
      select(undef, undef, undef, $millis_pause);

      $node_value = { '_broadcast' => 1, 
                      pos => [0,0,0], 
                      rot => [0,0,0], 
                      scl => [1,1,1], 
                      id => Grids::UUID->new_id(), 
                      room_id => $room, 
                      attr => { type => 'Link',
                                node1 => $last_irc_id,
                                node2 => $new_id, 
                                owner => $id->{name} } };

      $client->dispatch_event('Room.CreateObject', $node_value);
  }



  $last_irc_id = $new_id;
}

sub send_create {
 # nothing here
} 

sub run {

	$con = Grids::Console->new(
							   title => "IRC BOT",
							   prompt => "IRC BOT>",
							   handlers => {
								   send => \&send_create,
							   },
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
							#'Room.CreateObject' => \&create_object_cb,
							#'Room.UpdateObject' => \&update_object_cb,
							#'Room.ListObjects' => \&list_objects_cb,
							);

	::connect();

	$irc_client->connect( $irc_server, 6667, { nick => $bot_nick } );

	$con->listen_for_input;

	$main->recv;
}

