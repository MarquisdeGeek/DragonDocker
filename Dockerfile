FROM ubuntu:22.04
LABEL Version="1.2" \
      Date="2024-Feb-24" \
      Docker_Version="Docker version 25.0.3, build 4debf41" \
      Vendor="em.ulat.es" \
      Maintainer="Steven Goodwin - Marquis de Geek (@marquisdeGeek)" \
      Description="A basic Docker container to compile and use Dragon tools"

ARG   USER_NAME=user
ARG   USER_PASSWORD=${USER_NAME}
ARG   USER_PATH=/home/${USER_NAME}

# Create a non-root user
RUN useradd -ms /bin/bash $USER_NAME && echo "$USER_NAME:$USER_PASSWORD" | chpasswd && adduser $USER_NAME sudo


# Base OS and tools
RUN apt-get update && \
    apt-get install -y wget unzip git sudo && \
    apt-get remove --purge --auto-remove -y


# System-wide installation:
#   the emulator, assembler, and C compiler to build cmoc (the 6809 C compiler)
RUN apt-get install -y --no-install-recommends --no-install-suggests software-properties-common dirmngr apt-transport-https lsb-release ca-certificates gpg-agent && \
    add-apt-repository -y ppa:sixxie/ppa && \
    apt-get update && \
    apt-get install -y xroar asm6809 && \
    apt-get install -y bison flex && \
    apt-get install -y make g++ && \
    apt-get remove --purge --auto-remove -y


# User-wide installation:
USER $USER_NAME


# The ROM
RUN cd ${USER_PATH} && \
    wget -O romdata.zip https://archive.worldofdragon.org/phpBB3/download/file.php?id=5875 && \
    unzip romdata.zip && \
    mkdir -p dragon/roms && \
    cat D32-10 D32-11 >dragon/roms/d32.rom && \
    rm D32-10 D32-11 romdata.zip


# BASIC compilation tools
# This uses a really old version, because it worked with less fuss than the new ones :(
USER root
RUN cd ${USER_PATH}/dragon && \
    apt-get install -y python2 && \
    apt-get remove --purge --auto-remove -y

USER $USER_NAME
RUN cd ${USER_PATH}/dragon && \
    wget -O pydc.zip https://github.com/jedie/PyDragon32/archive/refs/heads/master.zip && \
    mkdir contrib && \
    unzip pydc.zip && \
    mv PyDragon32-master contrib && \
    rm pydc.zip


# C compilation tools (currently using CMOC, although gcc6809 exists)
# The make install process puts the tools in the global path
USER root
RUN cd ${USER_PATH}/dragon && \
    wget http://www.lwtools.ca/releases/lwtools/lwtools-4.22.tar.gz && \
    tar xfz lwtools-4.22.tar.gz && \
    cd lwtools-4.22 && \
    make && \
    make install && \
    cd .. && \
    rm -rf lwtools-4.22 && \
    rm lwtools-4.22.tar.gz

RUN cd ${USER_PATH}/dragon && \
    wget http://perso.b2b2c.ca/~sarrazip/dev/cmoc-0.1.90.tar.gz && \
    tar xfz cmoc-0.1.90.tar.gz && \
    cd cmoc-0.1.90 && \
    ./configure && \
    make && \
    make install && \
    cd .. && \
    rm -rf cmoc-0.1.90 && \
    rm cmoc-0.1.90.tar.gz


# Our glue tools
RUN mkdir -p ${USER_PATH}/dragon
COPY dragon/tools/* ${USER_PATH}/dragon/


WORKDIR ${USER_PATH}/dragon


# Keeps the container open
CMD "/bin/bash"

