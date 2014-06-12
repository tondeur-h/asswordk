#!/bin/bash

xgettext -d cat_asswordk -s --copyright-holder="Tondeur Herve" --package-name=ASSWORDK --package-version=0.3 --msgid-bugs-address=tondeur.herve@yahoo.fr -o asswordk.pot ./*.cpp
msginit -l fr -o french.po -i asswordk.pot

rm asswordk.pot

vim french.po
