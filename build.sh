set -e

cd runtime
tsc
cd ../server
cmake --build .
cd ..