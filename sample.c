#!/bin/bash
yum update -y
yum -y install httpd
systemctl enable httpd
systemctl start httpd
echo '<html><h1>Hello World! This is server 1.</h1></html>' > /var/www/html/index.html

  
9:21 pm
sudo apt update
9:22 pm
sudo cat /etc/debian.cnf
9:22 pm
sudo apt install mysql -server
