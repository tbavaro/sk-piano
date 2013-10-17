#!/usr/bin/env ruby

require "fileutils"

raise "Must run as root" unless Process.uid == 0

# services, ordered by precedence
services = [
  "piano",
  "piano-simulator"
]
SERVICES_DIR = "/lib/systemd/system"

# set up services
services.each do |service|
  puts "Setting up service: #{service}"
  service_file = "#{service}.service"
  FileUtils.copy("startup/#{service_file}", SERVICES_DIR)
  system("systemctl", "enable", service_file) or raise "can't enable service"
  system("systemctl", "restart", service_file) or raise "can't start service"
end
