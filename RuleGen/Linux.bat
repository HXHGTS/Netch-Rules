cat route4.temp | gawk '{print $1,$2}' > route4.txt

cat route6.temp | gawk '{print $3}' | grep -v 'fe80' | grep -v 'ff00' | grep -v '/0' | grep -v '::1/128' > route6.txt
