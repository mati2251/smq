FROM gradle:8.4.0-jdk20-alpine
COPY ./ /smq
WORKDIR /smq
RUN gradle :app:build --no-daemon
ENTRYPOINT ["java", "-jar", "/smq/app/build/libs/app.jar"]
