set -e

cd runtime
tsc
cp core.js ../server
cp core.d.ts ../server/sample/exhibition
cd $OLDPWD

cd server
cmake --build .
cd $OLDPWD

cd server/sample/exhibition
tsc
cd $OLDPWD