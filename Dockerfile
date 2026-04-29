ARG GEANT4_PLATFORM=linux/amd64
FROM --platform=${GEANT4_PLATFORM} mambaorg/micromamba:2.0.5

USER root

RUN micromamba install -y -n base -c conda-forge \
    cmake \
    cxx-compiler \
    geant4=11.3.2 \
    hdf5 \
    ninja \
    && micromamba clean --all --yes

ENV PATH=/opt/conda/bin:${PATH}
ENV CMAKE_PREFIX_PATH=/opt/conda
ENV Geant4_DIR=/opt/conda/lib/cmake/Geant4

RUN geant4-config --features \
    && geant4-config --features | grep 'hdf5\[yes\]'

WORKDIR /workspace
COPY . /workspace

RUN cmake -S EMShowerSim -B EMShowerSim/build -G Ninja \
    -DGeant4_DIR=/opt/conda/lib/cmake/Geant4 \
    && cmake --build EMShowerSim/build

RUN mkdir -p /output

WORKDIR /workspace/EMShowerSim/build
ENTRYPOINT ["/bin/bash", "-lc"]
CMD ["./EMShower ../run.mac && cp shower_data*.h5 /output/"]
