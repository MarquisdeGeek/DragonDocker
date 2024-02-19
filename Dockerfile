FROM ubuntu

# Base OS and Dragon tools
RUN apt-get update && \
    apt-get install --no-install-recommends --no-install-suggests -y wget software-properties-common dirmngr apt-transport-https lsb-release ca-certificates gpg-agent unzip git && \
    add-apt-repository -y ppa:sixxie/ppa && \
    apt-get update && \
    apt-get install -y xroar asm6809 && \
    apt-get remove --purge --auto-remove -y

# The ROM
RUN cd /root && \
    wget -O romdata.zip https://archive.worldofdragon.org/phpBB3/download/file.php?id=5875 && \
    unzip romdata.zip && \
    mkdir -p dragon/roms && \
    cat D32-10 D32-11 >dragon/roms/d32.rom && \
    rm D32-10 D32-11 romdata.zip

# BASIC compilation tools
# This uses a really old version, because it worked with less fuss than the new ones :(
RUN cd /root/dragon && \
    apt-get install -y python2 && \
    wget -O pydc.zip https://github.com/jedie/PyDragon32/archive/refs/heads/master.zip && \
    mkdir contrib && \
    unzip pydc.zip && \
    mv PyDragon32-master contrib && \
    rm pydc.zip && \
    apt-get remove --purge --auto-remove -y


# Our glue tools
COPY dragon/tools/* /root/dragon

WORKDIR /root/dragon

# Keeps the container open
CMD "/bin/bash"

