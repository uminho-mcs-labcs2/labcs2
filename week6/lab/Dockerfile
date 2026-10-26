# Single image parameterized by the PHP version.
# docker-compose instantiates it twice: one "vulnerable", one "fixed".
ARG PHP_TAG=8.3.35-apache
FROM php:${PHP_TAG}

# Leave expose_php ON on purpose in the lab: the X-Powered-By header is one of the
# fingerprint vectors we explore in E1.
# (The discussion about turning it off — and why it is NOT a mitigation — is part
# of the class.)

COPY app/ /var/www/html/

# Check HTTP readiness, so Compose --wait waits for Apache to serve the app.
HEALTHCHECK --interval=2s --timeout=3s --start-period=10s --retries=15 \
    CMD php -r 'exit(@file_get_contents("http://127.0.0.1/version.php") === false ? 1 : 0);'

# Apache already serves /var/www/html by default in the official php:*-apache image.
EXPOSE 80
