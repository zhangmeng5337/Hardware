Fatfs_0.09文件夹里面的文件除了diskio.c文件和updates.txt文件外其他文件都是Fatfs官方网站下载的ff0.09解压包里的src文件夹里面的文件，没有作任何改变。

diskio.c文件是自己编写的与存储介质相关的底层驱动文件，这里是SD卡的底层硬件接口驱动文件

updates.tet文件是Fatfs官方网站下载的ff0.09解压包里的doc文件夹里面的updates.txt文件，内容是各个版本的更新说明



注意：
程序中把
FRESULT f_mount (
	BYTE vol,		/* Logical drive number to be mounted/unmounted */
	FATFS *fs		/* Pointer to new file system object (NULL for unmount)*/
)函数中的
rfs->fs_type = 0;		/* Clear old fs object */		   
改为
//rfs->fs_type = 0;		/* Clear old fs object */		   //此处作了修改

如果不改不能用FATFS文件系统嵌套读取SD卡里的数据