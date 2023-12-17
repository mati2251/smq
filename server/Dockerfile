FROM archlinux
COPY ./ /smq
WORKDIR /smq
RUN pacman -Syu  --noconfirm && pacman -S --noconfirm cmake make gcc git
RUN <<EOF
    mkdir build \
    && cd build \
    && cmake .. \
    && make \
    && cd .. \
EOF
ENV port 3000
EXPOSE $port
CMD /smq/build/smq $port
