#!/bin/bash

echo "	********************************"
echo "	*         APACHE/ECHO          *"
echo "	********************************"

echo "Nota: asegurate de que tienes el servicio APACHE instalado"
echo "Nota: Este script debe ejecutarse como superusuario o root"

echo "Introuce el nombre de la red:"
read nomRed

echo "Introduce la red del laboratorio [10.128.RED.1]: "
read red

mkdir /web

cp -R etc/* /etc #~/Escritorio/prueba/etc/
cp -R web/* /web #~/Escritorio/prueba/web/

cd / 

#modifico los ficheros con la red y nombre correspondientes

sed -i "s/redname/$nomRed/g" etc/apache2/sites-available/www.red-laboratorio.lab
sed -i "s/red_/$red/g" etc/apache2/sites-available/www.red-laboratorio.lab
sed -i "s/red_/$red/g" etc/apache2/mods-available/userdir.conf

#cambio de nombre el directorio para que cuadre con el nombre de la red

mv web/www.red-redname.lab web/www.red-$nomRed.lab


#habitlito la web

a2enmod userdir
a2ensite www.red-laboratorio.lab
service apache2 reload

#Permisos de las carpetas de la web

chmod -R 2755 /web
chgrp -R www-data /web

#para el servidor de ECHO:

#cd /etc/ECHO #voy a la ruta del echo
#make #compilo
#./echod-select #ejecuto el servidor de echo

echo "terminado... by josegury"

