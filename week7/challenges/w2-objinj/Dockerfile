# PATCHED PHP on purpose - the vulnerability is in the APP (insecure unserialize).
FROM php:8.3.36-apache

# Set a global ServerName so Apache doesn't emit the AH00558 "could not reliably
# determine the server's fully qualified domain name" warning at startup.
# This service is loopback-only and reached as http://localhost:9002.
RUN echo 'ServerName localhost' >> /etc/apache2/apache2.conf

COPY app/ /var/www/html/
EXPOSE 80
