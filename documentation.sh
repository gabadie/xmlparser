echo "execution of doxygen"
doxygen src/doxyConfig  &> doc.log

echo "creating pdf"
cd doxygen/latex 
make  &> ../../doc.log
rm ../../doc.log