#!/usr/bin/perl
use strict;

# Command-line interface to the Grids client

# Temp library loader while Grids is still in development
use FindBin;
use lib "$FindBin::Bin/../../grids-platform/lib";

use Grids::Client;
use Grids::Identity;
use Grids::Conf;

use Carp qw (croak);
use Getopt::Long;
use Data::Dumper;

my $id = Grids::Identity->new();
my $client = Grids::Client->new(id => $id, transport_class => 'Loop', debug => 1);

#$client->register_hook('Authentication.Login', \&login_hook);
#$client->register_hook('Services.List', \&client_service_list);
#$client->register_hook('Storage.List', \&client_storage_list);

#my $login_good = 0;

#$login_good = 1;
$client->login;

# Get session token
$client->session_token;

sub grids_write {
}

sub login_hook {
	my($c, $evt) = @_;

	my args = $evt->args;
#login successful	
	if($args->{success} == 1) {
		print "LOGIN!!!\n";
	}

}


