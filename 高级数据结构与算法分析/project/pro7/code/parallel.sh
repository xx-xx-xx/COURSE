bin/hadoop jar wc.jar WordCount ~/c01dkit/ADS/hadoop-3.2.1/input ~/c01dkit/ADS/hadoop-3.2.1/output
sort ~/c01dkit/ADS/hadoop-3.2.1/output/part-r-00000 -k 2rn -o result
