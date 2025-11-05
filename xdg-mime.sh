#!/bin/bash

if [ "$1" == "query" ] && [ "$2" == "filetype" ]; then
  echo "application/octet-stream"
fi
