#!/bin/bash

msgfmt -c -v -o asswordk.mo french.po

sudo cp asswordk.mo /usr/share/locale/fr/LC_MESSAGES/asswordk.mo
