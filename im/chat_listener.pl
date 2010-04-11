

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

use Monolog;
use Phrase;

my $id = Grids::Identity->new();
my $client;
my $room;

my $con = Grids::Console->new(
                              title => "Chat Listener",
                              prompt => "Chat Listener>",
                              );

sub connected_cb {
	my($c, $evt) = @_;

	# Don't crash the server
	select(undef,undef,undef, $network_pause);

	$client->dispatch_event('Room.List');
}


sub broadcast_cb { } # I don't know what this method is supposed to do
sub request_list_rooms { $client->dispatch_event('Room.List'); }
sub room_create_cb { request_list_rooms(); }
sub create_room { $client->dispatch_event('Room.Create'); }
sub update_object_cb { } # No outside chat should be using this (outside chats except Wave don't support update)
sub list_object_cb { }

# Set yourself to the first room
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

# Chat messages are sent as create object requests
sub create_object_cb {
	my($c, $evt) = @_;

	return unless($evt);
	my $args = $evt->args;
	return if( $args->{success});  # Filter out the bounceback confirmation code
	return if( $args->{error}); 	
	return unless( $evt->args->{attr}->{type} );

  if( $evt->args->{attr}->{type} eq "AddOutsideAccount"){

  } elsif( $evt->args->{attr}->{type} eq "OutsideChat" ){

  }

}

sub create_node_from_chat {
    #my($protocol, $sender_screen_name, $receiver_screen_name


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

run();
