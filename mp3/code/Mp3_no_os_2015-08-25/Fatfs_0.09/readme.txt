Fatfs_0.09�ļ���������ļ�����diskio.c�ļ���updates.txt�ļ��������ļ�����Fatfs�ٷ���վ���ص�ff0.09��ѹ�����src�ļ���������ļ���û�����κθı䡣

diskio.c�ļ����Լ���д����洢������صĵײ������ļ���������SD���ĵײ�Ӳ���ӿ������ļ�

updates.tet�ļ���Fatfs�ٷ���վ���ص�ff0.09��ѹ�����doc�ļ��������updates.txt�ļ��������Ǹ����汾�ĸ���˵��



ע�⣺
�����а�
FRESULT f_mount (
	BYTE vol,		/* Logical drive number to be mounted/unmounted */
	FATFS *fs		/* Pointer to new file system object (NULL for unmount)*/
)�����е�
rfs->fs_type = 0;		/* Clear old fs object */		   
��Ϊ
//rfs->fs_type = 0;		/* Clear old fs object */		   //�˴������޸�

������Ĳ�����FATFS�ļ�ϵͳǶ�׶�ȡSD���������