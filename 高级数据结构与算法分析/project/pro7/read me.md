# readme 

### Develop Environment

- CLion 

  Version: 2020.1.1 Build #CL-201.7223.86, built on April 29, 2020

  Runtime version: 11.0.6+8-b765.40 amd64

- Linux version: 5.3.0-55-generic (buildd@lcy01-amd64-009)#49-Ubuntu SMP Thu May 21 12:47:19 UTC 2020

- VM: OpenJDK 64-Bit Server VM by JetBrains s.r.o Windows 10 10.0 

  GC: ParNew, ConcurrentMarkSweep 

  Memory: 1987M 

  Cores: 8 

  Registry: run.processes.with.pty=TRUE, ide.suppress.double.click.handler=true

### Compiler

- gcc version 9.2.1 20191008 (Ubuntu 9.2.1-9ubuntu2)
- jdk version 14.0.1

### File Arrangement

**code**

- WordCount.java

  Java program to implement word count

- parallel.sh

  script of **parallel MapReduce** program

- serial.cpp

  source file of **serial** program

- serial.exe

  executive file of serial.cpp

- time_test

  data files to test the time of programs

  - 1000.txt
  - 2000.txt
  - ...
  - 1000000.txt

- correctness_test

  data to test the correctness of programs

  - test1.txt
  - test2.txt
  - ...
  - test7.txt

- text_splitting

  - 1.txt

    file containing 1,000,000 words

  - splitting.cpp

    program to split 1.txt which contains 1,000,000 words into n files, where n is from 1 to 128

  - splitting.exe

    executive file of splitting.cpp

  - 1

    n split files of 1.txt

  - 2

  - ...

  - 128

- for test.gif

  methods to test the program in command prompt

- test_result

  - test_result.xlsx

    test results of the 2 methods in table

  - single file

    test results in the way of single file at one time

    - P-1

      1 file in parallel algorithm

    - ...

    - P-128

    - S-1

      1 file in serial algorithm

    - ...
    - S-128

  - multi file

    test results in the way of multi files at the same time

    - P-1

      file with 1 words in parallel algorithm

    - ...

    - P-1000000

    - S-1

      file with 1 words in serial algorithm

    - ...
    - S-1000000

**document**

- report.pdf

**readme.pdf**

### For test

- Install **jdk** in https://www.oracle.com/java/technologies/javase-jdk14-downloads.html
- Install **Hadoop** in https://mirror.bit.edu.cn/apache/hadoop/common/hadoop-3.2.1/
- Refer to **for test.gif** in the folder named **code**.
- If there are any further problem, refer to https://hadoop.apache.org/docs/stable/hadoop-mapreduce-client/hadoop-mapreduce-client-core/MapReduceTutorial.html#Source_Code for instruction.
