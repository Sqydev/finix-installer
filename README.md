To test do:
    make run
    if vm:
        sudo -i
        mount -t 9p INSTALLER /mnt(or if doesn't work: mount -t 9p -o trans=virtio INSTALLER /mnt)
