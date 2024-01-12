FROM archlinux
COPY ./ /smq
WORKDIR /smq
RUN pacman -Syu  --noconfirm && pacman -S --noconfirm cmake make clang git
RUN <<EOF
    export CC=/usr/bin/clang \
    && export CXX=/usr/bin/clang++ \
    && mkdir build \
    && cd build \
    && cmake DCMAKE_BUILD_TYPE=Release .. \
    && make \
    && cd .. \
EOF
EXPOSE 3000
ENTRYPOINT ["/smq/build/smq", "3000"]
