# (c) 2013 jw@suse.de
# driver for a Graphtec Silhouette Cameo plotter.
# modelled after https://github.com/nosliwneb/robocut.git 
#
import usb.core
    
# taken from robocut/Plotter.h:53 ff

VENDOR_ID_GRAPHTEC = 0x0b4d 
PRODUCT_ID_CC200_20 = 0x110a
PRODUCT_ID_CC300_20 = 0x111a
PRODUCT_ID_SILHOUETTE_SD_1 = 0x111c
PRODUCT_ID_SILHOUETTE_SD_2 = 0x111d
PRODUCT_ID_SILHOUETTE_CAMEO =  0x1121
PRODUCT_ID_SILHOUETTE_PORTRAIT = 0x1123

class SilhouetteCameo:
  def __init__(self):
    dev = usb.core.find(idVendor=VENDOR_ID_GRAPHTEC, idProduct=PRODUCT_ID_SILHOUETTE_CAMEO)
    if dev is None:
      raise ValueError('Silhouette Cameo not found. Check USB and Power')
    print "Silhouette Cameo found on usb bus=%d addr=%d" % (dev.bus, dev.address)

    if dev.is_kernel_driver_active(0):
      print "is_kernel_driver_active(0) returned nonzero"
      if dev.detach_kernel_driver(0):
        print "detach_kernel_driver(0) returned nonzero"
    dev.reset();

    dev.set_configuration()
    try:
      dev.set_interface_altsetting()      # Probably not really necessary.
    except usb.core.USBError:
      pass
    self.dev = dev

  def write(self, string):
    return self.dev.write(1, string) 
