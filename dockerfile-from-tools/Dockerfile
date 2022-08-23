FROM espressif/idf:latest


# this is from qemu build docker
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    libglib2.0-dev \
    binutils-dev \
    autoconf \
    libtool \
    libssl-dev \
    libpixman-1-dev \
    libgcrypt-dev \
    python3 \
    python3-venv \
    python3-pip \
    libdbus-1-dev \
    libusb-1.0-0-dev \
  && rm -rf /var/lib/apt/lists/*

# build pytest apps
WORKDIR /opt/esp/idf

# install ESP IDF tools including pytest
RUN . ./export.sh && python3 ./tools/idf_tools.py install-python-env --features pytest,ci


RUN . ./export.sh && python3 /opt/esp/idf/tools/ci/ci_build_apps.py . --target esp32 -m qemu -vv --pytest-apps

# set path up front for QEMU binaries target destination
ENV PATH=/qemu-xtensa:${PATH}

# clone and build QMEU, currently disabled as it needs GitLab credentials
WORKDIR /
RUN cd / && git clone https://github.com/espressif/qemu.git --recursive

RUN mkdir /qemu-xtensa && cd /qemu-xtensa && ../qemu/configure --prefix=`pwd`/root --target-list=xtensa-softmmu,xtensaeb-softmmu
RUN cd /qemu-xtensa && make install

WORKDIR /opt/esp/idf
RUN . ./export.sh && pytest --target esp32 -m qemu --embedded-services idf,qemu

# additional python tools that maybe useful for some scripts
# RUN pip install matplotlib
# RUN pip install pandas
# RUN pip install jira==3.2.0
# RUN pip install PyGithub==1.54.1
# RUN pip3 install python-gitlab==3.0.0
