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

	my $args = $evt->args;
}

sub parse_update_object {
	my ($c, $evt) = @_;

	my $args = $evt->args;
}

sub create_generic_node {
	my ($c, $evt) = @_;

	my $args = $evt->args;
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

sub login_hook {
	my($c, $evt) = @_;

	my $args = $evt->args;
#login successful	
	if($args->{success} == 1) {
		print "LOGIN!!!\n";
	}
}

