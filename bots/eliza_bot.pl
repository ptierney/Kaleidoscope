
use warnings;
use strict;

use FindBin;
use lib "$FindBin::Bin/../../grids-platform/lib";

use Chatbot::Eliza;

use Grids::Address::IPv4;
use Grids::Client;
use Grids::Identity;
use Grids::Console;
use Grids::Conf;

my $id = Grids::Identity->new();
my $client;
my $room;
my $network_pause = 0.1;
my $bot_name = "Eliza";
my $bot_color = [rand()*10000 % 255,
                 rand()*10000 % 255,
                 rand()*10000 % 255 ];

my $eliza = new Chatbot::Eliza $bot_name, "script.txt";

my $con = Grids::Console->new(
                              title => "Eliza",
                              prompt => "Eliza>",
                              );

run();

sub connect {
    my $address = Grids::Address::IPv4->new( address => '127.0.0.1' );
    $client->connect($address);
}

sub connected_cb {
	my($c, $evt) = @_;

	# Don't crash the server
	select(undef,undef,undef, $network_pause);

	$client->dispatch_event('Room.List');
}


sub list_rooms_cb {
	my ($c, $evt) = @_;

	my $rooms = $evt->args->{rooms};
	$room = $rooms->[0];

	# No rooms on the server
	unless ($room) { 
		create_room();
		return; 
	}
	$con->print("Your room now set to $room");
}

sub broadcast_cb { } # I don't know what this method is supposed to do

sub create_object_cb { }

sub update_object_cb {
	my($c, $evt) = @_;

	return unless($evt);
	my $args = $evt->args;
	return if( $args->{success});  # Filter out the bounceback confirmation code
	return if( $args->{error}); 	
	return unless( $evt->args->{attr}->{type} );
  return unless( $evt->args->{attr}->{finished} );
  return unless( $evt->args->{attr}->{type} eq "Tete" );

  my $tete_text = $evt->args->{attr}->{text};
  my $tete_id = $evt->args->{attr}->{id};
  my $new_id = Grids::UUID->new_id();
  my $chat_id = $evt->args->{attr}->{chat_id};

  my $reply = $eliza->transform($tete_text);

  my $node_value = { '_broadcast' => 1, 
                     pos => [0, 0, 0], 
                     rot => [0,0,0], 
                     scl => [1,1,1], 
                     id => $new_id, 
                     room_id => $room, 
                     attr => { type => 'Tete', 
                               chat => $chat_id, 
                               text => $reply,
                               user_name => $bot_name,
                               owner_color => $bot_color,
                               parent => Grids::UUID->new_id(), 
                               owner => $id->{name} } };

  $client->dispatch_event('Room.CreateObject', $node_value);

  select(undef, undef, undef, $network_pause);

  $node_value = { '_broadcast' => 1, 
                  pos => [0,0,0], 
                  rot => [0,0,0], 
                  scl => [1,1,1], 
                  id => Grids::UUID->new_id(), 
                  room_id => $room, 
                  attr => { type => 'Link',
                            node1 => $tete_id,
                            node2 => $new_id, 
                            owner => $id->{name} } };

  $client->dispatch_event('Room.CreateObject', $node_value);
}

sub run {
    # main loop condition
    my $main = AnyEvent->condvar;

    $client = Grids::Client->new(id => $id, use_encryption => 0, debug => 0, auto_flush_queue => 1);

    # Refer to Grids::Node::Hooks.. for hooks
    $client->register_hooks(
                            'Authentication.Login' => \&connected_cb,
                            'Broadcast.Event' => \&broadcast_cb,            
                            'Connected' => \&connected_cb,
                            'Room.Create' => \&room_create_cb,
                            'Room.List' => \&list_rooms_cb,
                            'Room.CreateObject' => \&create_object_cb,
                            'Room.UpdateObject' => \&update_object_cb,
                            #'Room.ListObjects' => \&list_objects_cb,
                            );

    ::connect();

    $con->listen_for_input;

    $main->recv;

}
