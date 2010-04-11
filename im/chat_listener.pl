

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

use Net::Telnet;

use OutsideChat;

my $id = Grids::Identity->new();
my $client;
my $room;
my $network_pause = 0.1;

my $con = Grids::Console->new(
                              title => "Chat Listener",
                              prompt => "Chat Listener>",
                              );

# Disable the Errmode, default is die
# This is used to talk to Purpled
my $telnet = new Net::Telnet( Host => 'localhost',
                              Port => '32000',
                              Errmode => sub { } );

# Both these hashrefs are indexed by protocol-screenneame
# eg MrBob-AIM or patrick-xmpp
my $screen_id = { }; # holds the screen 
my $outside_chats = { };

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
      #$con->print(Dumper($args));
      my $screen_name = $evt->args->{attr}->{screen_name}; 
      my $grids_id = $evt->args->{attr}->{owner}; 
      # Set the hash so we can reference it later
      my $protocol = $evt->args->{attr}->{protocol}; 
      $screen_id->{"$protocol-$screen_name"} = $grids_id;
      my $password = $evt->args->{attr}->{password}; 
      purpled_add_account($protocol, $screen_name, $password);
      sleep(1);
      purpled_refresh_account_list();
  } elsif( $evt->args->{attr}->{type} eq "OutsideChat" ){
      # Get the name: protocol-screenname
      # Access the OutsideChat object
      # get the index
      # Send message using index
      my $protocol = $args->{attr}->{protocol};
      my $send_screen_name = $args->{attr}->{send_screen_name};
      my $receive_screen_name = $args->{attr}->{receive_screen_name};
      my $message = $args->{attr}->{message};
      my $hash_index = "$protocol-$send_screen_name";
      my $chat_object = $outside_chats->{$hash_index};
      return unless($chat_object);
      my $chat_index = $chat_object->index();
      purpled_send_im($chat_index, $receive_screen_name, $message);
  }
}

sub create_node_from_chat {
    #my($protocol, $sender_screen_name, $receiver_screen_name
}

sub purpled_refresh_account_list(){
    $telnet->print("account list");
}

sub purpled_add_account {
    my($protocol, $screen_name, $password) = @_;

    $telnet->print("account add $protocol $screen_name $password");
}

sub purpled_check {
    my $response = $telnet->getline(Timeout => 1);

    return unless($response);
    chomp($response);
    # If we are getting the account list, update our account 
    if($response eq "Start Account List"){
        my @account_list;
        $response = $telnet->getline(Timeout => 1);
        chomp($response);
        until( $response eq "End Account List"){
            push(@account_list, $response);
            $response = $telnet->getline(Timeout => 1);
            chomp($response);
        }
        parse_account_list(@account_list);
    } else {
        parse_purpled_im($response);
    }
}

sub purpled_send_im {
    my ($index, $other_name, $message) = @_;
}

sub parse_purpled_im {
    my($message) = @_;

    # $parse[0] is the index
    # $parse[1] is name of the person who sent it
    # $parse[2] is the message
    my @parse = split(/\^\^\^/, $message);
    my ($key, $object) = find_outside_chat_by_index($parse[0]);
    return unless($object);

    my $send_value = { '_broadcast' => 1, 
                       pos => [0,0,0], 
                       rot => [0,0,0], 
                       scl => [1,1,1], 
                       id => Grids::UUID->new_id,
                       room_id => $room, 
                       attr => { type => 'OutsideChat',
                                 protocol => $object->protocol(),
                                 send_screen_name => $parse[1],
                                 receive_screen_name => $object->screen_name,
                                 message => $parse[2],
                             } };

    $client->dispatch_event('Room.CreateObject', $send_value);
}

sub parse_account_list {
    my( @account_list) = @_;

    foreach my $account (@account_list){
        $con->print($account);
        # $parse[0] is the index
        # $parse[1] is the protocol name
        # $parse[2] is the screen name
        # $parse[3] is the status
        my @parse = split(/\s/, $account);
        my $outside_chat = OutsideChat->new();
        $outside_chat->index($parse[0]);
        $outside_chat->protocol($parse[1]);
        $outside_chat->screen_name($parse[2]);
        $outside_chat->status($parse[3]);
        $outside_chats->{ "$parse[1]-$parse[2]" } = $outside_chat;
    }
}

sub find_outside_chat_by_index {
    my($index) = @_;
    
    while( my ($key, $value) = each %$outside_chats ){
        if( $value->index() == $index ){
            return ($key, $value);
        }
    }
    
    return undef;
}

sub run {
    # main loop condition
    my $main = AnyEvent->condvar;

    my $w = AnyEvent->timer( after => 3.0, 
                             interval => 3.0, 
                             cb => \&purpled_check);

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


