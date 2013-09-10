== Manual Steps ==

* Make /usr/local/bin/allow_nonroot_gpio_pin_access.sh sudoable without a password
  * visudo
    * piano ALL=(ALL) NOPASSWD: /usr/local/bin/allow_nonroot_gpio_pin_access.sh