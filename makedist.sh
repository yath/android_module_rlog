#!/bin/sh

chmod -R u+w .

autoreconf
./configure

sh ./makedist2.sh

