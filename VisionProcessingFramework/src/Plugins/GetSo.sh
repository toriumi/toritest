#!/bin/sh

#Input Plugin
cp Bin/Bin.so ../../lib/Plugins/input/
cp Sensor/Sensor.so ../../lib/Plugins/input/
cp Avi/Avi.so ../../lib/Plugins/input/

#ISP Plugin
cp BayerAddGain/BayerAddGain.so ../../lib/Plugins/isp/
cp ColorMatrix/ColorMatrix.so ../../lib/Plugins/isp/
cp Demosaic/Demosaic.so ../../lib/Plugins/isp/
cp EdgeEnhancement/EdgeEnhancement.so ../../lib/Plugins/isp/
cp GammaCorrect/GammaCorrect.so ../../lib/Plugins/isp/
cp EdgeEnhancement/EdgeEnhancement.so ../../lib/Plugins/isp/
cp ResizeImage/ResizeImage.so ../../lib/Plugins/isp/
cp WhiteBalanceGain/WhiteBalanceGain.so ../../lib/Plugins/isp/
cp TemplatePlugin/Template.so ../../lib/Plugins/isp/

#Output Plugin
cp OpenCVDisp/OpenCVDisp.so ../../lib/Plugins/output/
cp OpenGLDisp/OpenGLDisp.so ../../lib/Plugins/output/
cp SensorFocus/SensorFocus.so ../../lib/Plugins/output/
cp SaveToAvi/SaveToAvi.so ../../lib/Plugins/output/
