# NOTE: a PATCHED PHP on purpose - the vulnerability here is in the APP, not PHP.
FROM php:8.3.36-apache

# Set a global ServerName so Apache doesn't emit the AH00558 "could not reliably
# determine the server's fully qualified domain name" warning at startup.
# This service is loopback-only and reached as http://localhost:9001.
RUN echo 'ServerName localhost' >> /etc/apache2/apache2.conf

# 'ping' for the app's intended (benign) functionality.
RUN apt-get update \
 && apt-get install -y --no-install-recommends iputils-ping \
 && rm -rf /var/lib/apt/lists/*

COPY app/ /var/www/html/
EXPOSE 80
