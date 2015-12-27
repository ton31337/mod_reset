It can be used together with Jenkins for building these.
```
docker rmi $(docker images | awk '{if ($1 ~ /<none>/) {print $3;}}') ; true
docker build --rm --force-rm=true --no-cache=true -t apache22-php52 docker-packages/apache22
docker run --build-arg GIT_COMMIT=12hjksdhskdjfa1 --build-arg BUILD_URL=something --rm=true -v /var/www/rpms:/var/www/rpms:rw apache22-php52 bash -c "mv /usr/src/redhat/RPMS/x86_64/mod_reset_* /var/www/rpms"
```
