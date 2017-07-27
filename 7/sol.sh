# part 1
sed "s/\(.\)\1\{3,\}/\1\1/g" input.dat | grep -e "\[[^]]*\([[:alpha:]]\)\([[:alpha:]]\)\2\1[^]]*\]" -v | sed "s/\[[^]]\+\]/ /g" | grep -e "\(.\)\(.\)\2\1" -c

# part 2
sed "s/\(.\)\1\{2,\}/\1\1/g" input.dat | grep --colour -e "\(\][[:alpha:]]*\([[:alpha:]]\)\([[:alpha:]]\)\2.*\[[[:alpha:]]*\3\2\3\)\|\(\[[[:alpha:]]*\([[:alpha:]]\)\([[:alpha:]]\)\5.*\][[:alpha:]]*\6\5\6\)\|\(^[[:alpha:]]*\([[:alpha:]]\)\([[:alpha:]]\)\8.*\[[[:alpha:]]*\9\8\9\)" -c
