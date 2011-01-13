PEAR_DIR=`pear config-get php_dir`
PEAR_DATA_DIR=`pear config-get data_dir`


PHPDOC_DIR=$PEAR_DIR/PhpDocumentor/phpDocumentor
PHPDOC_CONVERTER_DIR=$PHPDOC_DIR/Converters/HTML/Smarty
PHPDOC_DATA_DIR=$PEAR_DATA_DIR/PhpDocumentor/phpDocumentor/Converters/HTML/Smarty/

if [ ! -d $PHPDOC_DIR -o ! -d $PHPDOC_DATA_DIR -o ! -d $PHPDOC_CONVERTER_DIR ]
then
  echo 'make sure you installed PhpDocumentor as pear package'
fi;

if [ -d $PHPDOC_CONVERTER_DIR/JEvolve ]
then
  rm -Rf $PHPDOC_CONVERTER_DIR/JEvolve
fi

if [ -d $PHPDOC_DATA_DIR/JEvolve ]
then
  rm -Rf $PHPDOC_DATA_DIR/JEvolve
fi

mkdir $PHPDOC_CONVERTER_DIR/JEvolve
mkdir $PHPDOC_DATA_DIR/JEvolve

cp ./HTMLSmartyJEvolveConverter.inc $PHPDOC_CONVERTER_DIR/JEvolve/
cp -R ./templates $PHPDOC_DATA_DIR/JEvolve/
