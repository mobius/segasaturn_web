# syntax=docker/dockerfile:1

# ---- Build stage: compile the Ymir WASM module with Emscripten ----
FROM emscripten/emsdk:3.1.74 AS build

ARG CMAKE_VERSION=3.30.5

RUN apt-get update \
    && apt-get install -y --no-install-recommends ninja-build curl ca-certificates \
    && rm -rf /var/lib/apt/lists/* \
    && ARCH="$(uname -m)" \
    && curl -fsSL -o /tmp/cmake.sh \
        "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-${ARCH}.sh" \
    && sh /tmp/cmake.sh --skip-license --prefix=/usr/local \
    && rm /tmp/cmake.sh

WORKDIR /src

# The web project pulls in ../ymir via add_subdirectory, so both are required.
COPY web/ ./web/
COPY ymir/ ./ymir/

RUN emcmake cmake -S web -B web/build-web -G Ninja \
        -DARCHITECTURES=wasm32 -DCMAKE_BUILD_TYPE=Release \
    && cmake --build web/build-web --parallel

# Assemble the deployable site (mirrors web/serve.sh).
RUN mkdir -p /dist \
    && cp web/build-web/ymir-web.js web/build-web/ymir-web.wasm /dist/ \
    && cp web/shell/index.html web/shell/main.js web/shell/tiny-unzip.js \
          web/shell/ymir-web-audio-worklet.js /dist/

# ---- Runtime stage: serve static files with nginx ----
FROM nginx:alpine

# Railway injects PORT; nginx renders this template with envsubst on startup.
ENV PORT=8080
RUN rm -f /etc/nginx/conf.d/default.conf \
    && mkdir -p /etc/nginx/templates \
    && echo 'server {' > /etc/nginx/templates/default.conf.template \
    && echo '    listen ${PORT};' >> /etc/nginx/templates/default.conf.template \
    && echo '    listen [::]:${PORT};' >> /etc/nginx/templates/default.conf.template \
    && echo '    root /usr/share/nginx/html;' >> /etc/nginx/templates/default.conf.template \
    && echo '    index index.html;' >> /etc/nginx/templates/default.conf.template \
    && echo '    location / {' >> /etc/nginx/templates/default.conf.template \
    && echo '        try_files $uri $uri/ /index.html;' >> /etc/nginx/templates/default.conf.template \
    && echo '    }' >> /etc/nginx/templates/default.conf.template \
    && echo '    location ~* \.wasm$ {' >> /etc/nginx/templates/default.conf.template \
    && echo '        default_type application/wasm;' >> /etc/nginx/templates/default.conf.template \
    && echo '    }' >> /etc/nginx/templates/default.conf.template \
    && echo '}' >> /etc/nginx/templates/default.conf.template

COPY --from=build /dist/ /usr/share/nginx/html/

EXPOSE ${PORT}
