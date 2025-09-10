find "$1" -type f -regextype posix-extended -regex ".*\/$2"

# output
# CSE_5C2@debianpc-02:~/230905232/Lab_2$ ./2.sh . '[^/]*\.[^/]{1}$'
# ./test.a
