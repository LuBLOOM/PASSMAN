use warnings;
use strict;

my $HELP_SRC_NAME = "./src/help.c";
my $HELP_SRC_TEXT = "help.txt";

open HELP_SRC, ">$HELP_SRC_NAME" or die "Could not open $HELP_SRC_NAME\n";
open HELP_TXT, "<$HELP_SRC_TEXT" or die "Could not open $HELP_SRC_TEXT\n";

print HELP_SRC "char *help_msg = ";
while (<HELP_TXT>) {
    my $ln = $_;
    chop $ln;
    print HELP_SRC "\"$ln\\n\"\n";
}
print HELP_SRC "\"\";\n";

close HELP_SRC or die "$HELP_SRC_NAME: ";
close HELP_TXT or die "$HELP_SRC_TEXT: ";
