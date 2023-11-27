FROM archlinux
COPY ./ /smq
WORKDIR /smq
RUN pacman -Syu  --noconfirm && pacman -S --noconfirm cmake
RUN <<EOF
    mkdir build \
    && cd build \
    && cmake .. \
    && make \
    && cd .. \
EOF
ENV port 3333
EXPOSE $port
CMD /bin/sh /./build/smq 3333