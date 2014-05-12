#!/bin/sh
# Buttkicker script

sudo echo -n "0000:00:1a.0" | tee /sys/bus/pci/drivers/ehci_hcd/unbind
sudo echo -n "0000:00:1d.0" | tee /sys/bus/pci/drivers/ehci_hcd/unbind
sudo echo -n "0000:00:1a.0" | tee /sys/bus/pci/drivers/ehci_hcd/bind
sudo echo -n "0000:00:1d.0" | tee /sys/bus/pci/drivers/ehci_hcd/bind
