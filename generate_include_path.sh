#!/bin/sh
# @Author: Timé Kadel <root>
# @Date:   2020-01-30T15:38:03+01:00
# @Email:  time@asls.tk
# @Project: ASLS-INVIA V2.0
# @Filename: generate_include_path.sh
# @Last modified by:   root
# @Last modified time: 2020-01-30T18:23:40+01:00
# @License: This software is a property of ASLS - Autonomous Stage Lighting System.
# @Copyright: © 2018 ASLS All Rights Reserved

CDT_HEADER="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<cdtprojectproperties>\n<section name=\"org.eclipse.cdt.internal.ui.wizards.settingswizards.IncludePaths\">\n<language name=\"C Source File\">"
CDT_FOOTER="</language>\n</section>\n</cdtprojectproperties>"
CDT_INC=$(find  -type f -name "*.h" | grep -o '^.*/' | sed "s/\../\<includepath\>/; s/$/\<\/includepath>/" | sort -u)

echo "$CDT_HEADER$CDT_INC$CDT_FOOTER" > cdt_include_path.xml
