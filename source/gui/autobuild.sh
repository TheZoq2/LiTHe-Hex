#!/bin/bash

kill `cat pid`
MIX_ENV=prod mix compile
brunch build --production
MIX_ENV=prod mix phoenix.digest
MIX_ENV=prod PORT=4003 elixir --detached -e "File.write! 'pid', :os.getpid" -S  mix do compile, phoenix.server
