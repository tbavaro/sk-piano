#!/usr/bin/env ruby

require "fileutils"

raise "Must run as root" unless Process.uid == 0

# services, ordered by precedence
services = [
  "spidev-init",
  "gpio-permissions",
  "piano"
]
SERVICES_DIR = "/lib/systemd/system"

# scripts, grouped by destination dir
scripts_by_destination = {
  "/usr/local/bin" => [
    "spidev_init.sh",
    "gpio_permissions.sh",
    "allow_nonroot_gpio_pin_access.sh"
  ],
  "/etc/sudoers.d" => [
    "piano_sudoer"
  ]
}

# install scripts
scripts_by_destination.each do |dest_dir, scripts|
  scripts.each do |script|
    puts "Installing script: #{script}"
    FileUtils.copy(script, dest_dir)
  end
end

# set up services
services.each do |service|
  puts "Setting up service: #{service}"
  service_file = "#{service}.service"
  FileUtils.copy(service_file, SERVICES_DIR)
  system("systemctl", "enable", service_file) or raise "can't enable service"
  system("systemctl", "restart", service_file) or raise "can't start service"
end
