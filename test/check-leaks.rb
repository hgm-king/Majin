
cmd = "valgrind --leak-check=full \\
         --show-leak-kinds=all \\
         --track-origins=yes \\
         --verbose \\
         --log-file=valgrind-out.txt \\
	"

for arg in ARGV
   cmd += arg+' '
end

puts cmd+"\n"
system(cmd);
