# NOTE: a PATCHED PHP on purpose - the vulnerability here is in the APP, not PHP.
FROM php:8.3.36-apache

# 'ping' for the app's intended (benign) functionality.
RUN apt-get update \
 && apt-get install -y --no-install-recommends iputils-ping \
 && rm -rf /var/lib/apt/lists/*

COPY app/ /var/www/html/
EXPOSE 80
