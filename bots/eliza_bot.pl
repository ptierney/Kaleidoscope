
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
use JSON::XS;

package ElizaBot;

my $id;
my $client;
my $room;
my $chat; # the id of the current ChatNode the bot is linked to 
my $bot_name;
my $color;
my $chat_node_id;
my $last_chat_message_node_id;
my $eliza;

package main;

$ElizaBot::id = Grids::Identity->new();
$ElizaBot::bot_name = "Eliza";
$ElizaBot::color = [rand()*10000 % 255,
                 rand()*10000 % 255,
                 rand()*10000 % 255 ];
$ElizaBot::eliza = new Chatbot::Eliza($bot_name, "script.txt");

my $network_pause = 0.1;

my $con = Grids::Console->new(
    title => $ElizaBot::bot_name,
    prompt => "$ElizaBot::bot_name >",
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
    my $new_room = $rooms->[0];

    # No rooms on the server
    unless ($new_room) { 
        create_room();
        return; 
    }
    
    $ElizaBot::room = $new_room;

    $con->print("$ElizaBot::bot_name has set room to: $new_room");
}

sub create_room { $client->dispatch_event('Room.Create'); }

sub create_room_cb { } # When someone else creates a room

sub broadcast_cb { } # I don't know what this method is supposed to do

sub create_object_cb { 
    my($c, $evt) = @_;

    return unless($evt);
    my $args = $evt->args;
    return if( $args->{success});  # Filter out the bounceback confirmation code
    return if( $args->{error}); 	
    return unless( $evt->args->{attr}->{type} );
    #return unless( $evt->args->{attr}->{type} eq "CameraNode" );

    if ($evt->args->{attr}->{type} eq "CameraNode") {
        $con->print("Requesting UserNode");

        request_create_user_node();
    } elsif ($evt->args->{attr}->{type} eq "ChatNode") {
        $con->print("Received ChatNode");
        
        $ElizaBot::chat_node_id = $evt->{id};
    } elsif ($evt->args->{attr}->{type} eq "ChatMessageNode") {
        $con->print("Received ChatMessageNode");
        
        process_chat_message_node($evt);
    }
}

sub update_object_cb {
    my($c, $evt) = @_;

    return;

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
                       pos => [0,0,0], 
                       rot => [0,0,0], 
                       scl => [1,1,1], 
                       id => $new_id, 
                       room_id => $room, 
                       attr => { type => 'ChatMessage', 
                                 links => [ { id => $chat, read => 1, modify => 1 },
                                            { id => $id->{uuid}, creator => 1, read => 1, modify => 1 } ], 
                                 text => $reply } };

    $client->dispatch_event('Room.CreateObject', $node_value);
}

# Requests to the network that a UserNode be created
sub request_create_user_node {
    my $ran_x = rand(2) - 1;
    my $ran_y = rand(2) - 1;
    my $ran_z = rand(2) - 1;

    $con->print("x = $ran_x, y = $ran_y, z = $ran_z");

    my $test_scale = 0;
    $ran_x *= $test_scale;
    $ran_y *= $test_scale;
    $ran_z *= $test_scale;

    my $node_value = { '_broadcast' => 1, 
                       pos => [$ran_x, $ran_y, $ran_z], 
                       rot => [0,0,0], 
                       scl => [1,1,1], 
                       id => $ElizaBot::id->{uuid}, 
                       room_id => $ElizaBot::room, 
                       attr => { type => 'UserNode', 
                                 links => [ { id => $ElizaBot::id->{uuid},
                                              name => $ElizaBot::bot_name, 
                                              read => JSON::XS::true, 
                                              modify => JSON::XS::true,
                                              creator => JSON::XS::true } ] } };

    $client->dispatch_event('Room.CreateObject', $node_value);
}

sub process_chat_message_node {
    my ($evt) = @_;
    my $tete_text = $evt->args->{attr}->{text};
    
    my $reply = $ElizaBot::eliza->transform($tete_text);
}

# TODO: resolve position, etc
sub create_chat_message_node {
    my ($chat_id, $text) = @_;
    
    my $node_value = { '_broadcast' => 1,
                       pos => [0,0,0],
                       rot => [0,0,0],
                       scl => [1,1,1],
                       id => Grids::UUID->new_id(),
                       room_id => $ElizaBot::room };
}

sub run {
    # main loop condition
    my $main = AnyEvent->condvar;

    $ElizaBot::client = Grids::Client->new(id => $id, use_encryption => 0, debug => 0, auto_flush_queue => 1);

    # Refer to Grids::Node::Hooks.. for hooks
    $ElizaBot::client->register_hooks(
        'Authentication.Login' => \&connected_cb,
        'Broadcast.Event' => \&broadcast_cb,
        'Connected' => \&connected_cb,
        'Room.Create' => \&create_room_cb,
        'Room.List' => \&list_rooms_cb,
        'Room.CreateObject' => \&create_object_cb,
        'Room.UpdateObject' => \&update_object_cb,
        #'Room.ListObjects' => \&list_objects_cb,
        );

    ::connect();

    $con->listen_for_input;

    $main->recv;
}
