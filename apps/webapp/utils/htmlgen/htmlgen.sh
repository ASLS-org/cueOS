#!/bin/bash

DIST_PATH="./dist"
RELEASE_PATH="./release/htmlgen.c"

./.htmlgen $DIST_PATH -f:$RELEASE_PATH
