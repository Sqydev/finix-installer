To test do:
    make run
    if vm:
        sudo -i
        mkdir -p /etc/calamares
        mount -t 9p CALAMARES /mnt/calamares(or if doesn't work: mount -t 9p -o trans=virtio CALAMARES /mnt/calamares)
        calamares
