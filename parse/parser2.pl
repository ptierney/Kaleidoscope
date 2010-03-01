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
	my($c, $evt) = @_;

	unless($evt){
		return;
	}

	my $args = $evt->args;

	# Filter out the bounceback confirmation code
	if( $args->{success}){
		return;
	}

	if( $evt->args->{attr}->{type} eq "InputText"){
		$con->print("CREATED INPUT TEXT!!!");

		my $node_value;
		$node_value->{pos}->{0} = '0.0';
		$node_value->{pos}->{1} = '0.0';
		$node_value->{pos}->{2} = '0.0';
		# pos => [ 0.0, 0.0, 0.0 ]
		
		$node_value->{id} = Grids::UUID->new_id;

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
    if (my $chpid = fork()) {
        # parent, run console
        $con->listen_for_input;

        kill 'TERM', $chpid;
    } else {
        # child, run client
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

        my $finished = 0;
        local $SIG{TERM} = sub {
            $finished = 1;
        };

        while (! $finished) {
            $client->select;
        }
    }
}
