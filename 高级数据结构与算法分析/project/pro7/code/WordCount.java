import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.IOException;
import java.util.StringTokenizer;

public class WordCount {

    /**
     * Object      : the content of input file
     * Text        : every single line of input data
     * Text        : output the type of key
     * IntWritable : output the type of value
     */
    private static class WordCountMapper extends Mapper<Object, Text, Text, IntWritable> {
        @Override
        protected void map(Object key, Text value, Context context) throws IOException, InterruptedException {
            StringTokenizer itr = new StringTokenizer(value.toString());
            while (itr.hasMoreTokens()) {
                context.write(new Text(itr.nextToken()), new IntWritable(1));
            }
        }
    }

    /**
     * Text         :  Mapper the input key
     * IntWritable  :  Mapper the input value
     * Text         :  Reducer the output key
     * IntWritable  :  Reducer the output value
     */
    private static class WordCountReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
        @Override
        protected void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {
            int count = 0;
            for (IntWritable item : values) {
                count += item.get();
            }
            context.write(key, new IntWritable(count));
        }
    }

    public static void main(String[] args) throws IOException, ClassNotFoundException, InterruptedException {
        // create configuration
        Configuration configuration = new Configuration();
        // set job of hadoop jobName is WordCount
        Job job = Job.getInstance(configuration, "WordCount");
        // set jar
        job.setJarByClass(WordCount.class);
        // set Mapper's class
        job.setMapperClass(WordCountMapper.class);
        // set Reducer's class
        job.setReducerClass(WordCountReducer.class);
        // set the type of key and value outputted
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);

        // set input & output path
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));
        // exit after executing job
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}