1. Prepare Your Environment: 
Update System. 
Code 
sudo apt update 
sudo apt upgrade 
• Install Java Development Kit (JDK): Hadoop requires Java. Install OpenJDK 8 or 11. 
Code 
  
sudo apt install openjdk-8-jdk # Or openjdk-11-jdk 
Verify Java Installation. 
Code 
  
java -version 
• Install SSH: Hadoop uses SSH for communication between nodes (even in pseudo-distributed mode for localhost 
access). 
Code 
sudo apt install ssh 

2. Create a Hadoop User (Optional but Recommended): 
• Create a dedicated user for Hadoop for better security and organization.  
Code 
sudo addgroup hadoop_group 
sudo adduser --ingroup hadoop_group hadoop_user 
sudo usermod -aG sudo hadoop_user # Grant sudo privileges if needed 
Switch to the new user. 
Code 
su - hadoop_user 
  
3. Configure SSH for Passwordless Access: 
• Generate an SSH key pair for the hadoop_user (if not already done). 
Code 
ssh-keygen -t rsa -P "" 
• Add the public key to authorized keys for passwordless login to localhost. 
Code  
/home/hadoop_user/.ssh/id_rsa.pub 
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys 
Verify passwordless SSH. 
Code 
ssh localhost 

4. Download and Extract Hadoop: 
• Download the desired stable Hadoop release from the Apache Hadoop website. 
Code 
wget https://dlcdn.apache.org/hadoop/common/hadoop-3.4.2/hadoop-3.4.2.tar.gz # Replace x.x.x with the version 
or  
wget https://downloads.apache.org/hadoop/common/hadoop-3.3.6/hadoop-3.3.6.tar.gz 
Extract the downloaded archive. 
Code 
tar -xvzf hadoop-3.4.2.tar.gz 
• Move the extracted directory to a suitable location, e.g., /usr/local/hadoop: 
Code 
sudo mv hadoop-3.4.2 /usr/local/hadoop 
Change ownership to the Hadoop user. 
Code 
sudo chown -R hadoop_user:hadoop_group /usr/local/hadoop 
  
5. Configure Hadoop Environment Variables: 
• Nano ~/.bashrc (for the hadoop_user) and add the following: 
Code 
export HADOOP_HOME=/usr/local/hadoop 
export HADOOP_INSTALL=$HADOOP_HOME 
export HADOOP_MAPRED_HOME=$HADOOP_HOME 
export HADOOP_COMMON_HOME=$HADOOP_HOME 
export HADOOP_HDFS_HOME=$HADOOP_HOME 
export YARN_HOME=$HADOOP_HOME 
export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_HOME/lib/native 
export PATH=$PATH:$HADOOP_HOME/sbin:$HADOOP_HOME/bin 
export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64 
# Adjust if using a different Java version or path 
Apply the changes. 
Code 
source ~/.bashrc 

  
6. Configure Hadoop XML Files: 
• Navigate to  
• cd /usr/local/hadoop/etc/hadoop. 
• Edit the following files: 
o Sudo nano hadoop-env.sh: Set JAVA_HOME to your Java installation path. 
o export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64 
o source hadoop-env.sh 
o core-site.xml:  
nano core-site.xml 
Code 
<?xml version="1.0" encoding="UTF-8"?> 
<configuration> 
<property> 
<name>fs.defaultFS</name> 
<value>hdfs://localhost:9000</value> 
</property> 
</configuration> 

  
nano hdfs-site.xml: Define data and name node directories. Create these directories first. 
Code 
<configuration> 
    <property> 
        <name>dfs.replication</name> 
        <value>1</value> 
    </property> 
    <property> 
        <name>dfs.namenode.name.dir</name> 
        <value>file:///usr/local/hadoop/hdfs/namenode</value> 
    </property> 
    <property> 
        <name>dfs.datanode.data.dir</name> 
        <value>file:///usr/local/hadoop/hdfs/datanode</value> 
    </property> 
</configuration> 
 
Create directories: 
mkdir -p /usr/local/hadoop/hdfs/namenode 
mkdir -p /usr/local/hadoop/hdfs/datanode 
 
cp mapred-site.xml.template mapred-site.xml 
nano mapred-site.xml: (Rename mapred-site.xml.template to mapred-site.xml) 
 
Code 
        <configuration> 
    <property> 
        <name>mapreduce.framework.name</name> 
        <value>yarn</value> 
    </property> 
</configuration> 
Nano yarn-site.xml. 
Code 
        <configuration> 
 
    <property> 
        <name>yarn.resourcemanager.hostname</name> 
        <value>localhost</value> 
    </property> 
 
    <property> 
        <name>yarn.nodemanager.aux-services</name> 
        <value>mapreduce_shuffle</value> 
    </property> 
 
    <property> 
        <name>yarn.nodemanager.aux-services.mapreduce.shuffle.class</name> 
        <value>org.apache.hadoop.mapred.ShuffleHandler</value> 
    </property> 
 
    <!-- Disable privileged priority settings --> 
    <property> 
        <name>yarn.nodemanager.container-executor.class</name> 
        <value>org.apache.hadoop.yarn.server.nodemanager.DefaultContainerExecutor</value> 
    </property> 
 
    <property> 
        <name>yarn.nodemanager.linux-container-executor.nonsecure-mode.limit-users</name> 
        <value>false</value> 
    </property> 
 
</configuration> 

  
7. Format HDFS NameNode: 
• Caution: This will erase existing HDFS data. 
Code 
hdfs namenode -format 
  
8. Start Hadoop Services: 
Start HDFS daemons. 
Code 
start-dfs.sh 
Start YARN daemons. 
  
Code 
start-yarn.sh 
  
9. Verify Installation: 
Check running Java processes. 
Code 
jps 
You should see NameNode, DataNode, ResourceManager, NodeManager, and SecondaryNameNode. 
• Access Hadoop UI in your browser: 
o NameNode: http://localhost:9870 (or 50070 for older versions) 
o ResourceManager: http://localhost:8088 

10. Stop Hadoop Services: 
Stop YARN daemons. 
Code 
stop-yarn.sh 
Stop HDFS daemons. 
Code 
stop-dfs.sh
