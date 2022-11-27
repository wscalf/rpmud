set -e

cd runtime
tsc
cp core.js ../server
cp core.d.ts ../server/sample/exhibition
cp core.d.ts ../server/scripting/tests/test_scripts
cd $OLDPWD

cd server/scripting/tests/test_scripts
tsc
cd $OLDPWD

cd server
cmake --build .
cd $OLDPWD

cd server/sample/exhibition
tsc
cd $OLDPWD