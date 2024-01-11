FROM archlinux
COPY ./ /smq
WORKDIR /smq
RUN pacman -Syu  --noconfirm && pacman -S --noconfirm cmake make clang git
RUN <<EOF
    export CC=/usr/bin/clang \
    && export CXX=/usr/bin/clang++ \
    mkdir build \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release .. \
    && make \
    && cd .. \
EOF
ENV port 3000
EXPOSE $port
CMD /smq/build/smq $port
