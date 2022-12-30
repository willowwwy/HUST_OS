#设置特殊标识设为#OK#，表示已写入
str_insert="#OK# $USER `date +%Y-%m-%d,%H:%M:%S`"  
 
#用$1接收第一个命令行参数(目录)；*.txt检查文件后缀是否为txt
for ofile in $1/*.txt   
do
	if [ `grep -c "#OK#" $ofile` -eq 0 ];then
		#未写入，向文件中追加写入字符串str_insert
		echo $str_insert >> $ofile                   
	else
#已写入，用sed更新字符串
sed -i "/#OK#/c $str_insert" $ofile     			
fi
done
