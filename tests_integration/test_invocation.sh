
# $1 is the xmltool binary path
# $2 is the build tmp directory
# $3 is the test directory name

TEST_STATUS=0
TEST_PARAMS=`cat $3/params.txt`
TEST_RETURNCODE=`cat $3/returncode.txt`
TEST_STDOUT_PATH="$3/stdout.txt"
TEST_STDERR_PATH="$3/stderr.txt"

TEST_TMP_STDOUT_PATH="$2`basename $3`.stdout.log"
TEST_TMP_STDERR_PATH="$2`basename $3`.stderr.log"

./$1 $TEST_PARAMS > $TEST_TMP_STDOUT_PATH 2> $TEST_TMP_STDERR_PATH

if [ $? -ne $TEST_RETURNCODE ]
then
    echo "$2: returned $? instead of $TEST_RETURNCODE";
    TEST_STATUS=1;
fi;

diff -wB $TEST_TMP_STDOUT_PATH $TEST_STDOUT_PATH > /dev/null
if [ $? -ne 0 ]
then
    echo "$2: stdout doesn't match";
    TEST_STATUS=1;
fi;

diff -wB $TEST_TMP_STDERR_PATH $TEST_STDERR_PATH > /dev/null
if [ $? -ne 0 ]
then
    echo "$2: stderr doesn't match";
    TEST_STATUS=1;
fi;

exit $TEST_STATUS
