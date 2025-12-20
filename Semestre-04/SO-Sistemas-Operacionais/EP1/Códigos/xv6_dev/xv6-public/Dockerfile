FROM ubuntu:20.04

# Definir timezone para evitar interação
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=America/Sao_Paulo

# Configurar timezone
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Instalar pacotes
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-multilib \
    qemu-system-x86 \
    gdb \
    tmux \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /xv6-public
CMD ["bash"]
