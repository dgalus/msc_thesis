#!/bin/bash
wget -N http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz && gunzip GeoIP.dat.gz && mv GeoIP.dat ../../app/geolocation/