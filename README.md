# JEvolve theme for phpdocumentor. #

this theme relies on jquery ui

this is a modified version of [onigoetz.ch](http://www.onigoetz.ch/fr/2010/09/11/new-theme-for-phpdocumentor/) Evolve modified theme.

original evolve theme is available at [crazytje.be](http://evolve.crazytje.be/)

## third party libraries ##

 * jquery
 * jquery-ui
 * jquery.cookie
 * jquery layout
 * jstree

## Install ##

if you used pear to install phpDocumentor, just used provided deploy.sh script

    # ./deploy.sh

it will do it all :)

## generate doc ##

Use the following

    ./phpdocumentor/phpdoc -d './phpdocumentor' -t '/var/www/html/docs/' -o HTML:Smarty/JEvolve:default -s on

## WARNING ##

this piece of code is provided as is and is used to fit my personnal needs.
