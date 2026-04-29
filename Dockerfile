FROM ubuntu:24.04 AS geant4-builder

ARG DEBIAN_FRONTEND=noninteractive
ARG GEANT4_VERSION=11.3.2

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    build-essential \
    cmake \
    ninja-build \
    wget \
    libexpat1-dev \
    libxerces-c-dev \
    zlib1g-dev \
    libhdf5-dev \
    qtbase5-dev \
    libqt5opengl5-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libx11-dev \
    libxmu-dev \
    libxi-dev \
    libxpm-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /tmp
RUN wget -q https://gitlab.cern.ch/geant4/geant4/-/archive/v${GEANT4_VERSION}/geant4-v${GEANT4_VERSION}.tar.gz \
    && mkdir /tmp/geant4-source \
    && tar -xzf geant4-v${GEANT4_VERSION}.tar.gz -C /tmp/geant4-source --strip-components=1

RUN cmake -S /tmp/geant4-source -B /tmp/geant4-build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/opt/geant4 \
    -DGEANT4_BUILD_MULTITHREADED=OFF \
    -DGEANT4_INSTALL_DATA=ON \
    -DGEANT4_USE_GDML=ON \
    -DGEANT4_USE_HDF5=ON \
    -DGEANT4_USE_OPENGL_X11=ON \
    -DGEANT4_USE_QT=ON \
    && cmake --build /tmp/geant4-build \
    && cmake --install /tmp/geant4-build

FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    build-essential \
    cmake \
    ninja-build \
    libexpat1-dev \
    libxerces-c-dev \
    zlib1g-dev \
    libhdf5-dev \
    qtbase5-dev \
    libqt5opengl5-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libx11-dev \
    libxmu-dev \
    libxi-dev \
    libxpm-dev \
    && rm -rf /var/lib/apt/lists/*

COPY --from=geant4-builder /opt/geant4 /opt/geant4

ENV Geant4_DIR=/opt/geant4/lib/cmake/Geant4
ENV CMAKE_PREFIX_PATH=/opt/geant4
ENV PATH=/opt/geant4/bin:${PATH}

WORKDIR /workspace
COPY . /workspace

RUN cmake -S EMShowerSim -B EMShowerSim/build -G Ninja \
    -DGeant4_DIR=/opt/geant4/lib/cmake/Geant4 \
    && cmake --build EMShowerSim/build

RUN mkdir -p /output

WORKDIR /workspace/EMShowerSim/build
ENTRYPOINT ["/bin/bash", "-lc"]
CMD ["source /opt/geant4/bin/geant4.sh && ./EMShower ../run.mac && cp shower_data.h5 /output/shower_data.h5"]
