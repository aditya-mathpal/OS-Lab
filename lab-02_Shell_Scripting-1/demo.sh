echo "Script name: $0"
echo "First arg: $1"
echo "Second arg: $2"
echo "Number of args: $#"
echo "All args"
for arg in "$@"; do
	echo "- $arg"
done
