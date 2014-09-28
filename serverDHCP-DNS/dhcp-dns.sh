#!/bin/bash

echo "	*******************************"
echo "	*          DHCP/DNS           *"
echo "	*******************************"

echo "Nota: asegurate de que tienes los servicios DHCP y DNS instalados"
echo "Nota: Este script debe ejecutarse como superusuario o root"

echo "Introduce la mac del ROUTER: "
read router

echo "Introduce la mac del SERVIDOR2: "
read server


echo "Introuce el nombre de la red:"
read nomRed

echo "Introduce la red del laboratorio [10.128.RED.1]: "
read red

echo "Introduce la red del laboratorio Broadcast [10.128.RED.255]: "
read redbroad

echo "Introduce el interface de la red (MAC) del PC [00:10:...]: "
read interface


cp -R etc/* /etc 

cd / 

#modificaciones del dhcp/dhcpd.conf

sed -i "s/MAC-ROUTER/$router/g" etc/dhcp/dhcpd.conf
sed -i "s/MAC-SERVIDOR/$server/g" etc/dhcp/dhcpd.conf
sed -i "s/rednombre/$nomRed/g" etc/dhcp/dhcpd.conf
sed -i "s/red_/$red/g" etc/dhcp/dhcpd.conf
sed -i "s/dirbroad/$redbroad/g" etc/dhcp/dhcpd.conf

#modificaciones del network/interfaces

sed -i "s/red/$red/g" etc/network/interfaces
sed -i "s/inter/$interface/g" etc/network/interfaces

#modificaciones del /default/*

sed -i "s/inter/$interface/g" etc/default/dhcp3-server
sed -i "s/inter/$interface/g" etc/default/isc-dhcp-server

#modificacion bind/red.lab

sed -i "s/rednombre/$nomRed/g" etc/bind/db.red.lab
sed -i "s/red_/$red/g" etc/bind/db.red.lab

#modificacion bind/red.lab

sed -i "s/rednombre/$nomRed/g" etc/bind/db.10.128.136

#modificacionbind/named.conf.local

sed -i "s/red_/$red/g" etc/bind/named.conf.local
sed -i "s/rednombre/$nomRed/g" etc/bind/named.conf.local

#cambio el nombre del fichero etc/bind/db.10.128.136 para que sea el de su red

mv etc/bind/db.10.128.136 etc/bind/db.10.128.$red
mv etc/bind/db.red.lab etc/bind/db.red-$nomRed.lab

#inicio el servicio de dhcp

service isc­-dhcp­-server restart

#inicio el servicio DNS

service bind9 restart

echo "terminado... by josegury"


