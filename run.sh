./semInit

./producer 0 & echo $!
./producer 1 &  echo $!
./producer 2 &  echo $!
 
./consumer 0 &  echo $!
./consumer 0 &  echo $!
./consumer 0 &  echo $!


./consumer 1 &  echo $!
./consumer 1 &  echo $!
./consumer 1 &  echo $!

./consumer 2 &  echo $!
./consumer 2 &  echo $!
./consumer 2 &  echo $!

./sproducer & echo $!
./sproducer & echo &!
