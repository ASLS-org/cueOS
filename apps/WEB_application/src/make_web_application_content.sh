#!/bin/bash

DIST_PATH="./dist"
RELEASE_PATH="./web_application_content.c"

./.make_web_application_content $DIST_PATH -f:$RELEASE_PATH
